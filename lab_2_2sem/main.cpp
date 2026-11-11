
#include <iostream>
#include <memory>
#include <chrono>
using namespace std;
using namespace chrono;

// Первая реализация unique_ptr
template <typename T>
class LazyUnique {
private:
    unique_ptr<T> value;

public:
    T& get() {
        if (value == nullptr) {
            value = make_unique<T>();
        }
        return *value;
    }

    bool isInitialized() const {
        return value != nullptr;
    }

    void reset() {
        value.reset();
    }
};

// Вторая реализация: обычный указатель
template <typename T>
class LazyRaw {
private:
    T* value;

public:
    LazyRaw() {
        value = nullptr;
    }

    ~LazyRaw() {
        delete value;
    }

    // Запрещаем копирование
    LazyRaw(const LazyRaw&) = delete;
    LazyRaw& operator=(const LazyRaw&) = delete;

    T& get() {
        if (value == nullptr) {
            value = new T();
        }
        return *value;
    }

    bool isInitialized() const {
        return value != nullptr;
    }

    void reset() {
        delete value;
        value = nullptr;
    }
};

int main() {
    const int N = 1000000;
    volatile long long result = 0;

    // Проверка первой реализации
    auto start1 = steady_clock::now();

    for (int i = 0; i < N; i++) {
        LazyUnique<int> number;

        if (!number.isInitialized()) {
            number.get() = i;
        }

        result += number.get();
        number.reset();
    }

    auto end1 = steady_clock::now();

    // Проверка второй реализации
    auto start2 = steady_clock::now();

    for (int i = 0; i < N; i++) {
        LazyRaw<int> number;

        if (!number.isInitialized()) {
            number.get() = i;
        }

        result += number.get();
        number.reset();
    }

    auto end2 = steady_clock::now();

    // Вычисление времени
    auto time1 = duration_cast<milliseconds>(end1 - start1);
    auto time2 = duration_cast<milliseconds>(end2 - start2);

    cout << "LazyUnique (unique_ptr): "
         << time1.count() << " ms" << endl;

    cout << "LazyRaw (T*): "
         << time2.count() << " ms" << endl;

    cout << "Result: " << result << endl;

    return 0;
}
