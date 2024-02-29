#include "ImageMatrix.h"
#include <iostream>


// Default constructor
ImageMatrix::ImageMatrix(): height(0), width(0), data(nullptr){
    
}


// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    // Allocate memory for the matrix and initialize the array with 0's
    data = new double*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new double[width];
        for(int j = 0; j < width; j++) {
            data[i][j] = 0;
        }
    }
}

// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}



// Destructor
ImageMatrix::~ImageMatrix() {
    if(data != nullptr) {
        // Free the memory
        for(int i = 0; i < height; i++){
            delete [] data[i];
        }
        delete [] data;
    }
}

// Parameterized constructor - direct initialization with 2D matrix
ImageMatrix::ImageMatrix(const double** inputMatrix, int imgHeight, int imgWidth){
    // Get height and width
    height = imgHeight;
    width = imgWidth;

    // Allocate memory for the matrix and initialize the matrix from the inputMatrix
    data = new double*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new double[width];
        for(int j = 0; j < width; j++) {
            data[i][j] = inputMatrix[i][j];
        }
    }

}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other): height(other.height), width(other.width) {
    // Allocate memory for the matrix and copy other matrix
    data = new double*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new double[width];
        for(int j = 0; j < width; j++) {
            data[i][j] = other.get_data(i, j);
        }
    }
}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {
    // Check for self assignment
    if(this == &other) {
        return *this;
    }
    // Free old memory
    for(int i = 0; i < height; i++) {
        delete [] data[i];
    }
    delete [] data;
    // Copy
    height = other.height;
    width = other.width;
    data = new double*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new double[width];
        for(int j = 0; j < width; j++) {
            data[i][j] = other.get_data(i, j);
        }
    }
}



// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
    // Check if the size of the matrices are the same
    if(height == other.height && width == other.width) {
        ImageMatrix tempMatrix(height, width);
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                tempMatrix.get_data()[i][j] += (this->get_data(i, j) + other.get_data(i, j));
            }
        }
        return tempMatrix;
    }
}

// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    // Check if the size of the matrices are the same
    if(height == other.height && width == other.width) {
        ImageMatrix tempMatrix(height, width);
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                tempMatrix.get_data()[i][j] += (this->get_data(i, j) - other.get_data(i, j));
            }
        }
        return tempMatrix;
    }
}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}


// Getter function to access the data in the matrix
double** ImageMatrix::get_data() const {
    return data;
}

// Getter function to access the data at the index (i, j)
double ImageMatrix::get_data(int i, int j) const {
    return data[i][j];
}

// Getter function for height
int ImageMatrix::get_height() const {
    return height;
}

// Getter function for width
int ImageMatrix::get_width() const {
    return width;
}
