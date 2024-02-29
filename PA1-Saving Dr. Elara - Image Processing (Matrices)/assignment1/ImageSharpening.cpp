#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    double* kernel[3] = {
            new double[3]{1.0, 1.0, 1.0},
            new double[3]{1.0, 1.0, 1.0},
            new double[3]{1.0, 1.0, 1.0}
    };
    convolutionForBlur = Convolution(kernel, 3, 3, 1, true);
    convolutionForBlur.getKernelMatrix() = convolutionForBlur.getKernelMatrix()*(1.0/9.0);
    for(int i = 0; i < 3; i++) {
        delete [] kernel[i];
    }
}

ImageSharpening::~ImageSharpening()= default;

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    ImageMatrix noisyImg = convolutionForBlur.convolve(input_image);
    ImageMatrix sharpImg = input_image + (input_image-noisyImg)*k;
    // Check if any pixel is greater than 255, if it is then cap them to 255
    for(int i = 0; i < sharpImg.get_height(); i++) {
        for(int j = 0; j < sharpImg.get_width(); j++) {
            if(sharpImg.get_data(i, j) > 255) {
                sharpImg.get_data()[i][j] = 255;
            }
            else if(sharpImg.get_data(i, j) < 0) {
                sharpImg.get_data()[i][j] = 0;
            }
        }
    }
    return sharpImg;
}
