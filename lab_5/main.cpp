#include <iostream>
#include <cmath>

using namespace std; 

class Vector3D {
private:
    double x, y, z; // Координаты вектора

public:

    // 1. Конструктор по умолчанию
    Vector3D() : x(0), y(0), z(0) {}

    // 2. Параметризованный конструктор
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    // 3. Конструктор копирования
    Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z) {}

    // 4. Деструктор
    ~Vector3D() {}

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    void setZ(double newZ) { z = newZ; }


    // Скалярное произведение
    double dotProduct(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Векторное произведение
    Vector3D crossProduct(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }


    // Перезагрузка оператров ввода и вывода 

    // Вывод в поток
    friend ostream& operator<<(ostream& os, const Vector3D& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    // Ввод из потока
    friend istream& operator>>(istream& is, Vector3D& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }


    // Перезагрузка оператров сравнения

    bool operator==(const Vector3D& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    bool operator!=(const Vector3D& other) const {
        return !(*this == other);
    }

    // Сравнение по длине вектора
    bool operator<(const Vector3D& other) const {
        return (x*x + y*y + z*z) < (other.x*other.x + other.y*other.y + other.z*other.z);
    }

    bool operator>(const Vector3D& other) const {
        return other < *this;
    }

    bool operator<=(const Vector3D& other) const {
        return !(other < *this);
    }

    bool operator>=(const Vector3D& other) const {
        return !(*this < other);
    }


    // Перегрузка арифметический операторов

    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D operator/(double scalar) const {
        if (scalar == 0) throw invalid_argument("Division by zero!");
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }


    // Перезагрузка составного присваивания

    Vector3D& operator+=(const Vector3D& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vector3D& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vector3D& operator/=(double scalar) {
        if (scalar == 0) throw invalid_argument("Division by zero!");
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }


    // Перезагрузка инкремента и декремента

    // Префиксный инкремент (++v)
    Vector3D& operator++() {
        ++x; ++y; ++z;
        return *this;
    }

    // Постфиксный инкремент (v++)
    Vector3D operator++(int) {
        Vector3D temp(*this);
        ++(*this);
        return temp;
    }

    // Префиксный декремент (--v)
    Vector3D& operator--() {
        --x; --y; --z;
        return *this;
    }

    // Постфиксный декремент (v--)
    Vector3D operator--(int) {
        Vector3D temp(*this);
        --(*this);
        return temp;
    }


    // Перегрузка оператора ()(функтор)

    double operator()(int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw out_of_range("Index must be 0, 1, or 2");
    }
};

// Пример использования
int main() {
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);

    cout << "v1 = " << v1 << "\nv2 = " << v2 << "\n\n";

    // Арифметика
    cout << "v1 + v2 = " << (v1 + v2) << "\n";
    cout << "v1 * 2 = " << (v1 * 2) << "\n\n";

    // Вариант 4
    cout << "skalyarnoe proizvedenie: " << v1.dotProduct(v2) << "\n";
    cout << "vectornoe proizvedenie: " << v1.crossProduct(v2) << "\n\n";

    // Сравнение (по длине)
    cout << "v1 < v2? " << (v1 < v2 ? "Yes" : "No") << "\n";
    cout << "v1 == v2? " << (v1 == v2 ? "Yes" : "No") << "\n\n";

    // Инкремент и оператор ()
    v1++;
    cout << "v1 after v1++: " << v1 << "\n";
    cout << "coordinate v1(0) [X]: " << v1(0) << "\n";

    return 0;
}
