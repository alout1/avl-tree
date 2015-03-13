#include "AVLtree.h"

Tree::Tree(int n)
{
    mode = VALUE;
    root = nullptr;
    nextPosition = 0;
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < n; ++i)
    {
        int newKey = generator() % MAXVALUE + MINVALUE;
        insertValue(newKey);
    } 
}
Tree::Tree(std::initializer_list<int> args)
{
    mode = SEQUENCE;
    root = nullptr;
    nextPosition = 0;
    std::initializer_list<int>::iterator it;
    for (it = args.begin(); it != args.end(); ++it)
        insertAtPosition(*it, nextPosition);    
}

Tree::~Tree()
{
    deleteTree(root);
}

void Tree::changeModeToValue()
{
    if (mode == VALUE)
        return;
    mode = VALUE;
    std::vector<int>* v = this->toVector();
    deleteTree(root);
    root = nullptr;
    for (int i = 0; i < v->size(); ++i)
        insertValue(v->at(i));
    
}
void Tree::changeModeToSequence()
{
    if (mode == SEQUENCE)
        return;
    mode = SEQUENCE;
    std::vector<int>* v = this->toVector();
    deleteTree(root);
    root = nullptr;
    for (int i = 0; i < v->size(); ++i)
        insertAtPosition(v->at(i), this->size());
    
}

int Tree::size()
{
    return nextPosition;
}

Node* Tree::insertValue(Node* p, int val)
{
    if (p == nullptr)
        return new Node(val, nextPosition++);
    else if (val < p->value)
        p->left = insertValue(p->left, val);
    else
        p->right = insertValue(p->right, val);
    return balance(p);
}

Node* Tree::removeValue(Node* p, int val)
{
    if (p == nullptr) 
        return p;
    if (val < p->value)
        p->left = removeValue(p->left, val);
    else if (val > p->value)
        p->right = removeValue(p->right, val);
    else 
    {
        Node* q = p->left;
        Node* r = p->right;
        delete p; 
        if (!r)
            return q;
        Node* min = findMin(r);
        min->right = removeMin(r);
        min->left = q;
        
        return balance(min);
    }
    return balance(p);
}

Node* Tree::balance(Node* p)
{
 
    fixHeigth(p);
    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
            p->right = rotateRight(p->right);
        return rotateLeft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
            p->left = rotateLeft(p->left);
        return rotateRight(p);
    }
    return p;

}

Node* Tree::rotateRight(Node* p)
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeigth(p);
    fixHeigth(q);
    return q;
}

Node* Tree::rotateLeft(Node* q)
{
    /*
                     |                                           |
                    q(2)                                    p(0)
                   / \          ==>                      / \
            n[a]  p(1)n+2         n+1(0)q  [c]n+1
                     / \                                    / \
                n[b] [c]n+1                n[a] [b]n
        */
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeigth(q);
    fixHeigth(p);
    return p;
}

Node* Tree::findMin(Node* p)
{
    if (p->left)
        return findMin(p->left);
    else
        return p;
}

Node* Tree::removeMin(Node* p)
{
    if (p->left == nullptr)
        return p->right;
    p->left = removeMin(p->left);
    return balance(p);
}

Node* Tree::find(Node* p, int val)
{
    if (p == nullptr)
        return nullptr;
    if (p->value == val)
        return p;
    if (val < p->value)
        return find(p->left, val); 
    if (val > p->value)
        return find(p->right, val);
    
    assert(false);
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

void Tree::fixHeigth(Node* p)
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
        this->nextPosition = t.nextPosition;
        t.root = nullptr;  // move
        return *this;
    }
    
}
Tree& Tree::operator&(Tree& t)
{
    
    this->changeModeToValue();
    t.changeModeToValue();
    
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
    this->nextPosition = 0;
    this->root = nullptr;
    
    for (int i = 0; i < Result.size(); ++i)
        this->insertValue(Result[i]);
    
    return *this;
}
Tree& Tree::operator|(Tree& t)
{
    this->changeModeToValue();
    t.changeModeToValue();
    
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
    this->nextPosition = 0;
    this->root = nullptr;
    
    for (int i = 0; i < LeftTree.size(); ++i)
        this->insertValue(LeftTree[i]);
    
    return *this;
}
Tree& Tree::operator/(Tree& t)
{
    this->changeModeToValue();
    t.changeModeToValue();
    
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
    this->nextPosition = 0;
    this->root = nullptr;
    
    for (int i = 0; i < LeftTree.size(); ++i)
    {
        this->insertValue(LeftTree[i]);
    }
    return *this;
}

