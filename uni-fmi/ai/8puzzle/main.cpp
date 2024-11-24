#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

// TODO:
// 1) Add IDA*
// 3) Refactor, currently too complex seems.


static const int MaxDirections = 4;
static const pair<int, int> direction[MaxDirections] = {
    {0, -1}, {0, 1},  // left, right
    {-1, 0}, {1, 0}   // up, down
};

// We will use a vector of points to hold the [x,y] of each number.
// 1 -> [0, 0]
// 2 -> [0, 1]
// ...
// n - 1 -> [n - 1][n - 2]
// NOTE: We may rearange this based on the 0 tile.
//For example: If we want 1 2 3
//                        0 4 5
//                        6 7 8
//Then we would have
//1 -> [0, 0], 2 -> [0, 1], 3 -> [0, 2]
//0 -> [1, 0], 4 -> [1, 1], 5 -> [1, 2]
//6 -> [2, 0], 7 -> [2, 1], 8 -> [2, 2]s
static vector<pair<int, int>> finalPositions = {};

struct State {
    vector<vector<int>> board; // Replaced integer board with a 2D vector
    pair<int, int> blankPos;   // Used a pair to track the blank position
    int g;
    int h;

    bool operator<(const State &other) const {
        return (g + h) > (other.g + other.h);
    }
};

void FillFinalPositions(int n, pair<int, int> emptyTile = pair<int, int>(-1, -1))
{
    int len = (int)sqrt(n + 1);
    finalPositions.resize(n + 1);

    if (emptyTile == make_pair(-1, -1))
    {
        emptyTile = {len - 1, len - 1};
    }

    for (int value = 1, i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {   if (emptyTile.first == i && emptyTile.second == j)
            {
                finalPositions[0]  = {i, j};
            }
            else
            {
                finalPositions[value++] = {i, j};
            }
        }
    }
}

string BoardToString(const vector<vector<int>> &board)
{
    string key = "";

    for (auto row : board)
    {
        for (auto el : row)
        {
            key += to_string(el);
        }
    }

    return key;
}

pair<int, int> FindBlankPos(const vector<vector<int>> &board)
{
    const size_t n = board.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 0) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

// Manhattan distance
// This is the distance from our current board, to the final state, we are trying to get.
// The distance is calculated by taking: abs(xCurr - x-Final) + abs(yCurr - yFinal)
// Example
// Curr 2 1  Final 1 2
//      3 0        3 0
// (abs(0 - 0) + abs(0 - 1)) + (abs(0 - 0) + abs(1 - 0)) +
// (abs(1 - 1) + abs(0 - 0))  = 1 + 1 + 0 + 0 = 2
//
// When distance is closer to 0, we are getting to the correct configuration.
// This is needed to "guide" the path algorithm to where to search.
// Ideal is when Distance is 0
int ManhattanDistance(const vector<vector<int>> &board)
{

    int distance = 0;

    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {

            if (board[i][j] != 0)
            {
                distance += (abs(i - finalPositions[board[i][j]].first) +
                             abs(j - finalPositions[board[i][j]].second));
            }
        }
    }

    return distance;
}

vector<State> GenerateNeighbors(const State& current)
{
    vector<State> neighbors = {};
    const auto& blank = current.blankPos;
    auto board = current.board;
    const size_t len = current.board.size();

    for (const auto& dir : direction)
    {
        int newX = (blank.first + dir.first);
        int newY = (blank.second + dir.second);
        if ( 0 <= newX && newX < len &&
             0 <= newY && newY < len)
        {
            swap(board[blank.first][blank.second], board[newX][newY]);
            neighbors.push_back(
            {
                board,
                make_pair(newX, newY),
                current.g + 1,
                ManhattanDistance(board)
            });
            swap(board[blank.first][blank.second], board[newX][newY]);
        }
    }

    return neighbors;
}

int AStar(vector<vector<int>> startBoard)
{

    priority_queue<State> unvisitedStates;
    unordered_set<string> visitedStates;

    unvisitedStates.push(
    {
        startBoard,
        FindBlankPos(startBoard),
        0,
        ManhattanDistance(startBoard)
    });

    while (unvisitedStates.empty() == false)
    {
        State current = unvisitedStates.top();
        unvisitedStates.pop();

        if (current.h == 0)
        {
            return current.g;
        }

        for (auto neighbor : GenerateNeighbors(current))
        {
            string key = BoardToString(neighbor.board);
            if (visitedStates.find(key) == visitedStates.end())
            {
                visitedStates.emplace(key);
                unvisitedStates.push(neighbor);
            }
        }
    }

    return -1;
}

int run(vector<vector<int>> arr)
{
    auto startTime = chrono::high_resolution_clock::now();
    int result = AStar(arr);
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;

    cout << "Steps to solve: " << result << endl;
    cout << "Execution time: " << elapsed.count() << " seconds" << endl;
    return result;
}

TEST_CASE("Test: 1")
{
    vector<vector<int>> arr =
    {
        {1,  2, 0},
        {3,  4, 5},
        {6,  7, 8}
    };
    int res = 2;
    FillFinalPositions(8, make_pair(0, 0));
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 2")
{
    vector<vector<int>> arr =
    {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 0, 15}
    };
    int res = 1;
    FillFinalPositions(15);
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 3")
{
    vector<vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };
    int res = 2;
    FillFinalPositions(8, make_pair(2, 2));
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 4")
{
    vector<vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };
    int res = 0;
    FillFinalPositions(8);
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 5")
{
    vector<vector<int>> arr =
    {
        {6, 5, 3},
        {2, 4, 8},
        {7, 0, 1},
    };
    int res = 21;
    FillFinalPositions(8);
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 6")
{
    vector<vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 0},
        {6, 7, 8}
    };
    int res = 1;
    FillFinalPositions(8, make_pair(1, 1));
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 7")
{
    vector<vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {8, 7, 0}
    };
    int res = -1;
    FillFinalPositions(8, make_pair(1, 1));
    CHECK(run(arr) == res);

}

TEST_CASE("Test: 8")
{
    vector<vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };
    int res = 2;
    FillFinalPositions(8);
    CHECK(run(arr) == res);
}

int _main()
{
    vector<vector<int>> startBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };

    FillFinalPositions(8);

    auto startTime = chrono::high_resolution_clock::now();
    int result = AStar(startBoard);
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;

    cout << "Steps to solve: " << result << endl;
    cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    return 0;
}