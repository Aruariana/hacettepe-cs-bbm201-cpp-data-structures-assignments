#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() = default;

ImageProcessor::~ImageProcessor() = default;


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening sharpener;
    EdgeDetector edgeDetector;
    DecodeMessage decoder;

    ImageMatrix sharpenedImg = sharpener.sharpen(img, 2);
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpenedImg);
    std::string message = decoder.decodeFromImage(sharpenedImg, edges);
    return message;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening sharpener;
    EdgeDetector edgeDetector;
    EncodeMessage encoder;

    ImageMatrix sharpenedImg = sharpener.sharpen(img, 2);
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpenedImg);

    return encoder.encodeMessageToImage(img, message, edges);
}
