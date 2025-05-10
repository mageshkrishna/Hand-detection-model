#ifndef PIXEL_H
#define PIXEL_H

#include <cstdint>

namespace imaging {

/**
 * @struct Pixel
 * @brief Represents an RGB pixel in an image
 */
struct Pixel {
    uint8_t r;  ///< Red channel (0-255)
    uint8_t g;  ///< Green channel (0-255)
    uint8_t b;  ///< Blue channel (0-255)

    /**
     * @brief Default constructor, creates a black pixel
     */
    Pixel() : r(0), g(0), b(0) {}

    /**
     * @brief Constructor with RGB values
     * @param red Red channel value (0-255)
     * @param green Green channel value (0-255)
     * @param blue Blue channel value (0-255)
     */
    Pixel(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

    /**
     * @brief Equality operator
     * @param other Pixel to compare with
     * @return true if pixels are identical, false otherwise
     */
    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    /**
     * @brief Inequality operator
     * @param other Pixel to compare with
     * @return true if pixels are different, false otherwise
     */
    bool operator!=(const Pixel& other) const {
        return !(*this == other);
    }

    /**
     * @brief Calculate luminance (grayscale value)
     * @return Luminance value between 0-255
     * 
     * Uses standard RGB to luminance formula: Y = 0.299*R + 0.587*G + 0.114*B
     */
    uint8_t getLuminance() const {
        return static_cast<uint8_t>(0.299f * r + 0.587f * g + 0.114f * b);
    }

    /**
     * @brief Set all channels to the same value (grayscale)
     * @param value Value to set (0-255)
     */
    void setGray(uint8_t value) {
        r = g = b = value;
    }

    /**
     * @brief Addition operator for pixels (clamped to 255)
     * @param other Pixel to add
     * @return New pixel with summed values (clamped to 255)
     */
    Pixel operator+(const Pixel& other) const {
        return Pixel(
            static_cast<uint8_t>(std::min(255, static_cast<int>(r) + other.r)),
            static_cast<uint8_t>(std::min(255, static_cast<int>(g) + other.g)),
            static_cast<uint8_t>(std::min(255, static_cast<int>(b) + other.b))
        );
    }

    /**
     * @brief Multiply pixel by a scalar factor (clamped to 255)
     * @param factor Scalar multiplication factor
     * @return New pixel with scaled values (clamped to 255)
     */
    Pixel operator*(float factor) const {
        return Pixel(
            static_cast<uint8_t>(std::min(255.0f, r * factor)),
            static_cast<uint8_t>(std::min(255.0f, g * factor)),
            static_cast<uint8_t>(std::min(255.0f, b * factor))
        );
    }
};

} // namespace imaging

#endif // PIXEL_H