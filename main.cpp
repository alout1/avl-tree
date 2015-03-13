#include "AVLtree.h"

int main(int argc, char** argv) {

    Tree a(32), b(32), c(32), d(32), e(32); 
    std::cout << "===================\nA:\n" << a 
        << "===================\nB:\n" << b 
        << "===================\nC:\n" << c
        << "===================\nD:\n" << d;
    e = a|(b&c)/d;
    std::cout << "===================\nE:\n" << e ;
    
    std::cout << "==============================================\n";
    
    
    Tree f({0,1,2,3,4,5,6,7,8,9}), g({4,3,2,1,0}); 

    std::cout >> f;
    f.change(g, 5);
    std::cout >> f;

    
    
    return 0;
 
}

