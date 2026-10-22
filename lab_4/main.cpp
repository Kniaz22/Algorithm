#include <iostream>
using namespace std;

class IntVector {
private:
    int* data;
    int size;
    int capacity;

public:
    // Конструктор
    IntVector(int n = 0) {
        size = n;
        capacity = n;

        if (capacity > 0)
            data = new int[capacity];
        else
            data = nullptr;
    }

    // Конструктор копирования
    IntVector(const IntVector& other) {
        size = other.size;
        capacity = other.capacity;

        if (capacity > 0) {
            data = new int[capacity];

            for (int i = 0; i < size; i++)
                data[i] = other.data[i];
        } else {
            data = nullptr;
        }
    }

    // Оператор присваивания
    IntVector& operator=(const IntVector& other) {
        if (this != &other) {
            int* newData = nullptr;

            if (other.capacity > 0) {
                newData = new int[other.capacity];

                for (int i = 0; i < other.size; i++)
                    newData[i] = other.data[i];
            }

            delete[] data;

            data = newData;
            size = other.size;
            capacity = other.capacity;
        }

        return *this;
    }

    // Деструктор
    ~IntVector() {
        delete[] data;
    }

    // Добавление элемента в конец
    void push_back(int value) {
        if (size == capacity) {
            int newCapacity;

            if (capacity == 0)
                newCapacity = 1;
            else
                newCapacity = capacity * 2;

            int* newData = new int[newCapacity];

            for (int i = 0; i < size; i++)
                newData[i] = data[i];

            delete[] data;

            data = newData;
            capacity = newCapacity;
        }

        data[size] = value;
        size++;
    }

    // Ввод элементов
    void input() {
        for (int i = 0; i < size; i++)
            cin >> data[i];
    }

    // Вывод элементов
    void print() const {
        for (int i = 0; i < size; i++)
            cout << data[i] << " ";

        cout << endl;
    }

    // Минимальный элемент
    int min() const {
        int result = data[0];

        for (int i = 1; i < size; i++) {
            if (data[i] < result)
                result = data[i];
        }

        return result;
    }

    // Максимальный элемент
    int max() const {
        int result = data[0];

        for (int i = 1; i < size; i++) {
            if (data[i] > result)
                result = data[i];
        }

        return result;
    }

    // Получение размера
    int getSize() const {
        return size;
    }

    // Получение вместимости
    int getCapacity() const {
        return capacity;
    }
};

int main() {
    int n;

    cout << "Enter vector size: ";
    cin >> n;

    if (n <= 0) {
        cout << "Size must be positive." << endl;
        return 0;
    }

    IntVector v(n);

    cout << "Enter vector elements: ";
    v.input();

    cout << "Vector: ";
    v.print();

    cout << "Size: " << v.getSize() << endl;
    cout << "Capacity: " << v.getCapacity() << endl;

    cout << "Minimum element: " << v.min() << endl;
    cout << "Maximum element: " << v.max() << endl;

    // Добавление нового элемента
    int value;

    cout << "Enter new element: ";
    cin >> value;

    v.push_back(value);

    cout << "Vector after push_back: ";
    v.print();

    cout << "Size: " << v.getSize() << endl;
    cout << "Capacity: " << v.getCapacity() << endl;

    return 0;
}
