#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <stack>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    // Helper Functions
    bool isRed(Sector* cSector);
    Sector* rotateLeft(Sector* cSector);
    Sector* rotateRight(Sector* cSector);
    void flipColors(Sector* cSector);
    Sector* insertSector(Sector* cSector, int x, int y, int z);

    void deleteTree(Sector *&currentSector);
    void inorder(Sector* currentSector);
    void preorder(Sector* currentSector);
    void postorder(Sector* currentSector);

    Sector* findSector(Sector* currentSector, const std::string& sector_code);
};

#endif // SPACESECTORLLRBT_H
