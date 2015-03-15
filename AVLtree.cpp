#include "AVLtree.h"

Tree::Tree(int n)
{
    mode = VALUE;
    root = nullptr;
    nextPosition = 0;
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < n; ++i)
    {
        int newValue = generator() % MAXVALUE + MINVALUE;
        insertValue(newValue);
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

void Tree::switchModeToValue()
{
    if (mode == VALUE)
        return;
    mode = VALUE;
    std::vector<int>* v = this->toVector();
    deleteTree(root);
    root = nullptr;
    for (int i = 0; i < v->size(); ++i)
        insertValue(v->at(i));
    delete v;
}
void Tree::switchModeToSequence()
{
    if (mode == SEQUENCE)
        return;
    mode = SEQUENCE;
    std::vector<int>* v = this->toVector();
    deleteTree(root);
    root = nullptr;
    for (int i = 0; i < v->size(); ++i)
        insertAtPosition(v->at(i), this->size());
    delete v;
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
    //return balance(p);
    Node* x = balance(p);
    return x;
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
        if (r == nullptr)
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
    //assert(false);
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
        t.root = nullptr;  // крадем дерево!
        return *this;
    }
    
}
Tree& Tree::operator&(Tree& t)
{
    this->switchModeToValue();
    t.switchModeToValue();
    
    std::vector<int> leftOperand;
    std::vector<int> rightOperand;
    std::vector<int> result;
    toVector(this->root, &leftOperand); 
    toVector(t.root, &rightOperand);
    
    int l = 0, r = 0;
    while (l < leftOperand.size() && r < rightOperand.size())
    {         
        if (rightOperand[r] < leftOperand[l])
        {r++; continue;}
        if (rightOperand[r] > leftOperand[l])
        {l++; continue;}
        if (rightOperand[r] == leftOperand[l])
        {
            result.push_back(rightOperand[r]);
            leftOperand.erase(leftOperand.begin() + l);
            rightOperand.erase(rightOperand.begin() + r);
        }
    }
    deleteTree(this->root);
    nextPosition = 0;
    root = nullptr;
    
    for (int i = 0; i < result.size(); ++i)
        this->insertValue(result[i]);
    
    return *this;
}
Tree& Tree::operator|(Tree& t)
{
    this->switchModeToValue();
    t.switchModeToValue();
    
    std::vector<int> leftOperand;
    std::vector<int> rightOrerand;
    toVector(this->root, &leftOperand);
    toVector(t.root, &rightOrerand);
    
    int l = 0, r = 0;
    while (l < leftOperand.size() && r < rightOrerand.size())
    {         
        if (rightOrerand[r] < leftOperand[l])
        {r++; continue;}
        if (rightOrerand[r] > leftOperand[l])
        {l++; continue;}
        if (rightOrerand[r] == leftOperand[l])
        {
            rightOrerand.erase(rightOrerand.begin() + r);
        }
    }
    for (int i = 0; i < rightOrerand.size(); ++i)
        leftOperand.push_back(rightOrerand[i]);
            
    deleteTree(this->root);
    this->nextPosition = 0;
    this->root = nullptr;
    
    for (int i = 0; i < leftOperand.size(); ++i)
        this->insertValue(leftOperand[i]);
    
    return *this;
}
Tree& Tree::operator/(Tree& t)
{
    this->switchModeToValue();
    t.switchModeToValue();
    
    std::vector<int> leftTree;
    std::vector<int> rightTree;
    toVector(this->root, &leftTree);
    toVector(t.root, &rightTree);
    
    int l = 0, r = 0;
    while (l < leftTree.size() && r < rightTree.size())
    { 
        if (rightTree[r] < leftTree[l])
        {r++; continue;}
        if (rightTree[r] > leftTree[l])
        {l++; continue;}
        if (rightTree[r] == leftTree[l])
        {
            leftTree.erase(leftTree.begin() + l);
            rightTree.erase(rightTree.begin() + r);
        }
    }
        
    deleteTree(this->root);
    this->nextPosition = 0;
    this->root = nullptr;
    
    for (int i = 0; i < leftTree.size(); ++i)
    {
        this->insertValue(leftTree[i]);
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
    // assert(mode == VALUE);
    if (find(root, val))
        return true;
    else
        return false;
}

void Tree::removeValue(int val)
{
    if (mode != VALUE)
        return;
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
        return;
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
    else if (pos <= p->position)                // при совпадении позиций вставка влево --  <=                  
        p->left = insertAtPosition(p->left, val, pos);
    else
        p->right = insertAtPosition(p->right, val, pos);
    return balance(p); 
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
        if (r == nullptr) 
            return q;
        Node* min = findMin(r);
        min->right = removeMin(r);
        min->left = q;
        
        return balance(min);
    }
    return balance(p);
}

void Tree::merge(Tree& t) // O(n^2)
{
    switchModeToSequence();
    
    std::vector<int>* q = this->toVector();
    std::vector<int>* p = t.toVector();
    std::vector<int> result;
    result.resize(this->size() + t.size());
    std::vector<int>::iterator it;   
    
    // http://www.cplusplus.com/reference/algorithm/sort/
    //http://www.cplusplus.com/reference/algorithm/merge/
    std::sort(p->begin(), p->end());                                         // n*log(n)
    std::sort(q->begin(), q->end());
    std::merge(p->begin(), p->end(), q->begin(), q->end(), result.begin());  // n
    
    deleteTree(this->root);
    root = nullptr;
    nextPosition = 0;
    for (it = result.begin(); it != result.end(); ++it)    // n
        insertAtPosition(*it, nextPosition);                // n^2
    
    delete p;
    delete q;   
}

void Tree::subst(Tree& t, int pos) // O(n^2)
{
    switchModeToSequence();                
    std::vector<int>* v = t.toVector();
    for (int i = 0; i < t.size(); ++i)        // n
        insertAtPosition(v->at(i), pos + i);  // *n = n^2
    delete v;
}

void Tree::change(Tree& t, int pos) // O(n^2)
{
    switchModeToSequence();
    std::vector<int>* l = this->toVector();
    std::vector<int>* r = t.toVector();
    
    if (pos > l->size())
        pos = l->size();    // если задана слишком большая позиция, просто вставка в конец
    
    if (l->size() < r->size() + pos)
        l->resize(pos + r->size()); // если правая не помещается в левую, увеличить левую
    
    int i, k;
    for (i = pos, k = 0; k < r->size(); ++i, ++k)   // выполнение change() на векторах)
    {
        (*l)[i] = (*r)[k];
    }
    
    deleteTree(this->root);         // пересборка дерева(this) из левого вектора(l)
    root = nullptr;
    nextPosition = 0;
    for (int i = 0; i < l->size(); ++i)        
        insertAtPosition(l->at(i), i);  
    
    delete r;
    delete l;   
}