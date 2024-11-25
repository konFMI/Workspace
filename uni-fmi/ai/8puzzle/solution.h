#include <vector>

void FillFinalPositions(int n, std::pair<int, int> emptyTile = {-1, -1});
int AStar(std::vector<std::vector<int>> startBoard);
int IDAStar(const std::vector<std::vector<int>>& startBoard);
