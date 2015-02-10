#include "AVLtree.h"

int main(int argc, char** argv) {

    Tree a, b, c, d, e;
    a.add(56);
    //a.remove(56);
    std::cout << "===================\nA:" << a 
        << "===================\nB:" << b 
        << "===================\nC:" << c
        << "===================\nD:" << d;
    e = a|(b&c)/d;
    std::cout << "===================\nE:" << e;
    return 0;
}

