#include "Image.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>

namespace imaging {

// Default constructor
Image::Image() : width_(0), height_(0), pixels_() {}

// Constructor with dimensions
Image::Image(unsigned int width, unsigned int height, const Pixel& backgroundColor)
    : width_(width), height_(height), pixels_(width * height, backgroundColor) {}

// Copy constructor
Image::Image(const Image& other)
    : width_(other.width_), height_(other.height_), pixels_(other.pixels_) {}

// Move constructor
Image::Image(Image&& other) noexcept
    : width_(other.width_), height_(other.height_), pixels_(std::move(other.pixels_)) {
    other.width_ = 0;
    other.height_ = 0;
}

// Destructor
Image::~Image() = default;

// Copy assignment operator
Image& Image::operator=(const Image& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        pixels_ = other.pixels_;
    }
    return *this;
}

// Move assignment operator
Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        pixels_ = std::move(other.pixels_);
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

// Get width
unsigned int Image::getWidth() const {
    return width_;
}

// Get height
unsigned int Image::getHeight() const {
    return height_;
}

// Get total pixel count
unsigned int Image::getPixelCount() const {
    return width_ * height_;
}

// Check if image is empty
bool Image::isEmpty() const {
    return width_ == 0 || height_ == 0;
}

// Get pixel at coordinates
Pixel& Image::getPixel(unsigned int x, unsigned int y) {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return pixels_[y * width_ + x];
}

// Get pixel at coordinates (const version)
const Pixel& Image::getPixel(unsigned int x, unsigned int y) const {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return pixels_[y * width_ + x];
}

// Set pixel at coordinates
void Image::setPixel(unsigned int x, unsigned int y, const Pixel& pixel) {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    pixels_[y * width_ + x] = pixel;
}

// Get raw pointer to pixel data
Pixel* Image::getData() {
    return pixels_.empty() ? nullptr : &pixels_[0];
}

// Get raw pointer to pixel data (const version)
const Pixel* Image::getData() const {
    return pixels_.empty() ? nullptr : &pixels_[0];
}

// Resize image
void Image::resize(unsigned int newWidth, unsigned int newHeight, bool preserveContent) {
    if (newWidth == width_ && newHeight == height_) {
        return;  // No change needed
    }

    if (preserveContent && !isEmpty()) {
        // Create a new pixel buffer with the new size
        std::vector<Pixel> newPixels(newWidth * newHeight);
        
        // Copy existing pixels to the new buffer
        unsigned int copyWidth = std::min(width_, newWidth);
        unsigned int copyHeight = std::min(height_, newHeight);
        
        for (unsigned int y = 0; y < copyHeight; ++y) {
            for (unsigned int x = 0; x < copyWidth; ++x) {
                newPixels[y * newWidth + x] = pixels_[y * width_ + x];
            }
        }
        
        // Swap the old and new buffers
        pixels_.swap(newPixels);
    } else {
        // Just resize and initialize to black
        pixels_.resize(newWidth * newHeight);
    }
    
    width_ = newWidth;
    height_ = newHeight;
}

// Clear image to specified color
void Image::clear(const Pixel& color) {
    std::fill(pixels_.begin(), pixels_.end(), color);
}

// Create deep copy of the image
Image Image::clone() const {
    return Image(*this);
}

// Load image from BMP file
bool Image::loadFromBMP(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Read BMP header
    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header.signature), 2);
    file.read(reinterpret_cast<char*>(&header.fileSize), 4);
    file.read(reinterpret_cast<char*>(&header.reserved), 4);
    file.read(reinterpret_cast<char*>(&header.dataOffset), 4);
    file.read(reinterpret_cast<char*>(&header.headerSize), 4);
    file.read(reinterpret_cast<char*>(&header.width), 4);
    file.read(reinterpret_cast<char*>(&header.height), 4);
    file.read(reinterpret_cast<char*>(&header.planes), 2);
    file.read(reinterpret_cast<char*>(&header.bitsPerPixel), 2);
    file.read(reinterpret_cast<char*>(&header.compression), 4);
    file.read(reinterpret_cast<char*>(&header.imageSize), 4);
    file.read(reinterpret_cast<char*>(&header.xPixelsPerMeter), 4);
    file.read(reinterpret_cast<char*>(&header.yPixelsPerMeter), 4);
    file.read(reinterpret_cast<char*>(&header.colorsUsed), 4);
    file.read(reinterpret_cast<char*>(&header.importantColors), 4);

    // Validate BMP format
    if (header.signature != 0x4D42) { // 'BM' in little endian
        std::cerr << "Error: Not a valid BMP file" << std::endl;
        return false;
    }

    if (header.bitsPerPixel != 24) {
        std::cerr << "Error: Only 24-bit BMP files are supported" << std::endl;
        return false;
    }

    if (header.compression != 0) {
        std::cerr << "Error: Compressed BMP files are not supported" << std::endl;
        return false;
    }

    // Calculate row padding (BMP rows are padded to multiple of 4 bytes)
    int paddingSize = (4 - (header.width * 3) % 4) % 4;
    
    // Resize the image
    width_ = static_cast<unsigned int>(header.width);
    height_ = static_cast<unsigned int>(std::abs(header.height));
    pixels_.resize(width_ * height_);

    // Seek to pixel data
    file.seekg(header.dataOffset, std::ios::beg);

    // Read pixel data
    // BMP stores images bottom-to-top by default
    bool bottomUp = header.height > 0;
    
    std::vector<uint8_t> rowBuffer(width_ * 3 + paddingSize);
    for (unsigned int y = 0; y < height_; ++y) {
        // Calculate row index based on the BMP orientation
        unsigned int rowIndex = bottomUp ? (height_ - 1 - y) : y;
        
        // Read row data
        file.read(reinterpret_cast<char*>(rowBuffer.data()), rowBuffer.size());
        
        // Process row data (BGR to RGB)
        for (unsigned int x = 0; x < width_; ++x) {
            unsigned int pixelIndex = rowIndex * width_ + x;
            unsigned int bufferIndex = x * 3;
            
            // BMP stores colors as BGR, swap to RGB
            pixels_[pixelIndex].b = rowBuffer[bufferIndex];
            pixels_[pixelIndex].g = rowBuffer[bufferIndex + 1];
            pixels_[pixelIndex].r = rowBuffer[bufferIndex + 2];
        }
    }

    return !file.fail();
}

