#pragma once

#include "binary_image.hpp"

#include <cstddef>

namespace morphology {

class Dilation5x5 {
public:
    [[nodiscard]] BinaryImage apply(const BinaryImage& input) const;

    [[nodiscard]] static constexpr std::size_t elementSize() noexcept {
        return 5;
    }

private:
    static constexpr std::ptrdiff_t radius_ = 2;
};

}  // namespace morphology
