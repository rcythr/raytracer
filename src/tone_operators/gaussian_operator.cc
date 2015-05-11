#include "tone_operators/gaussian_operator.hpp"

using namespace raytracer;

void GaussianOperator::apply(ViewPlanePtr ptr) {
    /*
    glm::vec3 v11, v12, v13, v21, v22, v23, v31, v32, v33;

    glm::vec3 zero;

    // Create temporary storage for the output data.
    glm::vec3** out_data = new glm::vec3* [height];
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        out_data[i] = new glm::vec3[width];
    }

    // Apply filter
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        for (int64_t j = 0; j < (int64_t)width; ++j) {
            v11 = (i - 1 >= 0 && j - 1 >= 0) ? 0.0625f * data[i - 1][j - 1]
                                             : zero;
            v12 = (i - 1 >= 0) ? 0.128f * data[i - 1][j] : zero;
            v13 = (i - 1 >= 0 && j + 1 < width) ? 0.0625f * data[i - 1][j + 1]
                                                : zero;

            v21 = (j - 1 >= 0) ? 0.128f * data[i][j - 1] : zero;
            v22 = 0.256f * data[i][j];
            v23 = (j + 1 < width) ? 0.128f * data[i][j + 1] : zero;

            v31 = (i + 1 < height && j - 1 >= 0) ? 0.0625f * data[i + 1][j - 1]
                                                 : zero;
            v32 = (i + 1 < height) ? 0.128f * data[i + 1][j] : zero;
            v33 = (i + 1 < height && j + 1 < width)
                      ? 0.0625f * data[i + 1][j + 1]
                      : zero;

            out_data[i][j] =
                v11 + v12 + v13 + v21 + v22 + v23 + v31 + v32 + v33;
        }
    }

    // Delete the old data array.
    for (int64_t i = 0; i < (int64_t)height; ++i) {
        delete[] data[i];
    }
    delete[] data;

    // Replace it with the new data array.
    data = out_data;
    */
}