#include "AVLtree.h"

int main(int argc, char** argv) {
/*
    Tree a, b, c, d, e; 
    std::cout << "===================\nA:\n" << a 
        << "===================\nB:\n" << b 
        << "===================\nC:\n" << c
        << "===================\nD:\n" << d;
    e = a|(b&c)/d;
    std::cout << "===================\nE:\n" << e ;
    
    std::cout << "==============================================\n";
    
    Tree f,g; f.insert(100);g.insert(100);
    std::cout<<f<<g;
    std::cout>>f>>g;
    f.merge(g);
    std::cout>>f;
    */
    Tree a({0,1,2,3,4,5,6,7,8,9}), r({1,2,3,4}), b(3); 

    a.subst(r, a.size()-1);
    std::cout >> a;
    a.change(r, 2);
    std::cout >> a;

    
    
    return 0;
 
}

