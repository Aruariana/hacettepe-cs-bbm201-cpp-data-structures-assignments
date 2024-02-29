#include "Sector.h"

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = std::sqrt(x*x + y*y + z*z);
    sector_code.append(std::to_string(int(std::floor(distance_from_earth))));
    if(x > 0) {
        sector_code.append("R");
    }
    else if(x == 0) {
        sector_code.append("S");
    }
    else {
        sector_code.append("L");
    }

    if(y > 0) {
        sector_code.append("U");
    }
    else if(y == 0) {
        sector_code.append("S");
    }
    else {
        sector_code.append("D");
    }

    if(z > 0) {
        sector_code.append("F");
    }
    else if(z == 0) {
        sector_code.append("S");
    }
    else {
        sector_code.append("B");
    }
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    if (this == &other) {
        return *this;
    }
    x = other.x;
    y = other.y;
    z = other.z;
    distance_from_earth = other.distance_from_earth;
    sector_code = other.sector_code;
    // ???
    left = other.left;
    right = other.right;
    parent = other.parent;
    color = other.color;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}