#include "dilation.hpp"

#include <cstddef>

namespace morphology {

BinaryImage Dilation5x5::apply(const BinaryImage& input) const {
    BinaryImage output(input.rows(), input.columns());

    for (std::size_t row = 0; row < input.rows(); ++row) {
        for (std::size_t column = 0; column < input.columns(); ++column) {
            if (input.at(row, column) == 0) {
                continue;
            }

            for (std::ptrdiff_t rowOffset = -radius_;
                 rowOffset <= radius_;
                 ++rowOffset) {
                for (std::ptrdiff_t columnOffset = -radius_;
                     columnOffset <= radius_;
                     ++columnOffset) {
                    const auto outputRow =
                        static_cast<std::ptrdiff_t>(row) + rowOffset;
                    const auto outputColumn =
                        static_cast<std::ptrdiff_t>(column) + columnOffset;

                    if (output.inBounds(outputRow, outputColumn)) {
                        output.set(static_cast<std::size_t>(outputRow),
                                   static_cast<std::size_t>(outputColumn),
                                   1);
                    }
                }
            }
        }
    }

    return output;
}

}  // namespace morphology
