#include <algorithm>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>
#include <utility>

using namespace std;

// Задание 1. Рекурсивный факториал.
unsigned long long factorial(unsigned int n) {
    if (n <= 1) return 1;  // базовый случай
    if (n > 20) throw overflow_error("Factorial does not fit in unsigned long long");
    return n * factorial(n - 1);
}

// Задание 2. Рекурсивное вычисление числа Фибоначчи.
unsigned long long fibonacci(unsigned int n) {
    if (n <= 1) return n;  // базовый случай
    if (n > 93) throw overflow_error("Fibonacci does not fit in unsigned long long");
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Задание 3. Определитель матрицы 3x3 разложением по первой строке.
using Matrix3 = vector<vector<long long>>;

long long determinant2x2(long long a, long long b, long long c, long long d) {
    return a * d - b * c;
}

long long determinant3x3(const Matrix3& a) {
    if (a.size() != 3 || a[0].size() != 3 || a[1].size() != 3 || a[2].size() != 3)
        throw invalid_argument("Expected a 3x3 matrix");

    // Рекурсивное разложение определителя: миноры 2x2 вычисляются через базовый случай.
    auto det = [&](auto&& self, const vector<vector<long long>>& m) -> long long {
        int n = static_cast<int>(m.size());
        if (n == 1) return m[0][0];  // базовый случай
        if (n == 2) return determinant2x2(m[0][0], m[0][1], m[1][0], m[1][1]);
        long long result = 0;
        for (int col = 0; col < n; ++col) {
            vector<vector<long long>> minor;
            for (int row = 1; row < n; ++row) {
                vector<long long> line;
                for (int j = 0; j < n; ++j)
                    if (j != col) line.push_back(m[row][j]);
                minor.push_back(line);
            }
            result += (col % 2 == 0 ? 1 : -1) * m[0][col] * self(self, minor);
        }
        return result;
    };
    return det(det, a);
}

// Задание 4. Поиск КРАТЧАЙШЕГО пути в лабиринте рекурсивным DFS с возвратом.
// '.' — свободная клетка, '#' — стена, S — старт, F — финиш.
class MazeSolver {
    vector<string> maze;
    vector<vector<bool>> used;
    vector<pair<int, int>> currentPath;
    vector<pair<int, int>> bestPath;
    int rows, cols;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    void dfs(int x, int y) {
        if (x < 0 || x >= rows || y < 0 || y >= cols ||
            maze[x][y] == '#' || used[x][y]) return;
        // Отсекаем путь, который уже не может быть лучше найденного.
        if (!bestPath.empty() && currentPath.size() + 1 >= bestPath.size()) return;

        used[x][y] = true;
        currentPath.push_back({x, y});
        if (maze[x][y] == 'F') {
            bestPath = currentPath;
        } else {
            for (int d = 0; d < 4; ++d)
                dfs(x + dx[d], y + dy[d]);
        }
        currentPath.pop_back();
        used[x][y] = false;  // backtracking: разрешаем клетку для других путей
    }

public:
    explicit MazeSolver(vector<string> grid)
        : maze(move(grid)), rows(static_cast<int>(maze.size())),
          cols(rows ? static_cast<int>(maze[0].size()) : 0) {
        if (rows == 0 || cols == 0) throw invalid_argument("Empty maze");
        for (const auto& line : maze)
            if (static_cast<int>(line.size()) != cols)
                throw invalid_argument("Maze must be rectangular");
        used.assign(rows, vector<bool>(cols, false));
    }

    bool solve() {
        int sx = -1, sy = -1, starts = 0, finishes = 0;
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j) {
                if (maze[i][j] == 'S') { sx = i; sy = j; ++starts; }
                if (maze[i][j] == 'F') ++finishes;
            }
        if (starts != 1 || finishes != 1)
            throw invalid_argument("Maze needs exactly one S and one F");
        bestPath.clear();
        currentPath.clear();
        for (auto& line : used)
            fill(line.begin(), line.end(), false);
        dfs(sx, sy);
        return !bestPath.empty();
    }

    void printResult() const {
        if (bestPath.empty()) {
            cout << "No path found\n";
            return;
        }
        cout << "Shortest path length: " << bestPath.size() - 1 << " steps\n";
        auto out = maze;
        for (auto [x, y] : bestPath)
            if (out[x][y] == '.') out[x][y] = '*';
        for (const auto& line : out) cout << line << '\n';
    }
};

int main() {
    try {
        cout << "=== Part 1: Recursion, variant 14 ===\n";
        cout << "factorial(5) = " << factorial(5) << '\n';
        cout << "fibonacci(10) = " << fibonacci(10) << '\n';
        Matrix3 m = {{1, 2, 3}, {0, 4, 5}, {1, 0, 6}};
        cout << "determinant 3x3 = " << determinant3x3(m) << '\n';
        cout << "\nMaze (S=start, F=finish, *=path):\n";
        MazeSolver maze({
            "S.#..",
            "#.#.#",
            "...#.",
            "##...",
            "..#.F"
        });
        if (maze.solve()) maze.printResult();
        else cout << "No path found\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
