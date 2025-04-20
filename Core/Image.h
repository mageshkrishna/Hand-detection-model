#ifndef IMAGE_H
#define IMAGE_H

class Image{
    private:
        struct pixel{
            int red;
            int blue;
            int green;
        };

    private:
        int row;
        int col;
        pixel** data;
    
    public:
        Image(int row , int col);
        int getRow();
};

#endif