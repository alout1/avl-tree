#ifndef AVLTREE_H
#define	AVLTREE_H

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <initializer_list>



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
        key = k;            // todo: key=>value
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

class Tree
{
public:
    // конструкторы: n случайных элементов, режим value
    Tree(int n);
    // или список инициализации, режим sequence
    Tree(std::initializer_list<int> args);
    ~Tree();
    
    // показать размер
    int size();
    
    // никаких копирований
    Tree(Tree&) = delete;
     
    // простые внешние функции: вставка, удаление, проверка
    void insertValue(int key);
    void removeValue(int key);
    bool existValue(int key);
    
    // вставка/удаление по позиции
    void insertAtPosition(int key, int pos);
    void removeFromPosition(int pos);
    
    // функции для последовательностей: слияние, подстановка, замена
    void merge(Tree& t);
    void subst(Tree& t, int pos);
    void change(Tree& t, int pos);
    
    // операторы для красивого вывода: в виде дерева(<<) или последовательности(>>)
    friend std::ostream& operator<<(std::ostream& out, Tree& t);
    friend std::ostream& operator>>(std::ostream& out, Tree& t);
    
    //  нехорошее присваивание, которое крадет дерево у  rvalue
    Tree& operator=(Tree& t);
    
    // "логические" операции
    Tree& operator|(Tree& t);    //  \/ объединение
    Tree& operator&(Tree& t);    //  /\ пересечение 
    Tree& operator/(Tree& t);    //    \  вычитание
    
    // запись элементов в вектор
    std::vector<int>* toVector();
    
    // переключение "режимов"
    void changeModeToValue();
    void changeModeToSequence();
    
private:
    // указатель на текущий корень дерева
    Node* root;
    
    // количество элементов
    int nextPosition = 0;
    
    // "настройки": диапазон генерируемых значений
    static const int MINVALUE = 0;
    static const int MAXVALUE = 100;
    
    // "режим работы" дерева:  множество или последовательность
    enum {VALUE = 0, SEQUENCE = 1};
    int mode;
    
    // рабочие рекурсивные вставка/удаление в поддерево р, возвращают новый корень 
    Node* insertValue(Node* p, int k);
    Node* removeValue(Node* p, int k);
    
    // аналогично, но с позициями 
    Node* insertAtPosition(Node* p, int key, int pos);
    Node* removeFromPosition(Node* p, int pos);
    
    // балансировка поддерева
    Node* balance(Node* p);
    
    // "малые" повороты влево-вправо, возвращется новый корень
    Node* rotateRight(Node* p);
    Node* rotateLeft(Node* q);
    
    // найти/удалить наименьший элемент в поддереве
    Node* findMin(Node* p);
    Node* removeMin(Node* p);
    
    // бинарный поиск по ключу
    Node* find(Node* p, int key);
    
    // посчитать высоту и balance factor узла
    int height(Node* p);
    int bfactor(Node* p);
    
    // сохранение правильного значения высоты после поворотов
    void fixHeigth(Node* p);
    
    // обходы дерева с выводом значений в виде дерева/последовательности, вызываются операторами
    void displayTree(std::ostream& out, Node *current, int indent);
    void displaySequence(std::ostream& out, Node *current);
    
    // обход дерева и запись в вектор
    void toVector(Node* q, std::vector<int>* v);
    
    // обход с удалением узлов
    void deleteTree(Node* q);

    // исправление значений position узлов
    void fixPositions();
    void fixPositions(Node* p);
    
    // обход и слияние деревьев
    void merge(Node* p);
    
    

};

#endif	/* AVLTREE_H */