// Save image to BMP file
bool Image::saveToBMP(const std::string& filename) const {
    if (isEmpty()) {
        std::cerr << "Error: Cannot save empty image" << std::endl;
        return false;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return false;
    }

    // Calculate row padding (BMP rows are padded to multiple of 4 bytes)
    int paddingSize = (4 - (width_ * 3) % 4) % 4;
    
    // Calculate file size
    uint32_t pixelDataSize = (width_ * 3 + paddingSize) * height_;
    uint32_t fileSize = 54 + pixelDataSize; // 54 bytes for header
    
    // Prepare BMP header
    BMPHeader header;
    header.signature = 0x4D42; // 'BM' in little endian
    header.fileSize = fileSize;
    header.reserved = 0;
    header.dataOffset = 54;
    header.headerSize = 40;
    header.width = static_cast<int32_t>(width_);
    header.height = static_cast<int32_t>(height_); // Positive for bottom-up
    header.planes = 1;
    header.bitsPerPixel = 24;
    header.compression = 0;
    header.imageSize = pixelDataSize;
    header.xPixelsPerMeter = 2835; // 72 DPI
    header.yPixelsPerMeter = 2835; // 72 DPI
    header.colorsUsed = 0;
    header.importantColors = 0;
    
    // Write BMP header
    file.write(reinterpret_cast<const char*>(&header.signature), 2);
    file.write(reinterpret_cast<const char*>(&header.fileSize), 4);
    file.write(reinterpret_cast<const char*>(&header.reserved), 4);
    file.write(reinterpret_cast<const char*>(&header.dataOffset), 4);
    file.write(reinterpret_cast<const char*>(&header.headerSize), 4);
    file.write(reinterpret_cast<const char*>(&header.width), 4);
    file.write(reinterpret_cast<const char*>(&header.height), 4);
    file.write(reinterpret_cast<const char*>(&header.planes), 2);
    file.write(reinterpret_cast<const char*>(&header.bitsPerPixel), 2);
    file.write(reinterpret_cast<const char*>(&header.compression), 4);
    file.write(reinterpret_cast<const char*>(&header.imageSize), 4);
    file.write(reinterpret_cast<const char*>(&header.xPixelsPerMeter), 4);
    file.write(reinterpret_cast<const char*>(&header.yPixelsPerMeter), 4);
    file.write(reinterpret_cast<const char*>(&header.colorsUsed), 4);
    file.write(reinterpret_cast<const char*>(&header.importantColors), 4);
    
    // Prepare row buffer with padding
    std::vector<uint8_t> rowBuffer(width_ * 3 + paddingSize, 0);
    
    // Write pixel data (bottom-up)
    for (unsigned int y = 0; y < height_; ++y) {
        // BMP stores rows bottom-to-top
        unsigned int rowIndex = height_ - 1 - y;
        
        // Fill row buffer (RGB to BGR)
        for (unsigned int x = 0; x < width_; ++x) {
            const Pixel& pixel = pixels_[rowIndex * width_ + x];
            unsigned int bufferIndex = x * 3;
            
            // BMP stores colors as BGR
            rowBuffer[bufferIndex] = pixel.b;
            rowBuffer[bufferIndex + 1] = pixel.g;
            rowBuffer[bufferIndex + 2] = pixel.r;
        }
        
        // Write row data
        file.write(reinterpret_cast<const char*>(rowBuffer.data()), rowBuffer.size());
    }
    
    return !file.fail();
}

} // namespace imaging