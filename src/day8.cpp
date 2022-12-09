#include <iostream>
#include <vector>
#include <set>
#include <exercise.hpp>

using namespace exercise;


void rotateView(
    std::vector<std::vector<short>>& view,
    int numRows,
    int numCols
)
{
    // rotate 90 degrees clockwise
    std::vector<std::vector<short>> nextView = std::vector<std::vector<short>>(numCols);

    for (auto i=0; i < numRows; i++)
    {
        for (auto j=0; j < numCols; j++)
        {
            nextView[j].push_back(view[numRows - 1 - i][j]);
        }
    }
    view = nextView;
}

std::pair<int, int> originalLocation(
    std::pair<int, int> currentLoc,
    int numRows,
    int numCols,
    int rotation
)
{
    if (rotation == 0) return currentLoc;
    else if (rotation == 1) return std::make_pair(numRows - 1 - currentLoc.second, currentLoc.first);
    else if (rotation == 2) return std::make_pair(numRows - 1 - currentLoc.first, numCols - 1 - currentLoc.second);
    else return std::make_pair(currentLoc.second, numCols - 1 - currentLoc.first);
}

int scenicScore(
    std::pair<int, int> currentLoc,
    std::vector<std::vector<short>>& view,
    int numRows,
    int numCols
)
{
    std::vector<std::pair<int, int>> steps = {
        std::make_pair(-1, 0),  // left
        std::make_pair(0, -1),  // up
        std::make_pair(1, 0),   // right
        std::make_pair(0, 1),   // down
    };
    
    int maxAllowed(view[currentLoc.first][currentLoc.second]);
    std::pair<int, int> pos;
    int dirScore;
    int totalScore(1);
    for (auto step: steps)
    {
        pos = std::make_pair(
            currentLoc.first + step.first,
            currentLoc.second + step.second
        );
        dirScore = 0;
        while (
            pos.first >= 0 && pos.first < numRows
            && pos.second >= 0 && pos.second < numCols
            && view[pos.first][pos.second] < maxAllowed
        )
        {
            dirScore++;
            pos = std::make_pair(
                pos.first + step.first,
                pos.second + step.second
            );
        }
        if (
            pos.first >= 0 && pos.first < numRows
            && pos.second >= 0 && pos.second < numCols
        ) dirScore++;
        totalScore *= dirScore;
    }
    return totalScore;
}

template <>
void DigitGridExercise::runPart1()
{
    std::set<std::pair<int, int>> visibleCoords = std::set<std::pair<int, int>>();
    std::vector<std::vector<short>> treeView(this -> data);
    int numRows(treeView.size()), numCols(treeView[0].size());
    for (auto direction=0; direction < 4; direction++)
    {
        for (auto r=0; r<treeView.size(); r++)
        {
            int currentMax(-1);
            for (auto c=0; c<treeView[0].size(); c++)
            {
                int height(treeView[r][c]);
                if (height > currentMax) 
                {
                    std::pair<int, int> loc = originalLocation(
                        std::make_pair(r, c), numRows, numCols, direction
                    );
                    visibleCoords.insert(loc);
                    currentMax = height;
                }
            }
        }
        rotateView(treeView, treeView.size(), treeView[0].size());
    }
    std::cout << "Part 1: Num visible trees " << std::to_string(visibleCoords.size()) << std::endl;
}

template <>
void DigitGridExercise::runPart2()
{
    std::vector<std::vector<short>> treeView(this -> data);
    int numRows(treeView.size()), numCols(treeView[0].size());
    int highestScenicScore(-1);
    for (auto r=0; r<treeView.size(); r++)
    {
        for (auto c=0; c<treeView[0].size(); c++)
        {
            highestScenicScore = std::max(
                highestScenicScore,
                scenicScore(
                    std::make_pair(r, c),
                    treeView,
                    numRows,
                    numCols
                )
            );
        }
    }
    std::cout << "Part 2: Highest scenic score " << std::to_string(highestScenicScore) << std::endl;
}

int main() {
    DigitGridExercise exerciseRunner(8);
    std::cout << "Day 8 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
