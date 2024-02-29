#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : kernel(nullptr), kernelHeight(0), kernelWidth(0), stride(0), padding(false) {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad):
    kernel(customKernel), kernelHeight(kh), kernelWidth(kw), stride(stride_val), padding(pad) {
    kernelMatrix = ImageMatrix(const_cast<const double**>(kernel), kernelHeight, kernelWidth);
}

// Destructor
Convolution::~Convolution() = default;

// Copy constructor
Convolution::Convolution(const Convolution &other){
    kernel = other.kernel;
    kernelMatrix = other.kernelMatrix;
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if(this == &other) {
        return *this;
    }
    kernel = other.kernel;
    kernelMatrix = other.kernelMatrix;
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    const ImageMatrix& inputImage = (padding) ? addPadding(input_image) : input_image;
    ImageMatrix outputImg(((inputImage.get_height() - kernelMatrix.get_height()) / stride) + 1,
                          ((inputImage.get_width() - kernelMatrix.get_width()) / stride) + 1);
    for(int i = 0; i < inputImage.get_height(); i++) {
        for(int j = 0; j < inputImage.get_width(); j++) {
            if(i*stride + kernelHeight <= inputImage.get_height() && j*stride + kernelWidth <= inputImage.get_width()) {
                double sum = 0;
                for(int c = 0; c < kernelHeight; c++) {
                    for(int d = 0; d < kernelWidth; d++) {
                        sum += kernelMatrix.get_data(c, d)*inputImage.get_data(i*stride+c, j*stride+d);
                    }
                }
                outputImg.get_data()[i][j] = sum;
            }
        }
    }
    return outputImg;
}

ImageMatrix Convolution::addPadding(const ImageMatrix &input_image) const {
    ImageMatrix padded_img(input_image.get_height() + 2, input_image.get_width() + 2);
    for(int i = 1; i < padded_img.get_height() - 1; i++) {
        for(int j = 1; j < padded_img.get_width() - 1; j++) {
            padded_img.get_data()[i][j] = input_image.get_data(i-1, j-1);
        }
    }
    return padded_img;
}

ImageMatrix &Convolution::getKernelMatrix() {
    return kernelMatrix;
}
