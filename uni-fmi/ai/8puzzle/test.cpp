#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "solution.h"
#include <chrono>

#define STEPS_TIME 0

int run(std::vector<std::vector<int>> arr)
{

#if STEPS_TIME
    auto startTime = std::chrono::high_resolution_clock::now();
#endif

    int result = AStar(arr);

#if STEPS_TIME
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Steps to solve: " << result << std::endl;
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
#endif
    return result;
}

TEST_CASE("Test: 1")
{
    std::vector<std::vector<int>> arr =
    {
        {1,  2, 0},
        {3,  4, 5},
        {6,  7, 8}
    };
    int res = 2;
    FillFinalPositions(8, std::make_pair(0, 0));
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 2")
{
    std::vector<std::vector<int>> arr =
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
    std::vector<std::vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };
    int res = 2;
    FillFinalPositions(8, std::make_pair(2, 2));
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 4")
{
    std::vector<std::vector<int>> arr =
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
    std::vector<std::vector<int>> arr =
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
    std::vector<std::vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 0},
        {6, 7, 8}
    };
    int res = 1;
    FillFinalPositions(8, std::make_pair(1, 1));
    CHECK(run(arr) == res);
}

TEST_CASE("Test: 7")
{
    std::vector<std::vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {8, 7, 0}
    };
    int res = -1;
    FillFinalPositions(8, std::make_pair(1, 1));
    CHECK(run(arr) == res);

}

TEST_CASE("Test: 8")
{
    std::vector<std::vector<int>> arr =
    {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8}
    };
    int res = 2;
    FillFinalPositions(8);
    CHECK(run(arr) == res);
}
