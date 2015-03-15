#include "AVLtree.h"

int main(int argc, char** argv) {

    Tree a(32), b(32), c(32), d(32), e(0); 
    std::cout << "===================\nA:\n" << a 
        << "===================\nB:\n" << b 
        << "===================\nC:\n" << c
        << "===================\nD:\n" << d;
    e = a|(b&c)/d;
    std::cout << "===================\nE:\n" << e ;
    
        
    e.switchModeToSequence();
    std::cout << "==============================================\n" >> e <<
                "change:\n";    
    Tree  n({4,3,2,1});
    std::cout >> n;
    e.change(n, 10);
    std::cout << "==> " >> e;
  
  
    std::cout << "==============================================\n" <<
                "merge:\n";    
    Tree f({0,0,0,1,1,1,97,98,99}); 
    std::cout  >> f;
    e.merge(f);
    std::cout <<"==> ">> e;

    std::cout << "==============================================\n" <<
                "subst:\n";    
    Tree h({1,2,3,4,5,6,7,8,9});
    std::cout  >> h;
    e.subst(h, 6);
    std::cout << "==> " >> e;
    
    
   return 0;
}

