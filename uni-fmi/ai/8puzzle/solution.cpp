#include "solution.h"
#include <climits>
#include <cstddef>
#include <utility>
#include <string>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <algorithm>

// TODO:
// 1) Add IDA*
// 3) Refactor, currently too complex seems.

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
static std::vector<std::pair<int, int>> finalPositions = {};

struct State {
    std::vector<std::vector<int>> board; // Replaced integer board with a 2D vector
    std::pair<int, int> blankPos;   // Used a pair to track the blank position
    int g;
    int h;

    int f() const
    {
        return g+h;
    }

    bool operator<(const State &other) const {
        return (g + h) > (other.g + other.h);
    }
};

void FillFinalPositions(int n, std::pair<int, int> emptyTile)
{
    int len = (int)sqrt(n + 1);
    finalPositions.resize(n + 1);

    if (emptyTile == std::make_pair(-1, -1))
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

static std::string BoardToString(const std::vector<std::vector<int>> &board)
{
    std::string key = "";

    for (auto row : board)
    {
        for (auto el : row)
        {
            key += std::to_string(el);
        }
    }

    return key;
}

static std::pair<int, int> FindBlankPos(const std::vector<std::vector<int>> &board)
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
static int ManhattanDistance(const std::vector<std::vector<int>> &board)
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

static std::vector<State> GenerateNeighbors(const State& current)
{
    static const std::pair<int, int> direction[4] = {
        {0, -1}, {0, 1},  // left, right
        {-1, 0}, {1, 0}   // up, down
    };

    std::vector<State>           neighbors   = {};
    const std::pair<int, int>&   blank       = current.blankPos;
    std::vector<std::vector<int>>     board       = current.board;
    const size_t            len         = current.board.size();

    for (const auto& dir : direction)
    {
        int newX = (blank.first + dir.first);
        int newY = (blank.second + dir.second);
        if ( 0 <= newX && newX < len &&
             0 <= newY && newY < len)
        {
            std::swap(board[blank.first][blank.second], board[newX][newY]);
            neighbors.push_back(
            {
                board,
                std::make_pair(newX, newY),
                current.g + 1,
                ManhattanDistance(board)
            });
            std::swap(board[blank.first][blank.second], board[newX][newY]);
        }
    }

    return neighbors;
}

int AStar(std::vector<std::vector<int>> startBoard)
{

    std::priority_queue<State> unvisitedStates;
    std::unordered_set<std::string> visitedStates;

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
            std::string key = BoardToString(neighbor.board);
            if (visitedStates.find(key) == visitedStates.end())
            {
                visitedStates.emplace(key);
                unvisitedStates.push(neighbor);
            }
        }
    }

    return -1;
}

// Improved IDAStar Implementation

// Helper function to perform the IDA* search with a given bound.
static bool IDAStarSearch(State& current, int bound, int& newBound, std::unordered_set<std::string>& visitedStates) {
    int f = current.f(); // f = g + h
    if (f > bound) {
        newBound = std::min(newBound, f); // Update the new bound
        return false;
    }

    if (current.h == 0) { // If Manhattan distance is 0, we've found the goal
        return true;
    }

    visitedStates.insert(BoardToString(current.board)); // Mark the current state as visited

    // Try all possible neighbors
    for (auto& neighbor : GenerateNeighbors(current)) {
        std::string key = BoardToString(neighbor.board);
        if (visitedStates.find(key) == visitedStates.end()) {
            if (IDAStarSearch(neighbor, bound, newBound, visitedStates)) {
                return true; // Goal reached
            }
        }
    }

    visitedStates.erase(BoardToString(current.board)); // Backtrack
    return false; // No solution found at this level
}

// Main IDA* function to find the solution.
int IDAStar(const std::vector<std::vector<int>>& startBoard) {
    State startState{
        startBoard,
        FindBlankPos(startBoard),
        0,
        ManhattanDistance(startBoard)
    };

    int bound = startState.f(); // Initial bound
    std::unordered_set<std::string> visitedStates;

    while (true) {
        int newBound = INT_MAX; // Set to the maximum possible value
        if (IDAStarSearch(startState, bound, newBound, visitedStates)) {
            return startState.g; // If goal is found, return the depth (g)
        }

        if (newBound == INT_MAX) {
            return -1; // No solution found
        }

        bound = newBound; // Increase the bound for the next iteration
    }
}