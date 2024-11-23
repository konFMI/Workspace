#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <chrono>

using namespace std;

// TODO:
// 1) Add IDA*
// 2) Fix movements, currently not correct order, as per tests expectation.
// 3) Refactor, currently too complex seems.


// Global for direction movements
const int direction[4][2] = {
    {0, -1}, {0, 1},  // left, right
    {-1, 0}, {1, 0}   // up, down
};

// Global for string movements
const string moveNames[4] = {"left", "right", "up", "down"};

struct State {
    vector<vector<int>> board; // Replaced integer board with a 2D vector
    pair<int, int> blankPos;   // Used a pair to track the blank position
    int g;
    int h;
    vector<string> steps;      // Steps taken to reach this state

    bool operator<(const State &other) const {
        return (g + h) > (other.g + other.h);
    }
};

// Function to convert board to a string representation for hashing
string boardToString(const vector<vector<int>> &board) {
    string result;
    for (const auto &row : board) {
        for (int el : row) {
            result += to_string(el);
        }
    }
    return result;
}

// Function to find the blank position (0) in the board
pair<int, int> findBlankPos(const vector<vector<int>> &board) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j] == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// Manhattan distance
int heuristicDistance(const vector<vector<int>> &board, const vector<pair<int, int>> &finalPositions) {
    int distance = 0;
    size_t n = board.size();

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            int tile = board[i][j];
            if (tile != 0) {
                int finalRow = finalPositions[tile].first;
                int finalCol = finalPositions[tile].second;
                distance += abs(((int)i - finalRow)) + abs((int)j - finalCol);
            }
        }
    }
    return distance;
}

// Generate neighbors
vector<State> generateNeighbors(const State &current, const vector<pair<int, int>> &finalPositions) {
    vector<State> neighbors;
    size_t n = current.board.size();
    int x = current.blankPos.first;
    int y = current.blankPos.second;

    for (int i = 0; i < 4; ++i) {
        int nX = x + direction[i][0];
        int nY = y + direction[i][1];

        if (nX >= 0 && nX < n && nY >= 0 && nY < n) {
            vector<vector<int>> newBoard = current.board;
            swap(newBoard[x][y], newBoard[nX][nY]);
            vector<string> newSteps = current.steps;
            newSteps.push_back(moveNames[i]);
            neighbors.push_back({
                newBoard,
                {nX, nY},
                current.g + 1,
                heuristicDistance(newBoard, finalPositions),
                newSteps
            });
        }
    }

    return neighbors;
}

// A* algorithm
pair<int, vector<string>>  aStar(vector<vector<int>> startBoard, const vector<vector<int>> &goalBoard) {
    size_t n = startBoard.size();
    vector<pair<int, int>> finalPositions(n * n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            finalPositions[goalBoard[i][j]] = {i, j};
        }
    }

    auto cmp = [](const State &a, const State &b) { return a < b; };
    priority_queue<State, vector<State>, decltype(cmp)> pq(cmp);
    unordered_set<string> visited;
    pair<int, int> startBlankPos = findBlankPos(startBoard);
    pq.push({
        startBoard,
        startBlankPos,
        0,
        heuristicDistance(startBoard, finalPositions),
        {} // Initial empty steps
    });
    visited.insert(boardToString(startBoard));

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.board == goalBoard) {
            return {current.g, current.steps};
        }

        for (const auto &neighbor : generateNeighbors(current, finalPositions)) {
            string boardStr = boardToString(neighbor.board);
            if (visited.find(boardStr) == visited.end()) {
                visited.insert(boardStr);
                pq.push(neighbor);
            }
        }
    }

    return {-1, {}};
}

int main() {
    vector<vector<int>> startBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };

    vector<vector<int>> goalBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    auto startTime = chrono::high_resolution_clock::now();
    pair<int, vector<string>> result = aStar(startBoard, goalBoard);
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;

    cout << "Steps to solve: " << result.first << endl;
    cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    cout << "Solution path: ";
    for (const auto &step : result.second) {
        cout << step << " ";
    }

    return 0;
}
