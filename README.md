# GI Raytracer Project

## Requirements
 * A C++11 compliant compiler (e.g. **gcc** or **clang**)
 * GLM Library (currently in **include/glm**)
    
## To Setup
    mkdir build
    cd build
    cmake ..
    make
    cp ../config.example.xml config.xml
    
## To Run
The raytracer program requires an optional argument [config_name] which specifies the configuration xml file. By default it is **config.xml**. The current output should be the spew of a tree structure which represents the XML's data.

    ./raytracer [config_name]
