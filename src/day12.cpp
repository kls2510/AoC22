#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <exercise.hpp>

using namespace exercise;


std::pair<int, int> find(std::vector<std::vector<char>>& grid, char toFind)
{
    int row(0), col(0);
    bool found(false);
    std::pair<int, int> coords;
    for (auto r: grid) 
    {
        col = 0;
        for (auto v: r){
            if (v == toFind) {
                found = true;
                coords = std::make_pair(row, col);
            }
            col++;
        }
        if (found) break;
        row++;
    }
    return coords;
}

std::pair<int, int> findStart(std::vector<std::vector<char>>& grid)
{
    return find(grid, 'S');
}

std::pair<int, int> findEnd(std::vector<std::vector<char>>& grid)
{
    return find(grid, 'E');
}

bool isValid(char from, char to, bool goingUp) 
{
    if ((goingUp ? from: to) == 'S') (goingUp ? from: to) = 'a';
    if ((goingUp ? to: from) == 'E') (goingUp ? to: from) = 'z';
    return (goingUp ? to - from: from - to) <= 1;
}

std::vector<std::pair<int, int>> availableMoves(
    std::vector<std::vector<char>>& grid,
    const std::set<std::pair<int, int>>& visited,
    std::pair<int, int> current,
    bool goingUp = true
) {
    char currentLevel(grid[current.first][current.second]);
    std::vector<std::pair<int, int>> moves = std::vector<std::pair<int, int>>();
    if (
        current.first > 0 
        && !visited.contains(std::make_pair(current.first - 1, current.second))
        && isValid(currentLevel, grid[current.first - 1][current.second], goingUp)
    ) moves.push_back(std::make_pair(current.first - 1, current.second));

    if (
        current.first < grid.size() - 1
        && !visited.contains(std::make_pair(current.first + 1, current.second))
        && isValid(currentLevel, grid[current.first + 1][current.second], goingUp)
    ) moves.push_back(std::make_pair(current.first + 1, current.second));

    if (
        current.second > 0
        && !visited.contains(std::make_pair(current.first, current.second - 1))
        && isValid(currentLevel, grid[current.first][current.second - 1], goingUp)
    ) moves.push_back(std::make_pair(current.first, current.second - 1));

    if (
        current.second < grid[0].size() - 1
        && !visited.contains(std::make_pair(current.first, current.second + 1))
        && isValid(currentLevel, grid[current.first][current.second + 1], goingUp)
    ) moves.push_back(std::make_pair(current.first, current.second + 1));

    return moves;
}

template <>
void CharGridExercise::runPart1()
{
    std::vector<std::vector<char>> grid = this -> data;
    std::pair<int, int> start(findStart(grid));
    std::queue<std::pair<int, std::pair<int, int>>> q({std::make_pair(0, start)});
    std::set<std::pair<int, int>> visited({start});
    int numSteps;

    while (!q.empty())
    {
        std::pair<int, std::pair<int, int>> current = q.front();
        q.pop();
        char currentChar(grid[current.second.first][current.second.second]);
        if (currentChar == 'E')
        { 
            numSteps = current.first;
            break; 
        }
        for (auto next: availableMoves(grid, visited, current.second))
        {
            q.push(std::make_pair(current.first + 1, next));
            visited.insert(next);
        }
    }

    std::cout << "Part 1: min num steps " << std::to_string(numSteps) << std::endl;
}

template <>
void CharGridExercise::runPart2()
{
    std::vector<std::vector<char>> grid = this -> data;
    std::pair<int, int> end(findEnd(grid));
    std::queue<std::pair<int, std::pair<int, int>>> q({std::make_pair(0, end)});
    std::set<std::pair<int, int>> visited({end});
    int minNumSteps;

    while (!q.empty())
    {
        std::pair<int, std::pair<int, int>> current = q.front();
        q.pop();
        char currentChar(grid[current.second.first][current.second.second]);
        if (currentChar == 'a' || currentChar == 'S') 
        { 
            minNumSteps = current.first;
            break; 
        }
        for (auto next: availableMoves(grid, visited, current.second, false))
        {
            q.push(std::make_pair(current.first + 1, next));
            visited.insert(next);
        }
    }

    std::cout << "Part 2: min possible steps " << std::to_string(minNumSteps) << std::endl;
}

int main() {
    CharGridExercise exerciseRunner(12);
    std::cout << "Day 12 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
