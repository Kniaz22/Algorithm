#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Restore max-heap property iteratively (without recursion).
void heapify(int arr[], int n, int root) {
    int i = root;
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[left] > arr[largest]) largest = left;
        if (right < n && arr[right] > arr[largest]) largest = right;
        if (largest == i) break;
        swap(arr[i], arr[largest]);
        i = largest;
    }
}

// In-place heap sort, O(n log n) time, O(1) extra memory.
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int end = n - 1; end > 0; --end) {
        swap(arr[0], arr[end]);
        heapify(arr, end, 0);
    }
}

void printArray(const vector<int>& a) {
    cout << "[";
    for (size_t i = 0; i < a.size(); ++i) {
        if (i) cout << ", ";
        cout << a[i];
    }
    cout << "]\n";
}

bool test(vector<int> a, const char* name) {
    vector<int> expected = a;
    sort(expected.begin(), expected.end());
    cout << name << " before: "; printArray(a);
    heapSort(a.data(), static_cast<int>(a.size()));
    cout << name << " after:  "; printArray(a);
    bool ok = (a == expected);
    cout << "Check: " << (ok ? "OK" : "FAIL") << "\n\n";
    return ok;
}

int main() {
    bool ok = true;
    ok &= test({4, 10, 3, 5, 1}, "Example");
    ok &= test({1, 2, 3, 4, 5}, "Sorted");
    ok &= test({5, 4, 3, 2, 1}, "Reverse");
    ok &= test({3, -1, 3, 0, -1}, "Duplicates");
    ok &= test({}, "Empty");
    ok &= test({42}, "Single");

}