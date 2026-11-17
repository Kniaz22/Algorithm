#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Задание 1. Пузырьковая сортировка с досрочным завершением.
void bubbleSort(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// Задание 2. Сортировка вставками.
void insertionSort(vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

// Задание 3. Сортировка выбором.
void selectionSort(vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); ++j)
            if (arr[j] < arr[minIndex]) minIndex = j;
        if (minIndex != i) swap(arr[i], arr[minIndex]);
    }
}

// Задание 4. Сравнение на ОДНИХ И ТЕХ ЖЕ исходных данных.
using SortFunction = void (*)(vector<int>&);

double measureMs(SortFunction sort, const vector<int>& input,
                 const vector<int>& expected, int repeats) {
    double total = 0.0;
    for (int trial = 0; trial < repeats; ++trial) {
        vector<int> copy = input;  // копирование не входит во время сортировки
        const auto start = chrono::steady_clock::now();
        sort(copy);
        const auto finish = std::chrono::steady_clock::now();
        if (copy != expected) throw runtime_error("Sorting result is incorrect");
        total += chrono::duration<double, milli>(finish - start).count();
    }
    return total / repeats;
}

int main() {
    constexpr int N = 3000;
    constexpr int REPEATS = 3;
    mt19937 gen(14);  // фиксированный seed для воспроизводимости
    uniform_int_distribution<int> dist(-10000, 10000);

    vector<int> input(N);
    for (int& value : input) value = dist(gen);

    // sort — эталон для проверки правильности и сравнения скорости.
    vector<int> expected = input;
    sort(expected.begin(), expected.end());
    auto standardSort = [](vector<int>& a) { sort(a.begin(), a.end()); };

    cout << "=== Part 2: Sorting, variant 14 ===\n";
    cout << "N = " << N << ", runs = " << REPEATS << " (average ms)\n";
    cout << fixed << setprecision(3);
    cout << "Bubble sort:    " << measureMs(bubbleSort, input, expected, REPEATS) << " ms\n";
    cout << "Insertion sort: " << measureMs(insertionSort, input, expected, REPEATS) << " ms\n";
    cout << "Selection sort: " << measureMs(selectionSort, input, expected, REPEATS) << " ms\n";
    cout << "sort:      " << measureMs(standardSort, input, expected, REPEATS) << " ms\n";
    cout << "All results match sort.\n";
}
