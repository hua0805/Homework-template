
# 41343141

作業三
## 解題說明

要求設計一個循環連結串列來時做一個C++的Polynomial類別

### 解題策略

1. 定義 Term 儲存係數與指數，ChainNode 用動態資料結構來儲存項數不固定的多項式，Polynomial 用來簡化多項式的操作流程。
2. ChainNode<Term>* head 為了避免處理空串列或第一項的特例、刪除與走訪。
3. static ChainNode<Term>* avail 為了提升效能，一直new/delete會浪費效能。
4. GetNode(const Term& t) 作用是有舊節點就使用如果沒有那就創造一個新節點，設定資料為 t。
5. RetNode(ChainNode<Term>* x) 作用把不需要的舊節點刪掉。
6. operator >>(輸入) 將「外部表示法」轉為內部 linked list，operator <<(輸出) 將內部資料結構轉回題目指定格式。
7. operator + 完成多項式的加法運算，operator - 多項式的減法運算，operator * 多項式的乘法運算，Evaluate 驗證多項式內容是否正確，提供實際應用功能。


## 程式實作

主要程式碼：

```cpp


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


```

## 申論及開發報告

## 效能分析

1. 時間複雜度：Polynomial():O(1)  ~Polynomial():O(k)  operator>>:O(n)  operator<<:O(n)  operator+:O(m+n)   operator-:O(m+n)  operator*:O(m*n)  Evaluate:O(m)  GetNode / RetNode:O(1)。
2. 空間複雜度：Polynomial():O(n)  operator+:O(m+n)   operator-:O(m+n)  operator*:O(m*n)  Evaluate:O(1)。

## 測試與驗證

### 測試案例

<img width="738" height="217" alt="image" src="https://github.com/user-attachments/assets/0e6dc5e4-5446-4bde-9269-46b70144ae78" />

<img width="776" height="167" alt="image" src="https://github.com/user-attachments/assets/cf801f5d-8039-46cf-b576-2b3fad529ccc" />

<img width="806" height="164" alt="image" src="https://github.com/user-attachments/assets/4a8d3fde-2475-4078-bc33-1e4c301c5ef2" />


### 編譯與執行指令

```shell
$ g++ HW3.cpp -o HW3
$ .\HW3

```

### 結論

1. 正確輸出*P1 P2*是多少、*P1+P2,P1-P2*,P1*P2fm,確認輸出正確以跟數值輸入進去(P1(x))會輸出對的答案。  
2. 利用*Circular Linked List & Header Node*完成多項式的輸入輸出、加減乘法的計算。
3. *Available List*提升記憶體的效率，為了避免輸入的變數太大或程式碼太多導致電腦記憶體不足。 

## 心得討論

##為什麼使用 *operator>>*與*operator<<*
*operator>>*：多載輸入運算子，讓多自訂資料型別使用能跟保持一致，用 cin >> p1  cin >> p2; 讀入資料。將輸入格式的解析，降低主程式複雜度，避免解析規則造成的錯誤。
*operator<<*：用多載輸出運算子，多項式的格式統一，主程式只需負責輸出物件，不必關心實際的細節。輸出格式需要調整時，修改 operator<<，即可影響所有輸出結果，提升程式的可維護性與擴充性。

### 心得

這次的作業是要用鏈結串列來做多項式，一開始對我來說真的有點困難，因為我連整個程式在做什麼都看不太懂，尤其是指標的部分更是完全沒概念。雖然之前上課有學過 C++ 的指標，知道它是用來存記憶體位址，但實際要寫的時候，根本不知道什麼時候該用、怎麼用。所以一開始我是先參考同學的程式碼，然後自己慢慢一行一行看，試著理解每一段在做什麼。有不懂的地方就去問同學，或是自己改一點點看會發生什麼事。這樣反覆看、改、問，才慢慢抓到一些感覺。在做的過程中，我才比較理解指標的用途，不只是單純存資料的位置，而是可以把不同的節點連在一起，變成一個串列。像 `ChainNode<Term>*` 這種寫法，其實重點是在「連接」，讓每一項多項式可以接到下一項，而不是只看裡面存了什麼資料。另外我也慢慢理解為什麼這個作業要用鏈結串列來做。因為多項式的項數不固定，如果用陣列的話，一開始就要決定大小，會比較不方便。但用鏈結串列的話，就可以依需要新增或刪除節點，比較有彈性。雖然這份程式大部分不是我從零開始想出來的，但透過一直看、一直改，再加上同學的幫忙，我對指標和鏈結串列的理解有比之前好很多。至少現在看到類似的寫法，不會像一開始那樣完全看不懂，也比較知道它在做什麼。對我來說，這次作業最大的收穫就是開始慢慢看懂這些以前覺得很難的東西。

