#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
using namespace std;

// Превый индекс, числа больше заданного
int searchUpperBound(const vector<int>& a, int target) {
    auto it = upper_bound(a.begin(), a.end(), target);
    return it == a.end() ? -1 : static_cast<int>(it - a.begin());
}

// бинарный поиск
int binarySearch(const vector<int>& a, int target) {
    int left = 0, right = static_cast<int>(a.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

class HashTable {
    struct Entry {
        int key = 0;
        int value = 0;
        bool occupied = false;
    };
    vector<Entry> table;
    int count = 0;

    int indexFor(int key) const {
        // Multiplicative mixing, works for negative keys as well.
        unsigned int x = static_cast<unsigned int>(key);
        x ^= x >> 16;
        x *= 0x7feb352dU;
        x ^= x >> 15;
        x *= 0x846ca68bU;
        x ^= x >> 16;
        return static_cast<int>(x % table.size());
    }
    // Добавление без разширения
    void putWithoutResize(int key, int value) {
        int start = indexFor(key);
        for (int i = 0; i < static_cast<int>(table.size()); ++i) {
            int pos = (start + i) % static_cast<int>(table.size());
            if (!table[pos].occupied) {
                table[pos] = {key, value, true};
                ++count;
                return;
            }
            if (table[pos].key == key) {
                table[pos].value = value;
                return;
            }
        }
    }

    // Разширение таблицы 
    void rehash(int newCapacity) {
        vector<Entry> old = move(table);
        table.resize(newCapacity);
        count = 0;
        for (const Entry& e : old)
            if (e.occupied) putWithoutResize(e.key, e.value);
    }

public:
    explicit HashTable(int capacity = 8) : table(max(8, capacity)) {}

    // Добавление
    void insert(int key, int value) {
        if ((count + 1) * 10 >= static_cast<int>(table.size()) * 7)
            rehash(static_cast<int>(table.size()) * 2);
        putWithoutResize(key, value);
    }

    // Поиск по ключу
    bool find(int key, int& value) const {
        int start = indexFor(key);
        for (int i = 0; i < static_cast<int>(table.size()); ++i) {
            int pos = (start + i) % static_cast<int>(table.size());
            if (!table[pos].occupied) return false;
            if (table[pos].key == key) {
                value = table[pos].value;
                return true;
            }
        }
        return false;
    }

    // Удаление
    bool remove(int key) {
        int start = indexFor(key);
        int capacity = static_cast<int>(table.size());
        for (int i = 0; i < capacity; ++i) {
            int pos = (start + i) % capacity;
            if (!table[pos].occupied) return false;
            if (table[pos].key == key) {
                table[pos].occupied = false;
                --count;
                int next = (pos + 1) % capacity;
                while (table[next].occupied) {
                    Entry e = table[next];
                    table[next].occupied = false;
                    --count;
                    putWithoutResize(e.key, e.value);
                    next = (next + 1) % capacity;
                }
                return true;
            }
        }
        return false;
    }

    int size() const { return count; }
    int capacity() const { return static_cast<int>(table.size()); }
};

int main() {
    cout << "PART 1: upper_bound\n";
    vector<int> a = {1, 3, 3, 5, 7, 9};
    for (int key : {0, 3, 5, 9}) {
        int pos = searchUpperBound(a, key);
        cout << "target=" << key << ", first greater index=" << pos;
        if (pos != -1) cout << ", value=" << a[pos];
        cout << '\n';
    }

    cout << "\nPART 2: hash table\n";
    HashTable ht(8);
    for (int i = 0; i < 20; ++i) ht.insert(i, i * 10);
    int value = 0;
    bool found = ht.find(12, value);
    cout << "find(12): " << (found ? "FOUND" : "NOT FOUND");
    if (found) cout << ", value=" << value;
    cout << '\n';
    ht.insert(12, 999);
    ht.find(12, value);
    cout << "after update: " << value << '\n';
    cout << "remove(12): " << (ht.remove(12) ? "OK" : "FAIL") << '\n';
    cout << "find(12) after remove: " << (ht.find(12, value) ? "FOUND" : "NOT FOUND") << '\n';
    cout << "size=" << ht.size() << ", capacity=" << ht.capacity() << '\n';

    cout << "\nPART 3: search performance\n";
    cout << "N | binary search (us) | hash search (us)\n";
    volatile long long checksum = 0; 
    for (int n : {1000, 10000, 100000}) {
        vector<int> data(n);
        HashTable hash(n);
        for (int i = 0; i < n; ++i) {
            data[i] = i * 2;
            hash.insert(i * 2, i);
        }
        const int queries = 100000;
        auto start = chrono::steady_clock::now();
        for (int i = 0; i < queries; ++i)
            checksum += binarySearch(data, (i % n) * 2);
        auto middle = chrono::steady_clock::now();
        for (int i = 0; i < queries; ++i) {
            int result;
            if (hash.find((i % n) * 2, result)) checksum += result;
        }
        auto finish = chrono::steady_clock::now();
        auto binaryUs = chrono::duration_cast<chrono::microseconds>(middle - start).count();
        auto hashUs = chrono::duration_cast<chrono::microseconds>(finish - middle).count();
        cout << n << " | " << binaryUs << " | " << hashUs << '\n';
    }
    cout << "checksum=" << checksum << '\n';
    return 0;
}
