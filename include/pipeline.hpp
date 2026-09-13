#pragma once

#include "binary_image.hpp"
#include "dilation.hpp"

#include <iosfwd>

namespace morphology {

class DilationPipeline {
public:
    DilationPipeline();
    explicit DilationPipeline(StructuringElement5x5 element);

    [[nodiscard]] BinaryImage process(const BinaryImage& input) const;

    void run(std::istream& input,
             std::ostream& output) const;

private:
    static BinaryImage readImage(std::istream& input);
    static void writeImage(const BinaryImage& image, std::ostream& output);

    Dilation5x5 dilation_;
};

}  // namespace morphology
