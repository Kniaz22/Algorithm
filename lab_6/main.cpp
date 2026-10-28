#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// Блюдо, композиция
class Dish {
    string name;
    double price;

public:
    Dish(const string& name, double price) : name(name), price(price) {}

    void print() const {
        cout << "  " << name << " - " << price << " руб." << endl;
    }
};

// Заказ владеет блюдами, композиция
class Order {
    int number;
    vector<Dish> dishes;

public:
    inline static int count = 0; // счётчик живых заказов

    explicit Order(int number) : number(number) {
        ++count;
        cout << "Создан заказ №" << number << endl;
    }

    ~Order() {
        --count;
        cout << "Удалён заказ №" << number << endl;
    }

    // Копирование запрещено, иначе счётчик собьётся
    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    void addDish(const string& name, double price) {
        dishes.emplace_back(name, price);
    }

    void print() const {
        cout << "Заказ №" << number << ":" << endl;
        for (const Dish& dish : dishes) {
            dish.print();
        }
    };
};

// Ресторан - оидночка, агрегация
class Restaurant {
    vector<Order*> orders;

    Restaurant() { cout << "Ресторан открыт" << endl; }

public:
    Restaurant(const Restaurant&) = delete;
    Restaurant& operator=(const Restaurant&) = delete;
    ~Restaurant() { cout << "Ресторан закрыт" << endl; }

    static Restaurant& getInstance() {
        static Restaurant instance; // создаётся один раз, удаляется сам
        return instance;
    }

    void addOrder(Order& order) { orders.push_back(&order); }
    void clearOrders() { orders.clear(); } // заказы не удаляются

    void printOrders() const {
        cout << "\nЗаказы ресторана:" << endl;
        for (const auto* o : orders) o->print();
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Singleton: обе ссылки указывают на один объект
    Restaurant& r1 = Restaurant::getInstance();
    Restaurant& r2 = Restaurant::getInstance();
    cout << "Один и тот же ресторан: " << (&r1 == &r2 ? "Да" : "Нет") << endl;

    {
        Order order1(1);
        order1.addDish("Пицца", 650);
        order1.addDish("Сок", 150);

        Order order2(2);
        order2.addDish("Бургер", 350);
        order2.addDish("Картофель фри", 180);

        r1.addOrder(order1);
        r1.addOrder(order2);
        r1.printOrders();
        cout << "\nЗаказов: " << Order::count << endl;

        // Агрегация: ресторан "забыл" заказы, но они всё ещё существуют
        r1.clearOrders();
        cout << "После clearOrders заказов: " << Order::count << endl;
    } // здесь заказы удаляются вместе со своими блюдами (композиция)

    cout << "После выхода из блока заказов: " << Order::count << endl;
}