#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Vector {
    T* data;
    size_t sz, cap;
public:
    Vector() {
        data = nullptr;
        sz = 0;
        cap = 0;
    }

    // 1. Деструктор
    ~Vector() { delete[] data; }

    // 2. Конструктор копирования
    Vector(const Vector& o) {
        sz = o.sz;
        cap = o.cap;
        data = new T[cap];
        for (size_t i = 0; i < sz; i++) data[i] = o.data[i];
    }

    // 3. Копирующее присваивание
    Vector& operator=(const Vector& o) {
        if (this == &o) return *this;
        delete[] data;
        sz = o.sz;
        cap = o.cap;
        data = new T[cap];
        for (size_t i = 0; i < sz; i++) data[i] = o.data[i];
        return *this;
    }

    // 4. Конструктор перемещения
    Vector(Vector&& o) noexcept {
        data = o.data;
        sz = o.sz;
        cap = o.cap;
        o.data = nullptr;
        o.sz = 0;
        o.cap = 0;
    }

    // 5. Перемещающее присваивание
    Vector& operator=(Vector&& o) noexcept {
        if (this == &o) return *this;
        delete[] data;
        data = o.data;
        sz = o.sz;
        cap = o.cap;
        o.data = nullptr;
        o.sz = 0;
        o.cap = 0;
        return *this;
    }

    void push_back(const T& v) {
        if (sz == cap) {
            if (cap == 0) cap = 1;
            else cap = cap * 2;

            T* nd = new T[cap];
            for (size_t i = 0; i < sz; i++) nd[i] = data[i];
            delete[] data;
            data = nd;
        }
        data[sz] = v;
        sz++;
    }

    size_t size() const { return sz; }
    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }

    // Слияние двух отсортированных векторов
    Vector merge(const Vector& other) const {
        Vector res;
        size_t i = 0, j = 0;
        while (i < sz && j < other.sz) {
            if (data[i] <= other.data[j]) {
                res.push_back(data[i]);
                i++;
            } else {
                res.push_back(other.data[j]);
                j++;
            }
        }
        while (i < sz) {
            res.push_back(data[i]);
            i++;
        }
        while (j < other.sz) {
            res.push_back(other.data[j]);
            j++;
        }
        return res;
    }

    void print() const {
        for (size_t i = 0; i < sz; i++) cout << data[i] << " ";
        cout << "\n";
    }
};

int main() {
    Vector<int> a, b;
    for (int x : {1, 3, 5, 7}) a.push_back(x);
    for (int x : {2, 4, 6, 8, 10}) b.push_back(x);

    Vector<int> c = a.merge(b);
    c.print();                 // 1 2 3 4 5 6 7 8 10

    Vector<int> d = c;         // копирование
    Vector<int> e = move(c);   // перемещение
    d.print();
    e.print();
    cout << "c after move: " << c.size() << "\n";   // 0

    Vector<string> s1, s2;
    s1.push_back("apple");
    s1.push_back("cherry");
    s2.push_back("banana");
    s2.push_back("date");
    Vector<string> s3 = s1.merge(s2);
    s3.print();                // apple banana cherry date
}