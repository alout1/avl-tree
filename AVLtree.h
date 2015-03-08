#ifndef AVLTREE_H
#define	AVLTREE_H

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

class Node
{
public:
    int position;
    int key;
    int height;
    Node* left;
    Node* right;
    Node(int k, int pos)
    {
        position = pos;
        key = k;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

class Tree
{
public:
    Tree();
    Tree(Tree&) = delete;
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
    int size = 0;
    
    static const int MinKey = 0;
    static const int MaxKey = 100;
    static const int MaxElements = 5; //32
    
    Node* insert(Node* p, int k, int pos);
    Node* remove(Node* p, int k);
    Node* balance(Node* p);
    Node* rotateright(Node* p);
    Node* rotateleft(Node* q);
    Node* findmin(Node* p);
    Node* removemin(Node* p);
    Node* find(Node* p, int key);
    int height(Node* p);
    int bfactor(Node* p);
    void fixheight(Node* p);
    void display(Node *current, int indent);
    void toVector(Node* q, std::vector<int>* v); 
    void deleteTree(Node* q);
    
    // merge, subst, change

};

#endif	/* AVLTREE_H */

