
// Challenge 7 from Modern C
// determines shortest possible paths between two locations in Fremont County, Wyoming
#include <stdio.h>
#include <stdlib.h>

// SIZE_MAX for denoting no direct connection
const size_t X = 0 - 1;

// Enumerable for towns in distance matrix
enum FremontCounty {
    Lander,
    FortWashakie,
    Hudson,
    Riverton,
    Shoshoni,
    Dubois,
    JefferyCity,
    AtlanticCity,
    townCount
};

// two dimensional array representing distance between towns
const size_t distanceMap[8][8] =
    {{0,  10, 8,  X,  X,  X,  60, 30},
     {10, 0 , X,  20, X,  70, X,  X},
     {8,  X,  0,  12, X,  X,  X,  X},
     {X,  20, 12, 0,  25, 80, 70, X},
     {X,  X,  X,  25, 0,  90, 75, X},
     {X,  70, X,  80, 90, 0,  X,  X},
     {60, X,  X,  70, X,  X,  0,  90},
     {30, X,  X,  X,  X,  X,  90, 0} 
    };

// function declarations
size_t isDirect(size_t townA, size_t townB);
void setShortest(size_t distance);
void allPaths(size_t townA, size_t townB, size_t distance, size_t depth);
size_t shortestPath(size_t townA, size_t townB);

// shortest distance global
size_t shortestDistance = X;

int main(void) {
    printf("the shortest distance between Lander and Fort Washakie is %zu\n", shortestPath(Lander, FortWashakie));
    printf("the shortest distance between Lander and Atlantic City is %zu\n", shortestPath(Lander, AtlanticCity));
    printf("the shortest distance between Lander and Riverton is %zu\n", shortestPath(Lander, Riverton));
    printf("the shortest distance between Atlantic City and Shoshoni is %zu\n", shortestPath(AtlanticCity, Shoshoni));
    return 0;
}

// top level function finds shortest path
size_t shortestPath(size_t townA, size_t townB) {
    shortestDistance = X;
    allPaths(townA, townB, 0, 0);
    return shortestDistance;
}

// recurses through matrix to determine all possible paths
void allPaths(size_t townA, size_t townB, size_t distance, size_t depth) {
    // if valid route is found, check to see if shortest
    if (isDirect(townA, townB)) {
        distance += distanceMap[townA][townB];
        setShortest(distance);
        return;
    }
    // if no route is found by depth of length of Fremont Count
    // break infinite recursion by return
    if (depth > townCount) {
        return;
    }
    // check all other possible paths recursively
    for (size_t i = 0; i < townCount; ++i) {
        if (distanceMap[townA][i] < X && distanceMap[townA][i] > 0) {
            allPaths(i, townB, distance + distanceMap[townA][i], depth + 1);
        }
    }
}

// returns true if there is a direct path between two towns
size_t isDirect(size_t townA, size_t townB) {
    return (distanceMap[townA][townB] < X);
}

// updates shortest distance if new distance is shorter
void setShortest(size_t distance) {
    if (distance < shortestDistance) {
        shortestDistance = distance;
    }
    return;
}

