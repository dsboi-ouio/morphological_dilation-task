#include "binary_image.hpp"

#include <stdexcept>
#include <utility>

namespace morphology {

BinaryImage::BinaryImage(Matrix pixels) : pixels_(std::move(pixels)) {
    validate();
}

BinaryImage::BinaryImage(std::size_t rows,
                         std::size_t columns,
                         int initialValue)
    : pixels_(rows, std::vector<int>(columns, initialValue)) {
    validatePixel(initialValue);
    if (rows == 0 || columns == 0) {
        throw std::invalid_argument("image dimensions must be positive");
    }
}

std::size_t BinaryImage::rows() const noexcept {
    return pixels_.size();
}

std::size_t BinaryImage::columns() const noexcept {
    return pixels_.empty() ? 0 : pixels_.front().size();
}

bool BinaryImage::inBounds(std::ptrdiff_t row,
                           std::ptrdiff_t column) const noexcept {
    return row >= 0 && column >= 0 &&
           static_cast<std::size_t>(row) < rows() &&
           static_cast<std::size_t>(column) < columns();
}

int BinaryImage::at(std::size_t row, std::size_t column) const {
    if (row >= rows() || column >= columns()) {
        throw std::out_of_range("pixel coordinate is outside the image");
    }
    return pixels_[row][column];
}

void BinaryImage::set(std::size_t row, std::size_t column, int value) {
    validatePixel(value);
    if (row >= rows() || column >= columns()) {
        throw std::out_of_range("pixel coordinate is outside the image");
    }
    pixels_[row][column] = value;
}

const BinaryImage::Matrix& BinaryImage::pixels() const noexcept {
    return pixels_;
}

void BinaryImage::validatePixel(int value) {
    if (value != 0 && value != 1) {
        throw std::invalid_argument("a binary-image pixel must be 0 or 1");
    }
}

void BinaryImage::validate() const {
    if (pixels_.empty() || pixels_.front().empty()) {
        throw std::invalid_argument("a binary image cannot be empty");
    }

    const std::size_t expectedColumns = pixels_.front().size();
    for (const auto& row : pixels_) {
        if (row.size() != expectedColumns) {
            throw std::invalid_argument("all image rows must have the same width");
        }
        for (const int value : row) {
            validatePixel(value);
        }
    }
}

bool operator==(const BinaryImage& left,
                const BinaryImage& right) noexcept {
    return left.pixels_ == right.pixels_;
}

}  // namespace morphology
