// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>
#include <bitset>

// Default constructor
DecodeMessage::DecodeMessage() = default;

// Destructor
DecodeMessage::~DecodeMessage() = default;


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string lsb;
    for(int i = 0; i < edgePixels.size(); i++) {
        std::bitset<32> binary(static_cast<unsigned int>(image.get_data(edgePixels[i].first, edgePixels[i].second)));
        lsb += binary.to_string().back();
    }

    // Make the lsb string a multiplier of 7 by adding leading 0's
    int num_of_zeros = (lsb.size() % 7 == 0) ? 0 : (7-(lsb.size()%7));
    for(int i = 0; i < num_of_zeros; i++) {
        lsb.insert(0, "0");
    }

    std::vector<unsigned long> asciiValues;

    for(int i = 0; i < lsb.length(); i+=7) {
        std::string subStr = lsb.substr(i, 7);
        unsigned long asciiVal = std::bitset<7>(subStr).to_ulong();
        if(asciiVal <= 32) {
            asciiVal += 33;
        }
        else if(asciiVal >= 127) {
            asciiVal = 126;
        }
        asciiValues.push_back(asciiVal);
    }

    std::string message;
    for(int i = 0; i < asciiValues.size(); i++) {
        message += static_cast<char>(asciiValues[i]);
    }

    return message;
}

