#include <iostream>
#include "core/Image.h"

int main() {
    std::cout << "The very first program for the Hand-detection model project" << std::endl;
    
    // Create an imaging namespace alias for convenience
    using namespace imaging;
    
    // Create an image
    Image myImage;
    
    // Load an image from file
    if (myImage.loadFromBMP("/home/mageshkrishna/code-files/Hand Detection model from scrath using C++/Hand-detection-model/sample.bmp")) {
        std::cout << "Successfully loaded image with dimensions: " 
                  << myImage.getWidth() << "x" << myImage.getHeight() << std::endl;
        
        // Example: Process the image (inverting colors)
        for (unsigned int y = 0; y < myImage.getHeight(); ++y) {
            for (unsigned int x = 0; x < myImage.getWidth(); ++x) {
                Pixel& pixel = myImage.getPixel(x, y);
                // Invert the pixel colors
                pixel.r = 255 - pixel.r;
                pixel.g = 255 - pixel.g;
                pixel.b = 255 - pixel.b;
            }
        }
        
        // Save the processed image
        if (myImage.saveToBMP("output.bmp")) {
            std::cout << "Successfully saved processed image" << std::endl;
        } else {
            std::cerr << "Failed to save processed image" << std::endl;
        }
    } else {
        std::cerr << "Failed to load image" << std::endl;
    }
    
    return 0;
}