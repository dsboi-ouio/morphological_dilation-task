#include "erosion.hpp"

#include <cstddef>
#include <utility>

namespace morphology {

Erosion5x5::Erosion5x5()
    : Erosion5x5(StructuringElement5x5::square()) {}

Erosion5x5::Erosion5x5(StructuringElement5x5 element)
    : element_(std::move(element)) {}

BinaryImage Erosion5x5::apply(const BinaryImage& input) const {
    BinaryImage output(input.rows(), input.columns());

    for (std::size_t row = 0; row < input.rows(); ++row) {
        for (std::size_t column = 0; column < input.columns(); ++column) {
            bool keepForeground = true;

            for (std::ptrdiff_t rowOffset = -radius_;
                 rowOffset <= radius_ && keepForeground;
                 ++rowOffset) {
                const auto elementRow = static_cast<std::size_t>(
                    rowOffset + radius_);

                for (std::ptrdiff_t columnOffset = -radius_;
                     columnOffset <= radius_;
                     ++columnOffset) {
                    const auto elementColumn = static_cast<std::size_t>(
                        columnOffset + radius_);

                    if (!element_.isActive(elementRow, elementColumn)) {
                        continue;
                    }

                    const auto inputRow =
                        static_cast<std::ptrdiff_t>(row) + rowOffset;
                    const auto inputColumn =
                        static_cast<std::ptrdiff_t>(column) + columnOffset;

                    if (!input.inBounds(inputRow, inputColumn)) {
                        keepForeground = false;
                        break;
                    }

                    if (input.at(static_cast<std::size_t>(inputRow),
                                 static_cast<std::size_t>(inputColumn)) == 0) {
                        keepForeground = false;
                        break;
                    }
                }
            }

            if (keepForeground) {
                output.set(row, column, 1);
            }
        }
    }

    return output;
}

}  // namespace morphology
