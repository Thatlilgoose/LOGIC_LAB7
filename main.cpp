#include <iostream>
#include <random>
#include <vector>

using Matrix = std::vector<std::vector<int>>;
using AdjacencyList = std::vector<std::vector<int>>;

Matrix generateMatrix(int vertexCount, double edgeProbability) {
    Matrix matrix(vertexCount, std::vector<int>(vertexCount, 0));
    std::random_device device;
    std::mt19937 generator(device());
    std::bernoulli_distribution edge(edgeProbability);

    for (int i = 0; i < vertexCount; ++i) {
        for (int j = i + 1; j < vertexCount; ++j) {
            matrix[i][j] = matrix[j][i] = edge(generator);
        }
    }

    return matrix;
}

void printMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
}

void dfsMatrixRecursive(const Matrix& matrix, int vertex, std::vector<bool>& visited,
                        std::vector<int>& order) {
    visited[vertex] = true;
    order.push_back(vertex);

    for (int neighbour = 0; neighbour < static_cast<int>(matrix.size()); ++neighbour) {
        if (matrix[vertex][neighbour] == 1 && !visited[neighbour]) {
            dfsMatrixRecursive(matrix, neighbour, visited, order);
        }
    }
}

std::vector<int> traverseMatrixRecursive(const Matrix& matrix) {
    std::vector<bool> visited(matrix.size(), false);
    std::vector<int> order;

    for (int vertex = 0; vertex < static_cast<int>(matrix.size()); ++vertex) {
        if (!visited[vertex]) {
            dfsMatrixRecursive(matrix, vertex, visited, order);
        }
    }

    return order;
}

AdjacencyList makeAdjacencyList(const Matrix& matrix) {
    AdjacencyList list(matrix.size());

    for (int vertex = 0; vertex < static_cast<int>(matrix.size()); ++vertex) {
        for (int neighbour = 0; neighbour < static_cast<int>(matrix.size()); ++neighbour) {
            if (matrix[vertex][neighbour] == 1) {
                list[vertex].push_back(neighbour);
            }
        }
    }

    return list;
}

void dfsListRecursive(const AdjacencyList& list, int vertex, std::vector<bool>& visited,
                      std::vector<int>& order) {
    visited[vertex] = true;
    order.push_back(vertex);

    for (int neighbour : list[vertex]) {
        if (!visited[neighbour]) {
            dfsListRecursive(list, neighbour, visited, order);
        }
    }
}

std::vector<int> traverseListRecursive(const AdjacencyList& list) {
    std::vector<bool> visited(list.size(), false);
    std::vector<int> order;

    for (int vertex = 0; vertex < static_cast<int>(list.size()); ++vertex) {
        if (!visited[vertex]) {
            dfsListRecursive(list, vertex, visited, order);
        }
    }

    return order;
}

std::vector<int> traverseMatrixIterative(const Matrix& matrix) {
    std::vector<bool> visited(matrix.size(), false);
    std::vector<int> order;

    for (int start = 0; start < static_cast<int>(matrix.size()); ++start) {
        if (visited[start]) {
            continue;
        }

        std::vector<int> stack{start};

        while (!stack.empty()) {
            int vertex = stack.back();
            stack.pop_back();

            if (visited[vertex]) {
                continue;
            }

            visited[vertex] = true;
            order.push_back(vertex);

            for (int neighbour = static_cast<int>(matrix.size()) - 1; neighbour >= 0;
                 --neighbour) {
                if (matrix[vertex][neighbour] == 1 && !visited[neighbour]) {
                    stack.push_back(neighbour);
                }
            }
        }
    }

    return order;
}

void printOrder(const std::vector<int>& order) {
    for (int vertex : order) {
        std::cout << vertex + 1 << ' ';
    }
    std::cout << '\n';
}

int main() {
    int vertexCount;
    std::cout << "Введите количество вершин: ";
    std::cin >> vertexCount;

    if (!std::cin || vertexCount <= 0) {
        std::cerr << "Количество вершин должно быть положительным числом.\n";
        return 1;
    }

    Matrix matrix = generateMatrix(vertexCount, 0.35);
    AdjacencyList list = makeAdjacencyList(matrix);

    std::cout << "\nМатрица смежности:\n";
    printMatrix(matrix);

    std::cout << "\nОбход DFS рекурсивно по матрице:\n";
    printOrder(traverseMatrixRecursive(matrix));

    std::cout << "\nОбход DFS рекурсивно по спискам смежности:\n";
    printOrder(traverseListRecursive(list));

    std::cout << "\nОбход DFS нерекурсивно по матрице:\n";
    printOrder(traverseMatrixIterative(matrix));

    return 0;
}
