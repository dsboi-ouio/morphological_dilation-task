#pragma once

#include "binary_image.hpp"
#include "structuring_element.hpp"

#include <iosfwd>

namespace morphology {

enum class MorphologyOperation {
    Dilation,
    Erosion
};

class MorphologyPipeline {
public:
    MorphologyPipeline();
    explicit MorphologyPipeline(StructuringElement5x5 element);
    MorphologyPipeline(MorphologyOperation operation,
                       StructuringElement5x5 element);

    [[nodiscard]] BinaryImage process(const BinaryImage& input) const;

    void run(std::istream& input,
             std::ostream& output) const;

private:
    static BinaryImage readImage(std::istream& input);
    static void writeImage(const BinaryImage& image, std::ostream& output);

    MorphologyOperation operation_;
    StructuringElement5x5 element_;
};

using DilationPipeline = MorphologyPipeline;

}  // namespace morphology
