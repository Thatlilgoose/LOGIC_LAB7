#include <iostream>
#include <limits>
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

std::vector<int> traverseMatrixRecursive(const Matrix& matrix, int start) {
    std::vector<bool> visited(matrix.size(), false);
    std::vector<int> order;

    dfsMatrixRecursive(matrix, start, visited, order);
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

std::vector<int> traverseListRecursive(const AdjacencyList& list, int start) {
    std::vector<bool> visited(list.size(), false);
    std::vector<int> order;

    dfsListRecursive(list, start, visited, order);
    for (int vertex = 0; vertex < static_cast<int>(list.size()); ++vertex) {
        if (!visited[vertex]) {
            dfsListRecursive(list, vertex, visited, order);
        }
    }

    return order;
}

std::vector<int> traverseMatrixIterative(const Matrix& matrix, int start) {
    std::vector<bool> visited(matrix.size(), false);
    std::vector<int> order;

    std::vector<int> starts{start};
    for (int vertex = 0; vertex < static_cast<int>(matrix.size()); ++vertex) {
        if (vertex != start) {
            starts.push_back(vertex);
        }
    }

    for (int componentStart : starts) {
        if (visited[componentStart]) {
            continue;
        }

        std::vector<int> stack{componentStart};

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
    while (true) {
        std::cout << "Введите количество вершин: ";
        if (std::cin >> vertexCount && vertexCount > 0) {
            break;
        }
        std::cerr << "Количество вершин должно быть положительным числом.\n";
        if (std::cin.eof()) {
            return 1;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    double edgeProbability;
    while (true) {
        std::cout << "Введите вероятность появления ребра (от 0 до 1): ";
        if (std::cin >> edgeProbability &&
            edgeProbability >= 0.0 && edgeProbability <= 1.0) {
            break;
        }
        std::cerr << "Вероятность должна быть числом от 0 до 1.\n";
        if (std::cin.eof()) {
            return 1;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    Matrix matrix = generateMatrix(vertexCount, edgeProbability);
    AdjacencyList list = makeAdjacencyList(matrix);

    std::cout << "\nМатрица смежности:\n";
    printMatrix(matrix);

    int startVertex;
    while (true) {
        std::cout << "Введите начальную вершину обхода (от 1 до " << vertexCount << "): ";
        if (!(std::cin >> startVertex)) {
            std::cerr << "Введите целый номер вершины от 1 до " << vertexCount << ".\n";
            if (std::cin.eof()) {
                return 1;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (startVertex < 1 || startVertex > vertexCount) {
            std::cerr << "Номер вершины должен быть от 1 до " << vertexCount << ".\n";
            continue;
        }

        bool isolated = true;
        for (int neighbour = 0; neighbour < vertexCount; ++neighbour) {
            if (matrix[startVertex - 1][neighbour] == 1) {
                isolated = false;
                break;
            }
        }
        if (!isolated) {
            break;
        }
        std::cout << "Вершина " << startVertex
                  << " изолирована. Выберите другую вершину.\n";
    }
    const int start = startVertex - 1;

    std::cout << "\nОбход DFS рекурсивно по матрице:\n";
    printOrder(traverseMatrixRecursive(matrix, start));

    std::cout << "\nОбход DFS рекурсивно по спискам смежности:\n";
    printOrder(traverseListRecursive(list, start));

    std::cout << "\nОбход DFS нерекурсивно по матрице:\n";
    printOrder(traverseMatrixIterative(matrix, start));

    return 0;
}
