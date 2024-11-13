#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <unordered_set>

using namespace std;

struct State
{
    int     board;
    size_t  blankPos;
    int     g;
    int     h;

    bool operator<(const State &other) const
    {
        return (g+h) > (other.g + other.h);
    }
};

const int finalState = 123456780;
const int finalPositions[9][2] =
{
    {2, 2},                 // 0, so we can index them using 0-8
    {0, 0}, {0, 1}, {0, 2}, // 1 2 3
    {1, 0}, {1, 1}, {1, 2}, // 4 5 6
    {2, 0}, {2, 1},         // 7 8
};
const int direction[4][2] =
{
    {0, -1} , {0, 1},   //  left right
    {-1, 0} , {1, 0}    //  up   down
};

int toInt(const vector<vector<int>> &board)
{
    size_t result = 0;
    for (auto &row : board)
    {
        for (auto &el : row)
        {
            result *= 10;
            result += el;
        }
    }

    return result;
}

int heuristicDistance(int board)
{
    int distance = 0;
    int index = 0;
    int tile = 0;

    string strBoard = to_string(board);

    if (strBoard.size() != 9)
    {
        return -1;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            index = j + i*3;
            // cout << "index = " << index << endl;

            if ((tile = (strBoard[index] - '0')))
            {
                //           currentRow - finalRow             +  currentCol - finalCol
                distance += (abs(i - finalPositions[tile][0])) + (abs(j-finalPositions[tile][1]));
                // cout << "tile = " << tile << ", i = " << i << ", j = " << j << ""<< endl;
                // cout << "finalPositions[" << tile << "][0] = " << finalPositions[tile][0] << endl;
                // cout << "finalPositions[" << tile << "][1] = " << finalPositions[tile][1] << endl;
                // cout << "abs(" << i << " - " << finalPositions[tile][0] << ") = " << abs(i - finalPositions[tile][0]) << endl;
                // cout << "abs(" << j << " - " << finalPositions[tile][1] << ") = " << abs(j - finalPositions[tile][1]) << endl;

                // cout << distance << endl;
            }
        }
    }

    return distance;
}

//
// Returns an array with possible next configurations
// based on the legal moves.
//
vector<int> generateNeighbors(int board, int blankPos)
{
    vector<int> configurations;
    int x = blankPos / 3;
    int y = blankPos % 3;

    for (int i = 0; i < 4; i++)
    {
        int nX = x + direction[i][0];
        int nY = y + direction[i][1];

        if (nX >= 0 && nX < 3 && nY >= 0 && nY < 3)
        {
            string newBoard = to_string(board);
            int newBlankPos = nX*3 + nY;
            swap(newBoard[blankPos],newBoard[newBlankPos]);
            configurations.push_back(stoi(newBoard));
        }
    }

    return configurations;
}

int aStar(vector<vector<int>> &startBoard)
{
    priority_queue<State> pq;
    unordered_set<int> visited;

    pq.push(
        {
            toInt(startBoard),
            to_string(toInt(startBoard)).find('0'),
            0,
            heuristicDistance(toInt(startBoard))
        });

    visited.insert(toInt(startBoard));

    while (!pq.empty())
    {
        State current = pq.top();
        pq.pop();

        if(current.board == finalState)
        {
            return current.g;
        }

        for (auto el : generateNeighbors(current.board, current.blankPos))
        {
            if (visited.find(el) == visited.end())
            {
                visited.insert(el);
                pq.push(
                    {
                        el,
                        to_string(el).find('0'),
                        current.g+1,
                        heuristicDistance(el)
                    });
            }
        }
    }

    return -1;
}

/*
    TODO:
    1) Update the code to work for N.
    2) Use an array instead of integer.
    3) Fit the solution for the requirements.
    4) Add logging options, executions time
       and do it so it doesn't affect running
       time of algorithm.
    5) Optimize the solution.
*/

int main()
{
    vector<vector<int>> board =
    {
        {1,2,3},
        {4,5,6},
        {0,7,8}
    };
    int d1 = heuristicDistance(toInt(board));
    int d2 = heuristicDistance(finalState);

    cout << endl;
    cout << toInt(board) << endl;
    cout << d1 << endl;
    cout << d2 << endl;

    vector<int> configurations = generateNeighbors(toInt(board), 7);
    for (auto el : configurations)
    {
        cout << el << ", d = " << heuristicDistance(el) << endl;
    }

    cout << aStar(board) << endl;

    return 0;
}