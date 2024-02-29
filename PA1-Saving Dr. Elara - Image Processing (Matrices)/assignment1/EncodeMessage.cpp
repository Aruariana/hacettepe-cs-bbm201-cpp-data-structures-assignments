#include "EncodeMessage.h"
#include <cmath>
#include <bitset>



// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    // Character transformation
    std::string cMessage = message;
    for(int i = 0; i < cMessage.size(); i++) {
        if(checkPrime(i)) {
            int ascii = cMessage[i] + fibonacci(i);
            if(ascii <= 32) {
                ascii += 33;
            }
            else if(ascii >= 127) {
                ascii = 126;
            }
            cMessage[i] = ascii;
        }
    }
    // Right circular shifting
    int halfOfSize = static_cast<int>(cMessage.size())/2;
    std::string shiftedMessage = cMessage.substr(cMessage.size()-halfOfSize, halfOfSize) +
                                 cMessage.substr(0, cMessage.size()-halfOfSize);

    //Message embedding
    ImageMatrix cImg = img;
    std::vector<int> asciiValues;
    std::string lsb;
    for(int i = 0; i < shiftedMessage.size(); i++) {
        asciiValues.push_back(static_cast<int>(shiftedMessage[i]));
    }

    for(int i = 0; i < asciiValues.size(); i++) {
        lsb += (std::bitset<7>(asciiValues[i])).to_string();
    }

    for(int i = 0; (i < positions.size() && i < lsb.size()); i++) { // .......
        std::bitset<32> binary(static_cast<unsigned int>(cImg.get_data(positions[i].first, positions[i].second)));
        std::string binaryStr = binary.to_string();
        binaryStr[binaryStr.size()-1] = lsb[i];
        std::bitset<32> encodedBinary(binaryStr);
        cImg.get_data()[positions[i].first][positions[i].second] = encodedBinary.to_ulong();
    }

    return cImg;

}

int EncodeMessage::fibonacci(int index) {
    int i1 = 0;
    int i2 = 1;
    if(index == 0) {
        return 0;
    }
    else if(index == 1) {
        return 1;
    }
    else {
        for(int i = 1; i < index; i++) {
            int temp = i1 + i2;
            i1 = i2;
            i2 = temp;
        }
        return i2;
    }
}

bool EncodeMessage::checkPrime(int num) {
    if(num == 0 || num == 1) {
        return false;
    }
    else if(num == 2) {
        return true;
    }
    else {
        for(int i = 2; i < num; i++) {
            if(num % i == 0) {
                return false;
            }
        }
        return true;
    }
}
