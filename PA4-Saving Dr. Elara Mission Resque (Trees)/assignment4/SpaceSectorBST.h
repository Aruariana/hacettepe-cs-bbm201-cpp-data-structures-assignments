#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <stack>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    // Helper functions
    void insertSector(Sector*& currentSector, Sector* newSector);
    void inorder(Sector* currentSector);
    void preorder(Sector* currentSector);
    void postorder(Sector* currentSector);
    void deleteTree(Sector *&currentSector);
    void deleteSectorRecursive(Sector *&currentSector, const std::string& sector_code);
    void deleteNodeSector(Sector *&sectorToBeDeleted);
    void goToLeftmost(Sector *&currentSector, Sector *switchSector);
    Sector* findSector(Sector* currentSector, const std::string& sector_code);
};

#endif // SPACESECTORBST_H
