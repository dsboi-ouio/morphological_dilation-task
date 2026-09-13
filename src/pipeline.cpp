#include "pipeline.hpp"

#include "dilation.hpp"
#include "erosion.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace morphology {

MorphologyPipeline::MorphologyPipeline()
    : MorphologyPipeline(MorphologyOperation::Dilation,
                         StructuringElement5x5::square()) {}

MorphologyPipeline::MorphologyPipeline(StructuringElement5x5 element)
    : MorphologyPipeline(MorphologyOperation::Dilation,
                         std::move(element)) {}

MorphologyPipeline::MorphologyPipeline(MorphologyOperation operation,
                                       StructuringElement5x5 element)
    : operation_(operation), element_(std::move(element)) {}

BinaryImage MorphologyPipeline::process(const BinaryImage& input) const {
    switch (operation_) {
        case MorphologyOperation::Dilation:
            return Dilation5x5(element_).apply(input);
        case MorphologyOperation::Erosion:
            return Erosion5x5(element_).apply(input);
    }

    throw std::invalid_argument("unknown morphology operation");
}

void MorphologyPipeline::run(std::istream& input,
                             std::ostream& output) const {
    const BinaryImage inputImage = readImage(input);
    const BinaryImage outputImage = process(inputImage);
    writeImage(outputImage, output);
}

BinaryImage MorphologyPipeline::readImage(std::istream& input) {
    BinaryImage::Matrix pixels;
    std::string line;

    while (std::getline(input, line)) {
        std::istringstream lineInput(line);
        std::vector<int> row;
        std::string token;

        while (lineInput >> token) {
            if (token != "0" && token != "1") {
                throw std::invalid_argument(
                    "input contains a value other than 0 or 1: " + token);
            }
            row.push_back(token == "1" ? 1 : 0);
        }

        if (!row.empty()) {
            pixels.push_back(std::move(row));
        }
    }

    if (pixels.empty()) {
        throw std::invalid_argument(
            "input does not contain a binary-image matrix");
    }

    return BinaryImage(std::move(pixels));
}

void MorphologyPipeline::writeImage(const BinaryImage& image,
                                    std::ostream& output) {
    for (std::size_t row = 0; row < image.rows(); ++row) {
        for (std::size_t column = 0; column < image.columns(); ++column) {
            if (column != 0) {
                output << ' ';
            }
            output << image.at(row, column);
        }
        output << '\n';
    }
}

}  // namespace morphology
