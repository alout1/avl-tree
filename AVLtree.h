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
    
    void insert(int key);
    void remove(int key);
    bool exist(int key);
    void merge(Tree& t);
    void subst(Tree& t, int pos);
    void change(Tree& t, int pos);
    
    friend std::ostream& operator<<(std::ostream& out, Tree& t);
    friend std::ostream& operator>>(std::ostream& out, Tree& t);
    Tree& operator=(Tree& t);
    Tree& operator|(Tree& t);    //  \/ объединение
    Tree& operator&(Tree& t);    //  /\ пересечение 
    Tree& operator/(Tree& t);    //    \  вычитание
    
private:
    Node* root;
    int size = 0;
    
    static const int MinKey = 0;
    static const int MaxKey = 100;
    static const int MaxElements = 5; //32
    
    Node* insert(Node* p, int k);
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
    void displayTree(std::ostream& out, Node *current, int indent);
    void displaySequence(std::ostream& out, Node *current);
    void toVector(Node* q, std::vector<int>* v); 
    void deleteTree(Node* q);    
    void updatePositions();
    void traverseUpdatePositions(Node* p);
    
    
    void traverseMerge(Node* p);
    
    // merge, subst, change

};

#endif	/* AVLTREE_H */

