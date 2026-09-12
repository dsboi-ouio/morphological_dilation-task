#pragma once

#include <cstddef>
#include <vector>

namespace morphology {

class BinaryImage {
public:
    using Matrix = std::vector<std::vector<int>>;

    explicit BinaryImage(Matrix pixels);
    BinaryImage(std::size_t rows, std::size_t columns, int initialValue = 0);

    [[nodiscard]] std::size_t rows() const noexcept;
    [[nodiscard]] std::size_t columns() const noexcept;
    [[nodiscard]] bool inBounds(std::ptrdiff_t row,
                                std::ptrdiff_t column) const noexcept;

    [[nodiscard]] int at(std::size_t row, std::size_t column) const;
    void set(std::size_t row, std::size_t column, int value);

    [[nodiscard]] const Matrix& pixels() const noexcept;

    friend bool operator==(const BinaryImage& left,
                           const BinaryImage& right) noexcept;

private:
    static void validatePixel(int value);
    void validate() const;

    Matrix pixels_;
};

}  // namespace morphology
