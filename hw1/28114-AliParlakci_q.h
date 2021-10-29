#ifndef _Q_H
#define _Q_H

struct node
{
    int x;
    node* next;
    node() : x(-1), next(nullptr) {};
    node(int xx, node* n = nullptr) : x(xx), next(n) {};
};

class Q
{
public:
    Q();
    Q(const Q& copy);
    ~Q();
    void enqueue(int x);
    void dequeue(int& x);
    bool isEmpty() const;

private:
    node* front;
    node* back;
};

#endif