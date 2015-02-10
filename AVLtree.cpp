#include "AVLtree.h"


Tree::Tree()
{
    Root = nullptr;
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < MaxElements; ++i)
    {
        int NewKey = generator() % MaxKey;
        if (exist(MaxKey))
            i--;
        else
            add(NewKey);
    }
}

Tree::~Tree()
{
    delete Root;
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

Node* Tree::remove(Node* p, int k) // p = root
{
    if (p == nullptr) 
        return p;
    if (k < p->key)
        p->left = remove(p->left, k);
    else if (k > p->key)
        p->right = remove(p->right, k);
    else // k == p->key
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
    if( bfactor(p)==2 )
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
    return p->left ? findmin(p->left) : p;
}

Node* Tree::removemin(Node* p)
{
    if (p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

Node* Tree::find(Node* p, int k)
{
    if (p == nullptr)
        return nullptr;
    else if (p->key == k)
        return p;
    else if (p->key < k)
        return find(p->left, k);
    else if (p->key > k)
        return find(p->right, k);
}

int Tree::height(Node* p)
{
    return p ? p->height : 0;
}

int Tree::bfactor(Node* p)
{
    return height(p->right) - height(p->left);
}

void Tree::fixheight(Node* p)
{
    int hl = height(p->left);
    int hr = height(p->right);
    p->height = hl > hr ? hl+1 : hr+1;
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
        delete this->Root;
        this->Root = t.Root;
        return *this;
    }
    
}
Tree& Tree::operator&(Tree& t)
{
    
    return *this;
}
Tree& Tree::operator|(Tree& t)
{
    
    return *this;
}
Tree& Tree::operator/(Tree& t)
{
    
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