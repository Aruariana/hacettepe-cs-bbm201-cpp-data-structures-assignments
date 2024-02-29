cmake_minimum_required(VERSION 3.26)
project(As1)

set(CMAKE_CXX_STANDARD 11)

add_executable(As1 main.cpp
        ImageSharpening.h
        ImageSharpening.cpp
        ImageProcessor.h
        ImageProcessor.cpp
        ImageMatrix.cpp
        ImageLoader.cpp
        EncodeMessage.h
        EncodeMessage.cpp
        EdgeDetector.h
        EdgeDetector.cpp
        DecodeMessage.h
        DecodeMessage.cpp
        Convolution.cpp)
