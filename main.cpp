#include "AVLtree.h"

int main(int argc, char** argv) {

    Tree a, b, c, d, e;
    std::cout << "===================\nA:\n" << a 
        << "===================\nB:\n" << b 
        << "===================\nC:\n" << c
        << "===================\nD:\n" << d;
    e = a|(b&c)/d;
    std::cout << "===================\nE:" << e; 
    return 0;
}

