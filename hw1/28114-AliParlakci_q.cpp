#include "28114-AliParlakci_q.h"

Q::Q() : front(nullptr), back(nullptr)
{
}

Q::~Q()
{
    while (!isEmpty())
    {
        int x;
        dequeue(x);
    }
}

void Q::enqueue(int x)
{
    if (front == nullptr)
    {
        front = back = new node(x, nullptr);
    }
    else
    {
        back->next = new node(x, nullptr);
        back = back->next;
    }
}

void Q::dequeue(int &x)
{
    x = front->x;
    node *temp = front;
    front = front->next;
    delete temp;

    if (front == nullptr)
        back = front;
}

bool Q::isEmpty() const
{
    return front == nullptr;
}