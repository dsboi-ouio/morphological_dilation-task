#pragma once

#include <array>
#include <cstddef>

namespace morphology {

class StructuringElement5x5 {
public:
    static constexpr std::size_t kSize = 5;
    using Row = std::array<bool, kSize>;
    using Mask = std::array<Row, kSize>;

    explicit StructuringElement5x5(Mask mask);

    [[nodiscard]] static StructuringElement5x5 square();
    [[nodiscard]] static StructuringElement5x5 disk();

    [[nodiscard]] bool isActive(std::size_t row,
                                std::size_t column) const;

private:
    void validate() const;

    Mask mask_;
};

}  // namespace morphology
