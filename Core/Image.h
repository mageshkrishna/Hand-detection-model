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
        Image(int , int);
        int getRow();
};

