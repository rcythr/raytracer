// Copyright (c) 2013 Richard Laughlin
// All Rights Reserved

#pragma once
#include <string>
#include <unordered_map>

namespace raytracer {

typedef std::unordered_map<std::string, std::string> ParamMap;

enum class ParserState {
    None,
    InOpeningTagName,
    InTagAttribute,
    InTagAttributeFinished,
    InTagAttributeValue,
    InTagAttributeSingleQuoteString,
    InTagAttributeDoubleQuoteString,
    InClosingTagName
};

/*
 * A no nonsense xml parser for C++11. It's certainly not perfect, but it will
 *parse AT LEAST the valid xml inputs.
 *
 * To use simply implement a handler class with 3 methods:
 *     1. open(std::string& tag_name, std::map<std::string, std::string>&
 *attributes)
 *     2. ascii(std::string& ascii)
 *     3. close(std::string& tag_name)
 */
template <typename Handler>
void parseXml(const std::string& data, Handler& handler) {
    ParserState state = ParserState::None;

    bool also_closes = false;
    std::string ascii_buf, tag_name, attribute_name, attribute_value;
    ParamMap attributes;
    size_t len = data.length();
    char c;
    for (size_t i = 0; i < len; ++i) {
        c = data[i];
        switch (state) {
            case ParserState::None: {
                switch (c) {
                    case '<': {
                        if (i + 1 > len) { /*TODO: PARSE ERROR*/
                        }
                        if (ascii_buf != "") {
                            handler.ascii(ascii_buf);
                        }

                        // If next char is / it's a closer
                        if (data[i + 1] == '/') {
                            ++i;
                            state = ParserState::InClosingTagName;

                        } else  // it's an opener
                        {
                            state = ParserState::InOpeningTagName;
                        }
                        ascii_buf.clear();
                    } break;

                    default: { ascii_buf += c; }
                }
            } break;

            case ParserState::InClosingTagName: {
                switch (c) {
                    case '>': {
                        handler.close(tag_name);
                        tag_name.clear();
                        state = ParserState::None;
                    } break;

                    default: {
                        tag_name += c;
                    } break;
                }
            } break;

            case ParserState::InOpeningTagName: {
                switch (c) {
                    case '\t':
                    case '\r':
                    case '\n':
                    case ' ':  // Name is over, move into the first attribute.
                    {
                        state = ParserState::InTagAttribute;
                    } break;

                    case '/': {
                        also_closes = true;
                    } break;

                    case '>': {
                        handler.open(tag_name, attributes);
                        if (also_closes) {
                            handler.close(tag_name);
                        }

                        tag_name.clear();
                        attributes.clear();
                        state = ParserState::None;
                    } break;

                    default: {
                        tag_name += c;
                    } break;
                }
            } break;

            case ParserState::InTagAttribute: {
                switch (c) {
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n': {
                        if (attribute_name != "") {
                            state = ParserState::InTagAttributeFinished;
                        }
                    } break;

                    case '=':  // Okay, now we're in the value.
                    {
                        state = ParserState::InTagAttributeValue;
                    } break;

                    case '/': {
                        also_closes = true;
                    } break;

                    case '>': {
                        if (attribute_name != "") {
                            attributes.insert(std::make_pair(attribute_name,
                                                             std::string("")));
                        }

                        handler.open(tag_name, attributes);
                        if (also_closes) {
                            handler.close(tag_name);
                            also_closes = false;
                        }
                        tag_name.clear();
                        attribute_name.clear();
                        attributes.clear();
                        state = ParserState::None;
                    } break;

                    default: {
                        attribute_name += c;
                    } break;
                }
            } break;

            case ParserState::InTagAttributeFinished: {
                switch (c) {
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n':
                        break;

                    case '=': {
                        state = ParserState::InTagAttributeValue;
                    } break;

                    default: {
                        attributes.insert(
                            std::make_pair(attribute_name, std::string("")));
                        attribute_name.clear();
                        attribute_name += c;
                        state = ParserState::InTagAttribute;
                    } break;
                }
            } break;

            case ParserState::InTagAttributeValue: {
                switch (c) {
                    case '\t':
                    case '\n':
                    case '\r':
                        break;

                    case ' ': {
                        if (attribute_value == "") {
                            break;
                        }
                        attributes.insert(
                            std::make_pair(attribute_name, attribute_value));
                        attribute_name.clear();
                        attribute_value.clear();
                        state = ParserState::InTagAttribute;
                    } break;

                    case '>': {
                        attributes.insert(
                            std::make_pair(attribute_name, attribute_value));
                        handler.open(tag_name, attributes);
                        tag_name.clear();
                        attribute_name.clear();
                        attribute_value.clear();
                        attributes.clear();
                        state = ParserState::None;
                    } break;

                    case '\'': {
                        attribute_value.clear();
                        state = ParserState::InTagAttributeSingleQuoteString;
                    } break;

                    case '\"': {
                        attribute_value.clear();
                        state = ParserState::InTagAttributeDoubleQuoteString;
                    } break;

                    default: { attribute_value += c; }
                }
            } break;

            case ParserState::InTagAttributeSingleQuoteString: {
                switch (c) {
                    case '\'': {
                        attributes.insert(
                            std::make_pair(attribute_name, attribute_value));
                        attribute_name.clear();
                        attribute_value.clear();
                        state = ParserState::InTagAttribute;
                    } break;

                    case '\\': {
                        if (i + 1 >= len) { /*TODO: PARSE ERROR*/
                        }
                        if (data[i + 1] == '\'') {
                            ++i;
                            attribute_value += '\'';
                        } else {
                            attribute_value += '\\';
                        }

                    } break;

                    default: { attribute_value += c; }
                }
            } break;

            case ParserState::InTagAttributeDoubleQuoteString: {
                switch (c) {
                    case '"': {
                        attributes.insert(
                            std::make_pair(attribute_name, attribute_value));
                        attribute_name.clear();
                        attribute_value.clear();
                        state = ParserState::InTagAttribute;
                    } break;

                    case '\\': {
                        if (i + 1 >= len) { /*TODO: PARSE ERROR*/
                        }
                        if (data[i + 1] == '"') {
                            ++i;
                            attribute_value += '"';
                        } else {
                            attribute_value += '\\';
                        }

                    } break;

                    default: { attribute_value += c; }
                }
            } break;
        }
    }
}
}
