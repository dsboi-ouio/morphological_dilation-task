#pragma once

#include "binary_image.hpp"
#include "structuring_element.hpp"

#include <cstddef>

namespace morphology {

class Dilation5x5 {
public:
    Dilation5x5();
    explicit Dilation5x5(StructuringElement5x5 element);

    [[nodiscard]] BinaryImage apply(const BinaryImage& input) const;

    [[nodiscard]] static constexpr std::size_t elementSize() noexcept {
        return StructuringElement5x5::kSize;
    }

private:
    static constexpr std::ptrdiff_t radius_ =
        static_cast<std::ptrdiff_t>(StructuringElement5x5::kSize / 2);
    StructuringElement5x5 element_;
};

}  // namespace morphology
