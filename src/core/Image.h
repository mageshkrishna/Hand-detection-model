#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include "Pixel.h"

namespace imaging {

/**
 * @class Image
 * @brief Represents a bitmap image with RGB pixel data
 */
class Image {
public:
    /**
     * @brief Default constructor, creates an empty image
     */
    Image();

    /**
     * @brief Constructor that creates an image of specified dimensions
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param backgroundColor Optional background color (defaults to black)
     */
    Image(unsigned int width, unsigned int height, const Pixel& backgroundColor = Pixel());

    /**
     * @brief Copy constructor
     * @param other Image to copy from
     */
    Image(const Image& other);

    /**
     * @brief Move constructor
     * @param other Image to move from
     */
    Image(Image&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Image();

    /**
     * @brief Copy assignment operator
     * @param other Image to copy from
     * @return Reference to this image
     */
    Image& operator=(const Image& other);

    /**
     * @brief Move assignment operator
     * @param other Image to move from
     * @return Reference to this image
     */
    Image& operator=(Image&& other) noexcept;

    /**
     * @brief Get image width
     * @return Width in pixels
     */
    unsigned int getWidth() const;

    /**
     * @brief Get image height
     * @return Height in pixels
     */
    unsigned int getHeight() const;

    /**
     * @brief Get total number of pixels
     * @return Number of pixels (width * height)
     */
    unsigned int getPixelCount() const;

    /**
     * @brief Check if image is empty (has no pixels)
     * @return true if image is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * @brief Get pixel at specified coordinates
     * @param x X-coordinate
     * @param y Y-coordinate
     * @return Reference to pixel at specified location
     * @throws std::out_of_range if coordinates are out of bounds
     */
    Pixel& getPixel(unsigned int x, unsigned int y);

    /**
     * @brief Get pixel at specified coordinates (const version)
     * @param x X-coordinate
     * @param y Y-coordinate
     * @return Const reference to pixel at specified location
     * @throws std::out_of_range if coordinates are out of bounds
     */
    const Pixel& getPixel(unsigned int x, unsigned int y) const;

    /**
     * @brief Set pixel at specified coordinates
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param pixel New pixel value
     * @throws std::out_of_range if coordinates are out of bounds
     */
    void setPixel(unsigned int x, unsigned int y, const Pixel& pixel);

    /**
     * @brief Get raw pointer to pixel data
     * @return Pointer to first pixel
     */
    Pixel* getData();

    /**
     * @brief Get raw pointer to pixel data (const version)
     * @return Const pointer to first pixel
     */
    const Pixel* getData() const;

    /**
     * @brief Resize the image
     * @param newWidth New width in pixels
     * @param newHeight New height in pixels
     * @param preserveContent If true, preserve existing content when possible
     */
    void resize(unsigned int newWidth, unsigned int newHeight, bool preserveContent = true);

    /**
     * @brief Clear the image to a specified color
     * @param color Color to clear to (defaults to black)
     */
    void clear(const Pixel& color = Pixel());

    /**
     * @brief Create a deep copy of the image
     * @return New image with identical content
     */
    Image clone() const;

    /**
     * @brief Structure to hold BMP file header information
     */
    struct BMPHeader {
        uint16_t signature;        ///< File signature, must be 'BM'
        uint32_t fileSize;         ///< Total file size in bytes
        uint32_t reserved;         ///< Reserved field, must be 0
        uint32_t dataOffset;       ///< Offset to pixel data
        uint32_t headerSize;       ///< Header size in bytes
        int32_t width;             ///< Image width in pixels
        int32_t height;            ///< Image height in pixels
        uint16_t planes;           ///< Number of color planes, must be 1
        uint16_t bitsPerPixel;     ///< Bits per pixel (24 for RGB)
        uint32_t compression;      ///< Compression method (0 for no compression)
        uint32_t imageSize;        ///< Image size in bytes
        int32_t xPixelsPerMeter;   ///< Horizontal resolution
        int32_t yPixelsPerMeter;   ///< Vertical resolution
        uint32_t colorsUsed;       ///< Number of colors in the palette
        uint32_t importantColors;  ///< Number of important colors
    };

    /**
     * @brief Load image from BMP file
     * @param filename Path to BMP file
     * @return true if loading was successful, false otherwise
     */
    bool loadFromBMP(const std::string& filename);

    /**
     * @brief Save image to BMP file
     * @param filename Path to output BMP file
     * @return true if saving was successful, false otherwise
     */
    bool saveToBMP(const std::string& filename) const;

private:
    unsigned int width_;           ///< Image width in pixels
    unsigned int height_;          ///< Image height in pixels
    std::vector<Pixel> pixels_;    ///< Pixel data storage
};

} // namespace imaging

#endif // IMAGE_H