void Tree::insertValue(int val)
{
    if (mode != VALUE)
        return;
    if (!existValue(val))
       root = insertValue(root, val);
}

bool Tree::existValue(int val) 
{
    if (find(root, val))
        return true;
    else
        return false;
}

void Tree::removeValue(int val)
{
    if (existValue(val))
        root = removeValue(root, val);
    nextPosition--;
}

void Tree::displayTree(std::ostream& out, Node *current, int indent)
{
    if (current != nullptr)
    {
        displayTree(out, current->right, indent + 4);
        if (indent > 0)
        {   
            for (int i = 0; i < indent; ++i)
                out << ' ';
            out <<' ';
        } 
        if (mode == VALUE)
            out << current->value << std::endl;
        else  // sequence
            out << current->position << "(" << current->value << ")" << std::endl;
        displayTree(out, current->left, indent + 4);
    }
}

void Tree::displaySequence(std::ostream& out, Node *current)
{
    if (current == nullptr)
        return;
    if (current->left)
        displaySequence(out, current->left);
    out << current->value << " " ;
    if (current->right)
        displaySequence(out, current->right);
    
    
}
std::vector<int>* Tree::toVector()
{
    std::vector<int>* v = new std::vector<int>; 
    toVector(root, v);
    return v;
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
    v->push_back(q->value);
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

void Tree::fixPositions()
{
    nextPosition = 0;
    fixPositions(root);
}

void Tree::fixPositions(Node* p)
{
    if(p == nullptr)
        return;
    if (p->left)
        fixPositions(p->left);
    p->position = nextPosition++;
    if (p->right)
        fixPositions(p->right);   
    
}

void Tree::merge(Tree& t) // O(n*log(n))
{
    changeModeToValue();
    merge(t.root);
    fixPositions();
}

void Tree::merge(Node* p)
{
    if(p == nullptr)
        return;
    if (p->left)
        merge(p->left);
    this->root = this->insertValue(this->root, p->value); // this = lvalue tree
    if (p->right)
        merge(p->right);
}

void Tree::insertAtPosition(int val, int pos)
{
    root = insertAtPosition(root, val, pos);
    nextPosition++;
    fixPositions(); 
}

Node* Tree::insertAtPosition(Node* p, int val, int pos)
{
    if (p == nullptr)
        return new Node(val, nextPosition);
    else if (pos <= p->position)                        //    <=                  
        p->left = insertAtPosition(p->left, val, pos);
    else
        p->right = insertAtPosition(p->right, val, pos);
    return balance(p); // o_O 
}

void Tree::removeFromPosition(int pos)
{
    root = removeFromPosition(root, pos);
    nextPosition--;
    fixPositions();
}

Node* Tree::removeFromPosition(Node* p, int pos)
{
    if (p == nullptr) 
        return p;
    if (pos < p->position)
        p->left = removeFromPosition(p->left, pos);
    else if (pos > p->position)
        p->right = removeFromPosition(p->right, pos);
    else 
    {
        Node* q = p->left;
        Node* r = p->right;
        delete p; 
        if (r == nullptr) // !r
            return q;
        Node* min = findMin(r);
        min->right = removeMin(r);
        min->left = q;
        
        return balance(min);
    }
    return balance(p);
}

void Tree::subst(Tree& t, int pos)
{
    changeModeToSequence();
    std::vector<int>* v = t.toVector();
    for (int i = 0; i < t.size(); ++i)
        insertAtPosition(v->at(i), pos + i);
    delete v;
}

void Tree::change(Tree& t, int pos)
{
    changeModeToSequence();
    for (int i = this->size(); i >= pos; --i)
        this->removeFromPosition(i);
    std::vector<int>* v = t.toVector();
    for (int i = 0; i < t.size(); ++i)
        this->insertAtPosition(v->at(i), i + pos);
    delete v; 
}