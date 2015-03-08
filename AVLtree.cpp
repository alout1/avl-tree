#include "AVLtree.h"

Tree::Tree()
{
    root = nullptr;
    size=0;
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < MaxElements; ++i)
    {
        int newKey = generator() % MaxKey;
        insert(newKey);
    } 
}

Tree::~Tree()
{
    deleteTree(root);
}

Node* Tree::insert(Node* p, int k)
{
    if (p == nullptr)
        return new Node(k, size++);
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
    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
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
        return find(p->right, k); 
    if (p->key > k)
        return find(p->left, k);
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
    t.displayTree(out, t.root, 0);
    return out;
}
std::ostream& operator>>(std::ostream& out, Tree& t)
{
    out << "< ";
    t.displaySequence(out, t.root);
    out << ">" << std::endl;
    return out;
}

Tree& Tree::operator=(Tree& t)
{
    if (this->root == t.root)
        return *this;
    else
    {
        deleteTree(this->root);
        this->root = t.root;
        this->size = t.size;
        t.root = nullptr;  // move
        return *this;
    }
    
}
Tree& Tree::operator&(Tree& t)
{
    std::vector<int> LeftTree;
    std::vector<int> RightTree;
    std::vector<int> Result;
    toVector(this->root, &LeftTree);
    toVector(t.root, &RightTree);
    
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
    deleteTree(this->root);
    this->size = 0;
    this->root = nullptr;
    
    for (int i = 0; i < Result.size(); ++i)
        this->insert(Result[i]);
    
    return *this;
}
Tree& Tree::operator|(Tree& t)
{
    std::vector<int> LeftTree;
    std::vector<int> RightTree;
    toVector(this->root, &LeftTree);
    toVector(t.root, &RightTree);
    
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
            
    deleteTree(this->root);
    this->size = 0;
    this->root = nullptr;
    
    for (int i = 0; i < LeftTree.size(); ++i)
        this->insert(LeftTree[i]);
    
    return *this;
}
Tree& Tree::operator/(Tree& t)
{
    std::vector<int> LeftTree;
    std::vector<int> RightTree;
    toVector(this->root, &LeftTree);
    toVector(t.root, &RightTree);
    
    
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
        
    deleteTree(this->root);
    this->size = 0;
    this->root = nullptr;
    
    for (int i = 0; i < LeftTree.size(); ++i)
    {
        this->insert(LeftTree[i]);
    }
    return *this;
}

void Tree::insert(int key)
{
    if (!exist(key))
       root = insert(root, key);
    updatePositions();
}

bool Tree::exist(int key) 
{
    if (find(root, key))
        return true;
    else
        return false;
}

void Tree::remove(int key)
{
    if (exist(key))
        root = remove(root, key);
    size--;
    updatePositions();
}

void Tree::displayTree(std::ostream& out, Node *current, int indent)
{
    if (current != nullptr)
    {
        displayTree(out, current->right, indent + 4);
        if (indent > 0)
        {   
            //std::cout << std::setw(indent) << ".";
            for (int i = 0; i < indent; ++i)
                out << ' ';
            out <<' ';
        } 
        out << current->position << "(" << current->key << ") " << std::endl;
        displayTree(out, current->left, indent + 4);
    }
}

void Tree::displaySequence(std::ostream& out, Node *current)
{
    if (current == nullptr)
        return;
    if (current->left)
        displaySequence(out, current->left);
    out << current->key << " " ;
    if (current->right)
        displaySequence(out, current->right);
    
    
}

void Tree::toVector(Node* q, std::vector<int>* v)
{
    if (q == nullptr)
    {
       // std::cerr << "toVector(): recieved a nullptr!\n";
        return;
    }
    if (q->left)
        toVector(q->left, v);
    v->push_back(q->key);
    if (q->right)
        toVector(q->right, v);
    
}

void Tree::deleteTree(Node* q)
{
    if (q == nullptr)
        return;
    if (q->left)
        deleteTree(q->left);
    if (q->right)
        deleteTree(q->right);
    delete q;
}

void Tree::updatePositions()
{
    size = 0;
    traverseUpdatePositions(root);
}

void Tree::traverseUpdatePositions(Node* p)
{
    if(p == nullptr)
        return;
    if (p->left)
        traverseUpdatePositions(p->left);
    p->position = size++;
    if (p->right)
        traverseUpdatePositions(p->right);   
    
}

void Tree::merge(Tree& t) // O(n)
{
    traverseMerge(t.root);
    updatePositions();
}

void Tree::traverseMerge(Node* p)
{
    if(p == nullptr)
        return;
    if (p->left)
        traverseMerge(p->left);
    this->root = this->insert(this->root, p->key); // this = "left" tree
    if (p->right)
        traverseMerge(p->right);
}