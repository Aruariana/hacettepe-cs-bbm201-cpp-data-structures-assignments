#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
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

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorLLRBT::deleteTree(Sector *&currentSector) {
    if(currentSector != nullptr) {
        deleteTree(currentSector->left);
        deleteTree(currentSector->right);
        delete currentSector;
    }
}

bool SpaceSectorLLRBT::isRed(Sector *cSector) {
    if(cSector == nullptr) {
        return false;
    }
    return cSector->color;
}

Sector *SpaceSectorLLRBT::rotateLeft(Sector *cSector) {
    Sector* rightSector = cSector->right;
    cSector->right = rightSector->left;
    if(cSector->right != nullptr) {
        cSector->right->parent = cSector;
    }
    rightSector->left = cSector;
    cSector->parent = rightSector;
    rightSector->color = cSector->color;
    cSector->color = RED;
    return rightSector;
}

Sector *SpaceSectorLLRBT::rotateRight(Sector *cSector) {
    Sector* leftSector = cSector->left;
    cSector->left = leftSector->right;
    if(cSector->left != nullptr) {
        cSector->left->parent = cSector;
    }
    leftSector->right = cSector;
    cSector->parent = leftSector;
    leftSector->color = cSector->color;
    cSector->color = RED;
    return leftSector;
}

void SpaceSectorLLRBT::flipColors(Sector *cSector) {
    cSector->color = RED;
    cSector->left->color = BLACK;
    cSector->right->color = BLACK;
}

Sector *SpaceSectorLLRBT::insertSector(Sector *cSector, int x, int y, int z) {
    if(cSector == nullptr) {
        return new Sector(x, y, z);
    }

    if(x > cSector->x) {
        cSector->right = insertSector(cSector->right, x, y, z);
        cSector->right->parent = cSector;
    }
    else if(x < cSector->x) {
        cSector->left = insertSector(cSector->left, x, y, z);
        cSector->left->parent = cSector;
    }
    else {
        // Search for y
        if(y > cSector->y) {
            cSector->right = insertSector(cSector->right, x, y, z);
            cSector->right->parent = cSector;
        }
        else if(y < cSector->y) {
            cSector->left = insertSector(cSector->left, x, y, z);
            cSector->left->parent = cSector;
        }
        else {
            // Search for z
            if(z > cSector->z) {
                cSector->right = insertSector(cSector->right, x, y, z);
                cSector->right->parent = cSector;
            }
            else {
                cSector->left = insertSector(cSector->left, x, y, z);
                cSector->left->parent = cSector;
            }
        }
    }

    // Case 1: Right child red && Left child black
    if(isRed(cSector->right) && !isRed(cSector->left)) {
        cSector = rotateLeft(cSector);
    }

    // Case 2: Left child and left grandchild is red
    if(isRed(cSector->left) && isRed(cSector->left->left)) {
        cSector = rotateRight(cSector);
    }

    // Case 3: Right and left child is red
    if(isRed(cSector->right) && isRed(cSector->left)) {
        flipColors(cSector);
    }
    return cSector;
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    root = insertSector(root, x, y, z);
    root->color = BLACK;
    root->parent = nullptr;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorder(root);
}

void SpaceSectorLLRBT::inorder(Sector *currentSector) {
    if(currentSector != nullptr) {
        inorder(currentSector->left);
        cout << (currentSector->color ? "RED" : "BLACK") << " sector: " << currentSector->sector_code << endl;
        inorder(currentSector->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorder(root);
}

void SpaceSectorLLRBT::preorder(Sector *currentSector) {
    if(currentSector != nullptr) {
        cout << (currentSector->color ? "RED" : "BLACK") << " sector: " << currentSector->sector_code << endl;
        preorder(currentSector->left);
        preorder(currentSector->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorder(root);
}

void SpaceSectorLLRBT::postorder(Sector *currentSector) {
    if(currentSector != nullptr) {
        postorder(currentSector->left);
        postorder(currentSector->right);
        cout << (currentSector->color ? "RED" : "BLACK") << " sector: " << currentSector->sector_code << endl;
    }
}

Sector *SpaceSectorLLRBT::findSector(Sector *currentSector, const std::string &sector_code) {
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

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    std::vector<Sector*> pathFromEarth;
    std::vector<Sector*> pathToTarget;

    Sector* earth = findSector(root, "0SSS");
    Sector* target = findSector(root, sector_code);

    while(earth != nullptr) {
        pathFromEarth.push_back(earth);
        earth = earth->parent;
    }
    stack<Sector*> reverser;
    while(target != nullptr) {
        reverser.push(target);
        target = target->parent;
    }
    while(!reverser.empty()) {
        pathToTarget.push_back(reverser.top());
        reverser.pop();
    }

    if(!pathFromEarth.empty() && !pathToTarget.empty()) {
        int idx;
        for(int i = 0; i < pathFromEarth.size(); i++) {
            path.push_back(pathFromEarth[i]);
            bool flag = false;
            for(idx = 0; idx < pathToTarget.size(); idx++) {
                if(path[i]->sector_code == pathToTarget[idx]->sector_code) {
                    flag = true;
                    break;
                }
            }
            if(flag) {
                break;
            }
        }
        idx++;
        for(; idx < pathToTarget.size(); idx++) {
            path.push_back(pathToTarget[idx]);
        }
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
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