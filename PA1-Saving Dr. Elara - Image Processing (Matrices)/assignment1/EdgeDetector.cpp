// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    double* gx[3] = {new double[3]{-1, 0, 1},
                     new double[3]{-2, 0, 2},
                     new double[3]{-1, 0, 1}};
    double* gy[3] = {new double[3]{-1, -2, -1},
                     new double[3]{0, 0, 0},
                     new double[3]{1, 2, 1}};

    convolutionGx = Convolution(gx, 3, 3, 1, true);
    convolutionGy = Convolution(gy, 3, 3, 1, true);
    for(int i = 0; i < 3; i++) {
        delete [] gx[i];
        delete [] gy[i];
    }

}

// Destructor
EdgeDetector::~EdgeDetector() = default;

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    std::vector<std::pair<int, int>> edges;

    ImageMatrix Ix = convolutionGx.convolve(input_image);
    ImageMatrix Iy = convolutionGy.convolve(input_image);
    double sum = 0;

    ImageMatrix gradient(Ix.get_height(), Iy.get_width());
    for(int i = 0; i < Ix.get_height(); i++) {
        for(int j = 0; j < Ix.get_width(); j++) {
            gradient.get_data()[i][j] = std::sqrt((Ix.get_data(i ,j)*Ix.get_data(i, j))+(Iy.get_data(i, j)*Iy.get_data(i, j)));
            sum += gradient.get_data(i, j);
        }
    }

    double threshold = sum/(gradient.get_height()*gradient.get_width());

    for(int i = 0; i < gradient.get_height(); i++) {
        for(int j = 0; j < gradient.get_width(); j++) {
            if(gradient.get_data(i, j) > threshold) {
                edges.push_back(std::make_pair(i, j));
            }
        }
    }
    return edges;
}

