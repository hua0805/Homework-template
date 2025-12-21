

#include <iostream>
#include <cmath>
using namespace std;

struct Term {
    int coef;
    int exp;
};

template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;

    ChainNode(const T& d = T(), ChainNode<T>* l = nullptr)
        : data(d), link(l) {}
};

/*
template <class T>
class Chain {
public:
    typedef ChainIterator<T> iterator;

private:
    ChainNode<T>* head;   

public:
    Chain() {
        head = new ChainNode<T>();
        head->link = head;
    }

    ~Chain() {
        ChainNode<T>* cur = head->link;
        while (cur != head) {
            ChainNode<T>* temp = cur;
            cur = cur->link;
            delete temp;
        }
        delete head;
    }

    bool IsEmpty() const {
        return head->link == head;
    }

    iterator Begin() const {
        return iterator(head->link);
    }

    iterator End() const {
        return iterator(head);
    }

    void PushBack(const T& x) {
        ChainNode<T>* cur = head;
        while (cur->link != head)
            cur = cur->link;
        cur->link = new ChainNode<T>(x, head);
    }
};

*/

/*
template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;

public:
    ChainIterator(ChainNode<T>* start = nullptr) : current(start) {}

    T& operator*() const { return current->data; }

    ChainIterator<T>& operator++() {
        current = current->link;
        return *this;
    }

    bool operator!=(const ChainIterator<T>& rhs) const {
        return current != rhs.current;
    }
};
*/

class Polynomial {
private:
    ChainNode<Term>* head;                  
    static ChainNode<Term>* avail;          

    ChainNode<Term>* GetNode(const Term& t);
    void RetNode(ChainNode<Term>* x);        

public:
    Polynomial();                           
    Polynomial(const Polynomial& a);        
    ~Polynomial();                         

    Polynomial& operator=(const Polynomial& a);

    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;

    float Evaluate(float x) const;

    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);
};


ChainNode<Term>* Polynomial::avail = nullptr;


ChainNode<Term>* Polynomial::GetNode(const Term& t) {
    if (avail) {
        ChainNode<Term>* x = avail;
        avail = avail->link;
        x->data = t;
        return x;
    }
    return new ChainNode<Term>(t);
}

void Polynomial::RetNode(ChainNode<Term>* x) {
    x->link = avail;
    avail = x;
}


Polynomial::Polynomial() {
    head = new ChainNode<Term>();
    head->link = head;   // circular
}

Polynomial::~Polynomial() {
    ChainNode<Term>* cur = head->link;
    while (cur != head) {
        ChainNode<Term>* temp = cur;
        cur = cur->link;
        RetNode(temp);
    }
    delete head;
}


Polynomial::Polynomial(const Polynomial& a) : Polynomial() {
    *this = a;
}

Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this == &a) return *this;

    this->~Polynomial();

    head = new ChainNode<Term>();
    head->link = head;

    ChainNode<Term>* rear = head;
    ChainNode<Term>* cur = a.head->link;

    while (cur != a.head) {
        rear->link = GetNode(cur->data);
        rear = rear->link;
        cur = cur->link;
    }
    rear->link = head;
    return *this;
}


istream& operator>>(istream& is, Polynomial& x) { //cin change
    int n;
    is >> n;

    ChainNode<Term>* rear = x.head;
    for (int i = 0; i < n; i++) {
        Term t;
        is >> t.coef >> t.exp;
        rear->link = x.GetNode(t);
        rear = rear->link;
    }
    rear->link = x.head;
    return is;
}


ostream& operator<<(ostream& os, const Polynomial& x) { //cout change
    int count = 0;
    ChainNode<Term>* cur = x.head->link;

    while (cur != x.head) {
        count++;
        cur = cur->link;
    }

    os << count;
    cur = x.head->link;
    while (cur != x.head) {
        os << " " << cur->data.coef << " " << cur->data.exp;
        cur = cur->link;
    }
    return os;
}

Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial c;
    ChainNode<Term>* aPtr = head->link;
    ChainNode<Term>* bPtr = b.head->link;
    ChainNode<Term>* rear = c.head;

    while (aPtr != head && bPtr != b.head) {
        if (aPtr->data.exp == bPtr->data.exp) {
            int sum = aPtr->data.coef + bPtr->data.coef;
            if (sum != 0) {
                rear->link = c.GetNode({sum, aPtr->data.exp});
                rear = rear->link;
            }
            aPtr = aPtr->link;
            bPtr = bPtr->link;
        }
        else if (aPtr->data.exp > bPtr->data.exp) {
            rear->link = c.GetNode(aPtr->data);
            rear = rear->link;
            aPtr = aPtr->link;
        }
        else {
            rear->link = c.GetNode(bPtr->data);
            rear = rear->link;
            bPtr = bPtr->link;
        }
    }

    while (aPtr != head) {
        rear->link = c.GetNode(aPtr->data);
        rear = rear->link;
        aPtr = aPtr->link;
    }

    while (bPtr != b.head) {
        rear->link = c.GetNode(bPtr->data);
        rear = rear->link;
        bPtr = bPtr->link;
    }

    rear->link = c.head;
    return c;
}


Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial negB = b;
    for (auto cur = negB.head->link; cur != negB.head; cur = cur->link) {
        cur->data.coef = -cur->data.coef;
    }
    return (*this + negB);
}


Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial c;

    for (auto aPtr = head->link; aPtr != head; aPtr = aPtr->link) {
        Polynomial temp;
        ChainNode<Term>* rear = temp.head;

        for (auto bPtr = b.head->link; bPtr != b.head; bPtr = bPtr->link) {
            Term t;
            t.coef = aPtr->data.coef * bPtr->data.coef;
            t.exp  = aPtr->data.exp + bPtr->data.exp;
            rear->link = temp.GetNode(t);
            rear = rear->link;
        }
        rear->link = temp.head;
        c = c + temp;
    }
    return c;
}


float Polynomial::Evaluate(float x) const {
    float result = 0;
    for (auto cur = head->link; cur != head; cur = cur->link) {
        result += cur->data.coef * pow(x, cur->data.exp);
    }
    return result;
}


int main() {
    Polynomial p1, p2;

    cout << "Input polynomial 1: ";
    cin >> p1;
    cout << "Input polynomial 2: ";
    cin >> p2;

    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;

    cout << "P1 + P2 = " << (p1 + p2) << endl;
    cout << "P1 - P2 = " << (p1 - p2) << endl;
    cout << "P1 * P2 = " << (p1 * p2) << endl;

    cout << "P1(2) = " << p1.Evaluate(2) << endl;

    return 0;
}

