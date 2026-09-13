#include "structuring_element.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace morphology {

StructuringElement5x5::StructuringElement5x5(Mask mask)
    : mask_(std::move(mask)) {
    validate();
}

StructuringElement5x5 StructuringElement5x5::square() {
    Mask mask{};
    for (auto& row : mask) {
        row.fill(true);
    }
    return StructuringElement5x5(std::move(mask));
}

StructuringElement5x5 StructuringElement5x5::disk() {
    constexpr std::ptrdiff_t radius = 2;
    Mask mask{};

    for (std::size_t row = 0; row < kSize; ++row) {
        for (std::size_t column = 0; column < kSize; ++column) {
            const auto rowOffset =
                static_cast<std::ptrdiff_t>(row) - radius;
            const auto columnOffset =
                static_cast<std::ptrdiff_t>(column) - radius;

            mask[row][column] =
                rowOffset * rowOffset + columnOffset * columnOffset <=
                radius * radius;
        }
    }

    return StructuringElement5x5(std::move(mask));
}

bool StructuringElement5x5::isActive(std::size_t row,
                                     std::size_t column) const {
    return mask_.at(row).at(column);
}

void StructuringElement5x5::validate() const {
    for (const auto& row : mask_) {
        for (const bool active : row) {
            if (active) {
                return;
            }
        }
    }

    throw std::invalid_argument(
        "a structuring element must contain an active position");
}

}  // namespace morphology
