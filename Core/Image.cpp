#include <Image.h>
#include <iostream>

Image::Image(int row ,int col){
    std::cout << "This is from Image" << std::endl;
    this->row = row;
    this->col = col;
}

int Image::getRow(){
    return this->row;
}