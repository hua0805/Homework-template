
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


### 程式實作

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


## 效能分析

1. 時間複雜度：程式的時間複雜度為 Polynomial():O(1)  ~Polynomial():O(k)  operator>>:O(n)  operator<<:O(n)  operator+:O(m+n)   operator-:O(m+n)  operator*:O(m*n)  Evaluate:O(m)  GetNode / RetNode:O(1)。
2. 空間複雜度：空間複雜度為 Polynomial():O(n)  operator+:O(m+n)   operator-:O(m+n)  operator*:O(m*n)  Evaluate:O(1)。

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

1. 程式能正確輸出*P1 P2*分別是多少、*P1+P2,P1-P2*,P1*P2輸出是否正確以及當數值輸入進去(P1(x))也會輸出正確的答案(驗算答案在上面圖片)。  
2. 這個程式主要是利用*Circular Linked List & Header Node*來實作並完成多項式的輸入輸出、加減乘法與數值計算。
3. 這個程式最後還有一個*Available List*來提升記憶體管理的效率，這個是為了避免輸入的變數過大以及程式碼太常導致電腦記憶體不足的部分。 

## 心得討論

##為什麼使用 *operator>>*與*operator<<*
*operator>>*：透過多載輸入運算子，使多項式這種自訂資料型別在使用上能與基本型別保持一致，直接以 cin >> p1  cin >> p2; 的方式讀入資料。此設計將輸入格式的解析（包含各項的係數與指數）集中管理，不僅降低主程式的複雜度，也避免解析規則分散造成的錯誤風險。
*operator<<*：藉由多載輸出運算子，將多項式的顯示格式統一封裝，使主程式只需負責輸出物件本身，而不必關心實際的輸出細節。當輸出格式需要調整時，只需修改 operator<<，即可影響所有輸出結果，可提升程式的可維護性與擴充性。


在本程式中，第二題的實作我並沒有照著題目的要求做的原因是:

1. **不是使用通用 Chain 類別，而是將 Chain 功能內嵌在 Polynomial 中**
   題目第二部分要求實作一個通用的 Chain 類別，是可以讓多項式直接使用該類別來管理節點。
   然而，在實際撰寫 Polynomial 類別時，我選擇不直接使用通用的 Chain 類別，而是將 Chain 的功能內嵌於 Polynomial 中，這是基於設計彈性與實作需求所做的取捨。

2. **為甚麼不使用通用Chain**  
   因為多項式並不是一個「單純的連結串列」，他還需要一些東西。
   a.指數排序的節點存放規則。
   b.多項式加減乘法等專屬運算。
   c.Available List 來進行節點回收與重用。
   若直接使用通用 Chain 類別，勢必需要額外修改或擴充 Chain 的內部結構，反而會讓通用類別變得不再「通用」。
   因此，將鏈結串列的基本功能直接內嵌在 Polynomial 中，可以讓 Polynomial 更貼合多項式的需求。
   
3. **將 Chain 功能內嵌在 Polynomial的優缺點**
   優點:a.設計上完全符合多項式需求、 b.可直接控制節點配置與回收、c.實作加減乘運算時更直觀。
   缺點:a.重複性較高，較不具通用性、b.無法直接用於其他資料結構。

4. **使用通用 Chain 類別的優缺點**
   優點:a.可重複使用，適用於多種資料型別、b.結構清楚，符合資料結構抽象化精神、c.程式模組化程度高。
   缺點:a.對多項式這種「高度客製化」的結構來說較不靈活、b.額外包裝與呼叫會使程式較複雜、c.不易整合 Available List 與特殊運算邏輯。

 **綜合而言，我認為將本次作業選擇將 Chain 功能內嵌於 Polynomial，是一種「以問題導向為主」的設計取向，在可讀性與實作效率上較為合適，同時也是讓自己可以試試看用不同的方式寫出不一樣的程式碼並且答案是相通的**

在本程式中，第三題的實作我並沒有照著題目的要求做的原因是:

1. **第三題沒有實作 Iterator，而是直接使用指標**
    在鏈結串列中，指標本身就已具備 iterator 的核心功能：
    a.可以指向目前節點、b.可以透過link移動到下一個節點。
    我認為在本題目上，多項式的走訪方式單純且較固定，直接使用指標可以讓程式更直觀，也較容易理解與除錯。
    而且依我自己而言我更想使用指標的原因是因為，我在指標的程式很弱沒有學好，所以我想說可以在功課上試試看利用指標達成實作的目的。

2. **使用 Iterator的優缺點**
    優點:a.符合 STL 與物件導向設計風格、b.封裝指標操作，安全性較高、c.程式語意較清楚，可讀性佳。
    缺點:a.實作較繁瑣、b.對本題的效能與功能幫助有限、c.增加額外類別與程式複雜度。
3. **直接使用指標的優缺點**
    優點:a.實作簡單、直觀、b.效能佳，無額外包裝成本、c.易於理解底層資料結構運作方式。
    缺點:a.抽象化程度較低、b.若結構複雜，較容易操作錯誤。

**綜合而言，我認為在本次作業選擇 直接使用指標而不是用Iterator 是因為指標能清楚呈現資料結構的運作原理，因此未特別封裝成 Iterator 類別**

**在第二題和第三題我沒有照著作業的要求實作，但在宣告完ChainNode這個函式的下面的註解裡有題目要求的第二題和第三題的實作**

### 心得
在上學期的課程中，我其實就已經學過 C++ 的指標，也知道指標是用來存放記憶體位址的變數，但在實際寫程式時，卻只知道指標是做甚麼的，不知道要如何去使用。
當老師在本次作業中要求實作多項式並搭配鏈結串列時，我決定嘗試在作業中盡可能使用指標來完成所有功能，讓我對指標的應用和使用上有進一步的理解。
在實作過程中，我逐漸發現指標並不是指是用來儲存另一個變數在記憶體中的位址，而是一種用來「描述資料關係」的重要概念。例如在多項式的實作中，ChainNode<Term>* 這個程式不是只為了存取資料，而是負責將每一項多項式串接起來，來達成完整的資料結構。
此外，在這次作業也讓我學會在什麼情境下應該使用指標。由於多項式的項數在程式執行前並不固定，若使用陣列將會受到大小限制，反而增加設計上的困難；
相較之下，使用指標搭配鏈結串列能夠動態建立與刪除節點，能使資料結構更具彈性。這讓我體會到，指標並不是「一定要用」，而是「在適合的問題中使用才有價值」。
這次使用*指標*讓我對這個程式有更深入的理解，像是*指標*常使用的方式: *ChainNode<Term>* next*，以及該在什麼環境或場景下使用這個程式。
透過這次多項式程式設計作業，我不僅更加熟悉和理解指標的實際應用，也順帶真正理解了循環鏈結串列的運作方式，並進一步認識到什麼是抽象資料型別，也實際比較了不同設計選擇（通用類別 vs. 客製化實作、Iterator vs. 指標）所帶來的影響。
總體而言，這次作業帶給我的收穫不只是完成一份程式或作業，而是讓我理解到在實務程式設計中，並不存在唯一正確的寫法。面對不同的問題會對可讀性、彈性與效能有不同的需求，而程式設計者必須在這些因素之間做出取捨。這樣的思考方式，會讓我對我未來在撰寫更大型或更複雜的程式時，將會是非常重要的基礎。
