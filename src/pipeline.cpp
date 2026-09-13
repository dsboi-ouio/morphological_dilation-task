#include "pipeline.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace morphology {

DilationPipeline::DilationPipeline()
    : DilationPipeline(StructuringElement5x5::square()) {}

DilationPipeline::DilationPipeline(StructuringElement5x5 element)
    : dilation_(std::move(element)) {}

BinaryImage DilationPipeline::process(const BinaryImage& input) const {
    return dilation_.apply(input);
}

void DilationPipeline::run(std::istream& input,
                           std::ostream& output) const {
    const BinaryImage inputImage = readImage(input);
    const BinaryImage outputImage = process(inputImage);
    writeImage(outputImage, output);
}

BinaryImage DilationPipeline::readImage(std::istream& input) {
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

void DilationPipeline::writeImage(const BinaryImage& image,
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
