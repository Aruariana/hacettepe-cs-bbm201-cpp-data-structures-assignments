#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorBST::deleteTree(Sector *&currentSector) {
    if(currentSector != nullptr) {
        deleteTree(currentSector->left);
        deleteTree(currentSector->right);
        delete currentSector;
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.

    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line);
    while(getline(file, line)) {
        vector<int> coordinates;
        istringstream iss(line);
        string item;
        while(getline(iss, item, ',')) {
            coordinates.push_back(std::stoi(item));
        }
        insertSectorByCoordinates(coordinates[0], coordinates[1], coordinates[2]);
    }
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    Sector* sector = new Sector(x, y, z);
    insertSector(root, sector);
}

void SpaceSectorBST::insertSector(Sector *&currentSector, Sector *newSector) {
    if(currentSector == nullptr) {
        currentSector = newSector;
    }
    if(currentSector != newSector) {
        newSector->parent = currentSector;
        // Search for x
        if(newSector->x > currentSector->x) {
            insertSector(currentSector->right, newSector);
        }
        else if(newSector->x < currentSector->x) {
            insertSector(currentSector->left, newSector);
        }
        else {
            // Search for y
            if(newSector->y > currentSector->y) {
                insertSector(currentSector->right, newSector);
            }
            else if(newSector->y < currentSector->y) {
                insertSector(currentSector->left, newSector);
            }
            else {
                // Search for z
                if(newSector->z > currentSector->z) {
                    insertSector(currentSector->right, newSector);
                }
                else {
                    insertSector(currentSector->left, newSector);
                }
            }
        }
    }
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    deleteSectorRecursive(root, sector_code);
}

void SpaceSectorBST::deleteSectorRecursive(Sector *&currentSector, const std::string &sector_code) {
    if(currentSector == nullptr) {
        return;
    }
    else if(sector_code == currentSector->sector_code) {
        deleteNodeSector(currentSector);
    }
    else{
        deleteSectorRecursive(currentSector->left, sector_code);
        deleteSectorRecursive(currentSector->right, sector_code);
    }
}

void SpaceSectorBST::deleteNodeSector(Sector *&sectorToBeDeleted) {
    Sector* temp;

    // Leaf Node
    if(sectorToBeDeleted->left == nullptr && sectorToBeDeleted->right == nullptr) {
        delete sectorToBeDeleted;
        sectorToBeDeleted = nullptr;
    }
    // No left or right child
    else if(sectorToBeDeleted->left == nullptr) {
        temp = sectorToBeDeleted;
        sectorToBeDeleted->right->parent = sectorToBeDeleted->parent;
        sectorToBeDeleted = sectorToBeDeleted->right;
        delete temp;
    }
    else if(sectorToBeDeleted->right == nullptr) {
        temp = sectorToBeDeleted;
        sectorToBeDeleted->left->parent = sectorToBeDeleted->parent;
        sectorToBeDeleted = sectorToBeDeleted->left;
        delete temp;
    }
    else {
        goToLeftmost(sectorToBeDeleted->right, sectorToBeDeleted);
    }
}

void SpaceSectorBST::goToLeftmost(Sector *&currentSector, Sector *switchSector) {
    if(currentSector->left == nullptr) {
        switchSector->x = currentSector->x;
        switchSector->y = currentSector->y;
        switchSector->z = currentSector->z;
        switchSector->sector_code = currentSector->sector_code;
        switchSector->distance_from_earth = currentSector->distance_from_earth;
        switchSector->color = currentSector->color;

        Sector* temp = currentSector;
        if(currentSector->right != nullptr) {
            currentSector->right->parent = currentSector->parent;
        }
        currentSector = currentSector->right;
        delete temp;
    }
    else {
        goToLeftmost(currentSector->left, switchSector);
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorder(root);
}

void SpaceSectorBST::inorder(Sector *currentSector) {
    if(currentSector != nullptr) {
        inorder(currentSector->left);
        cout << currentSector->sector_code << endl;
        inorder(currentSector->right);
    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorder(root);
}

void SpaceSectorBST::preorder(Sector *currentSector) {
    if(currentSector != nullptr) {
        cout << currentSector->sector_code << endl;
        preorder(currentSector->left);
        preorder(currentSector->right);
    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorder(root);
}

void SpaceSectorBST::postorder(Sector *currentSector) {
    if(currentSector != nullptr) {
        postorder(currentSector->left);
        postorder(currentSector->right);
        cout << currentSector->sector_code << endl;
    }
}

Sector *SpaceSectorBST::findSector(Sector *currentSector, const std::string &sector_code) {
    if(currentSector != nullptr) {
        if(currentSector->sector_code == sector_code) {
            return currentSector;
        }
        else {
            Sector* left = findSector(currentSector->left, sector_code);
            if(left != nullptr) {
                return left;
            }
            return findSector(currentSector->right, sector_code);
        }
    }
    return nullptr;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* target = findSector(root, sector_code);
    stack<Sector*> reverseStack;
    while(target != nullptr) {
        reverseStack.push(target);
        target = target->parent;
    }
    while(!reverseStack.empty()) {
        path.push_back(reverseStack.top());
        reverseStack.pop();
    }
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if(path.empty()) {
        cout << "A path to Dr. Elara could not be found." << endl;
    }
    else {
        cout << "The stellar path to Dr. Elara: ";
        string str = path[0]->sector_code;
        for(int i = 1; i < path.size(); i++) {
            str.append("->");
            str.append(path[i]->sector_code);
        }
        cout << str << endl;
    }
}