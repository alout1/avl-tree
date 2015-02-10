#ifndef AVLTREE_H
#define	AVLTREE_H

#include <iostream>
#include <queue>
//#include <vector>
#include <chrono>
#include <random>

class Node
{
public:
    int key;
    int height;
    Node* left;
    Node* right;
    Node(int k)
    {
        key = k;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
    ~Node()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }
    friend std::ostream& operator<<(std::ostream& out, Node& n)
    {
        out << n.key;
        return out;
    }
};

class Tree
{
public:
    Tree();
    ~Tree();
    
    void add(int key);
    void remove(int key);
    bool exist(int key);
    
    friend std::ostream& operator<<(std::ostream& out, Tree& t);
    Tree& operator=(Tree& t);
    Tree& operator|(Tree& t);    //  \/ объединение
    Tree& operator&(Tree& t);    //  /\ пересечение 
    Tree& operator/(Tree& t);    //    \  вычитание
private:
    Node* Root;
    static const int MinKey = 0;
    static const int MaxKey = 100;
    static const int MaxElements = 32;
    
    Node* insert(Node* p, int k);
    Node* remove(Node* p, int k);
    Node* balance(Node* p);
    Node* rotateright(Node* p);
    Node* rotateleft(Node* q);
    Node* findmin(Node* p);
    Node* removemin(Node* p);
    int height(Node* p);
    int bfactor(Node* p);
    void fixheight(Node* p);
    
    
    
        
    
    
};

#endif	/* AVLTREE_H */

