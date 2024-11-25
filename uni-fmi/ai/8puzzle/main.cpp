#include <iostream>
#include <vector>
#include <chrono>
#include "solution.h"


int main()
{
    std::vector<std::vector<int>> startBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };

    FillFinalPositions(8);

    auto startTime = std::chrono::high_resolution_clock::now();
    int result = AStar(startBoard);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Steps to solve: " << result << std::endl;
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}