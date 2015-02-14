#include "AVLtree.h"

Tree::Tree()
{
    Root = nullptr;
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < MaxElements; ++i)
    {
        int NewKey = generator() % MaxKey;
            add(NewKey);
    }
}

Tree::~Tree()
{
    deleteTree(Root);
}

Node* Tree::insert(Node* p, int k)
{
    if (p == nullptr)
        return new Node(k);
    else if (k < p->key)
        p->left = insert(p->left, k);
    else
        p->right = insert(p->right, k);
    return balance(p);
}

Node* Tree::remove(Node* p, int k)
{
    if (p == nullptr) 
        return p;
    if (k < p->key)
        p->left = remove(p->left, k);
    else if (k > p->key)
        p->right = remove(p->right, k);
    else 
    {
        Node* q = p->left;
        Node* r = p->right;
        delete p; 
        if (!r)
            return q;
        Node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
}

Node* Tree::balance(Node* p)
{
 
    fixheight(p);
    if( bfactor(p) == 2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;

}

Node* Tree::rotateright(Node* p)
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

Node* Tree::rotateleft(Node* q)
{
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

Node* Tree::findmin(Node* p)
{
    if (p->left)
        return findmin(p->left);
    else
        return p;
}

Node* Tree::removemin(Node* p)
{
    if (p->left == nullptr)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

Node* Tree::find(Node* p, int k)
{
    if (p == nullptr)
        return nullptr;
    if (p->key == k)
        return p;
    if (p->key < k)
        return find(p->left, k);
    if (p->key > k)
        return find(p->right, k);
}

int Tree::height(Node* p)
{
    if (p != nullptr)
        return p->height;
    else
        return 0;
}

int Tree::bfactor(Node* p)
{
    return height(p->right) - height(p->left);
}

void Tree::fixheight(Node* p)
{
    int hl = height(p->left);
    int hr = height(p->right);
    if (hl > hr)
        p->height = hl + 1;
    else
        p->height = hr + 1;
}

std::ostream& operator<<(std::ostream& out, Tree& t)
{
    t.display(t.Root, 0);
    return out;
}

Tree& Tree::operator=(Tree& t)
{
    if (this->Root == t.Root)
        return *this;
    else
    {
        deleteTree(this->Root);
        this->Root = t.Root;
        return *this;
    }
    
}
Tree& Tree::operator&(Tree& t)
{
    std::vector<int> LeftTree;
    std::vector<int> RightTree;
    std::vector<int> Result;
    toVector(this->Root, &LeftTree);
    toVector(t.Root, &RightTree);
    
    int l = 0, r = 0;
    while (l < LeftTree.size() && r < RightTree.size())
    {         
        if (RightTree[r] < LeftTree[l])
        {r++; continue;}
        if (RightTree[r] > LeftTree[l])
        {l++; continue;}
        if (RightTree[r] == LeftTree[l])
        {
            Result.push_back(RightTree[r]);
            LeftTree.erase(LeftTree.begin() + l);
            RightTree.erase(RightTree.begin() + r);
        }
    }
    deleteTree(this->Root);
    this->Root = nullptr;
    
    for (int i = 0; i < Result.size(); ++i)
        this->add(Result[i]);
    
    return *this;
}
Tree& Tree::operator|(Tree& t)
{
    std::vector<int> LeftTree;
    std::vector<int> RightTree;
    toVector(this->Root, &LeftTree);
    toVector(t.Root, &RightTree);
    
    int l = 0, r = 0;
    while (l < LeftTree.size() && r < RightTree.size())
    {         
        if (RightTree[r] < LeftTree[l])
        {r++; continue;}
        if (RightTree[r] > LeftTree[l])
        {l++; continue;}
        if (RightTree[r] == LeftTree[l])
        {
            RightTree.erase(RightTree.begin() + r);
        }
    }
    for (int i = 0; i < RightTree.size(); ++i)
        LeftTree.push_back(RightTree[i]);
            
    deleteTree(this->Root);
    this->Root = nullptr;
    
    for (int i = 0; i < LeftTree.size(); ++i)
        this->add(LeftTree[i]);
    
    return *this;
}
Tree& Tree::operator/(Tree& t)
{
    std::vector<int> LeftTree;
    std::vector<int> RightTree;
    toVector(this->Root, &LeftTree);
    toVector(t.Root, &RightTree);
    
    
    int l = 0, r = 0;
      while (l < LeftTree.size() && r < RightTree.size())
    { 
        if (RightTree[r] < LeftTree[l])
        {r++; continue;}
        if (RightTree[r] > LeftTree[l])
        {l++; continue;}
        if (RightTree[r] == LeftTree[l])
        {
            LeftTree.erase(LeftTree.begin() + l);
            RightTree.erase(RightTree.begin() + r);
        }
    }
        
    deleteTree(this->Root);
    this->Root = nullptr;
    
    for (int i = 0; i < LeftTree.size(); ++i)
    {
        this->add(LeftTree[i]);
    }
    return *this;
}

void Tree::add(int key)
{
    if (!exist(key))
       Root = insert(Root, key);
}

bool Tree::exist(int key) 
{
    if (find(Root, key))
        return true;
    else
        return false;
}

void Tree::remove(int key)
{
    if (exist(key))
        Root = remove(Root, key);
}

void Tree::display(Node *current, int indent)
{
    if (current != nullptr)
    {
        display(current->right, indent + 4);
        if (indent > 0)
            std::cout << std::setw(indent) << " ";
        std::cout << current->key << std::endl;
        display(current->left, indent + 4);
    }
}

void Tree::toVector(Node* q, std::vector<int>* v)
{
    if (q->left)
        toVector(q->left, v);
    v->push_back(q->key);
    if (q->right)
        toVector(q->right, v);
    
}

void Tree::deleteTree(Node* q)
{
    if (q->left)
        deleteTree(q->left);
    if (q->right)
        deleteTree(q->right);
    delete q;
}