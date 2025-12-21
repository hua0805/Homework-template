
# 41343141

作業三
## 解題說明

本題要求設計一個使用循環連結串列來時做一個C++的Polynomial類別

### 解題策略

1. 定義 Term 儲存係數與指數，ChainNode 用動態資料結構來儲存項數不固定的多項式，Polynomial 用來簡化多項式的操作流程，避免特殊情況。
2. ChainNode<Term>* head 是為了避免處理空串列或第一項的特例以及簡化插入、刪除與走訪。
3. static ChainNode<Term>* avail 是為了提升效能而寫出來的，一直使用new/delete會浪費效能。
4. GetNode(const Term& t) 的作用是有舊節點就使用如果沒有那就創造一個新節點，並設定資料為 t。
5. RetNode(ChainNode<Term>* x) 的作用是把不需要的舊節點刪掉。
6. operator >>(輸入) 為了將「外部表示法」轉為內部 linked list，operator <<(輸出) 為了將內部資料結構轉回題目指定格式。
7. operator + 完成多項式的加法運算，operator - 用於多項式的減法運算，operator * 用於多項式的乘法運算，Evaluate 驗證多項式內容是否正確，並提供實際應用功能。


## 程式實作

以下為主要程式碼：

```cpp


#include <iostream>
#include <cmath>
using namespace std;

//儲存係數與指數//
struct Term {
    int coef;
    int exp;
};

//動態資料結構來儲存項數不固定的多項式//
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
    ChainNode<T>* head;   // header node (circular)

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

//簡化多項式的操作流程//
class Polynomial {
private:
    ChainNode<Term>* head;                  // header node
    static ChainNode<Term>* avail;          // 提升效能

    ChainNode<Term>* GetNode(const Term& t); //creative new node or seek old node and save file is t
    void RetNode(ChainNode<Term>* x);        //delete old node(無用的)

public:
    Polynomial();                           // constructor
    Polynomial(const Polynomial& a);        // copy constructor
    ~Polynomial();                          // destructor

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

//加法//
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

//減法//
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial negB = b;
    for (auto cur = negB.head->link; cur != negB.head; cur = cur->link) {
        cur->data.coef = -cur->data.coef;
    }
    return (*this + negB);
}

//乘法//
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

//帶入數字進去//
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

1. 能正確輸出*P1 P2*是多少、*P1+P2,P1-P2*,P1*P2fm,確認輸出正確以跟數值輸入進去(P1(x))也會輸出對的答案。  
2. 利用*Circular Linked List & Header Node*完成多項式的輸入輸出、加減乘法跟數值的計算。
3. *Available List*是提升記憶體的效率，為了避免輸入的變數太大或程式碼太多導致電腦記憶體不足。 

## 心得討論

##為什麼使用 *operator>>*與*operator<<*
*operator>>*：多載輸入運算子，讓多自訂資料型別使用能跟保持一致，用 cin >> p1  cin >> p2; 讀入資料。將輸入格式的解析，降低主程式複雜度，避免解析規則造成的錯誤。
*operator<<*：用多載輸出運算子，多項式的格式統一，主程式只需負責輸出物件，不必關心實際的細節。輸出格式需要調整時，修改 operator<<，即可影響所有輸出結果，提升程式的可維護性與擴充性。

### 心得

這次作業要求實作多項式並搭配鏈結串列，對我來說其實難度蠻高的，一開始我真的看不太懂整個程式在幹嘛，指標要怎麼用也沒有什麼概念，所以大多是先參考同學的寫法，再一行一行慢慢看、慢慢改，過程中也問了不少問題，在別人的幫忙下才把程式撐起來。其實我上學期就學過 C++ 的指標，也知道指標是存記憶體位址，但那時候只是在背定義，實際寫程式時根本不知道什麼情況該用、怎麼用。這次在實作多項式的過程中，慢慢才發現指標不只是拿來存資料，而是用來把每一項串起來，例如 `ChainNode<Term>*` 這種寫法，重點不是資料本身，而是用來表示各項之間的連結關係，讓整個多項式可以正常運作。另外我也比較能理解為什麼這個作業要用指標，因為多項式的項數一開始根本不確定，如果用陣列反而會被大小卡住，用指標加鏈結串列就可以視情況新增或刪除，比較有彈性。雖然這份程式不是我完全自己想出來的，也是在很多嘗試、修改跟同學協助下才完成，但至少讓我對指標實際在程式裡是怎麼用的、鏈結串列在做什麼，有比以前更清楚一點，也讓我知道寫程式不是只有一種正確答案，而是要依問題選擇比較適合的做法，對我這種基礎不太好的新手來說，能走到這一步就已經學到很多了。


