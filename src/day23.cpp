#include <iostream>
#include <set>
#include <deque>
#include <map>
#include <exercise.hpp>

using namespace exercise;



std::set<std::pair<int, int>> getStartingPositions(std::vector<std::vector<char>> lines)
{
    std::set<std::pair<int, int>> positions = std::set<std::pair<int, int>>();
    
    int row(0), col;
    for (auto l: lines)
    {
        col = 0;
        for (auto c: l)
        {
            if (c == '#') positions.insert(std::make_pair(col, row));
            col++;
        }
        row++;
    }
    return positions;
}

std::optional<std::pair<int, int>> move(
    const std::set<std::pair<int, int>>& positions,
    std::pair<int, int> current,
    char dir
)
{
    switch (dir)
    {
        case 'N':
            if (
                positions.contains(std::make_pair(current.first - 1, current.second - 1))
                || positions.contains(std::make_pair(current.first, current.second - 1))
                || positions.contains(std::make_pair(current.first + 1, current.second - 1))
            ) return {};
            else return std::make_pair(current.first, current.second - 1);
        case 'S':
            if (
                positions.contains(std::make_pair(current.first - 1, current.second + 1))
                || positions.contains(std::make_pair(current.first, current.second + 1))
                || positions.contains(std::make_pair(current.first + 1, current.second + 1))
            ) return {};
            else return std::make_pair(current.first, current.second + 1);
        case 'E':
            if (
                positions.contains(std::make_pair(current.first + 1, current.second - 1))
                || positions.contains(std::make_pair(current.first + 1, current.second))
                || positions.contains(std::make_pair(current.first + 1, current.second + 1))
            ) return {};
            else return std::make_pair(current.first + 1, current.second);
        case 'W':
            if (
                positions.contains(std::make_pair(current.first - 1, current.second - 1))
                || positions.contains(std::make_pair(current.first - 1, current.second))
                || positions.contains(std::make_pair(current.first - 1, current.second + 1))
            ) return {};
            else return std::make_pair(current.first - 1, current.second);
    };
    return {};
}

bool shouldMove(
    const std::set<std::pair<int, int>>& positions,
    std::pair<int, int> current
)
{
    return (
        positions.contains(std::make_pair(current.first - 1, current.second - 1))
        || positions.contains(std::make_pair(current.first - 1, current.second))
        || positions.contains(std::make_pair(current.first - 1, current.second + 1))
        || positions.contains(std::make_pair(current.first + 1, current.second - 1))
        || positions.contains(std::make_pair(current.first + 1, current.second))
        || positions.contains(std::make_pair(current.first + 1, current.second + 1))
        || positions.contains(std::make_pair(current.first, current.second + 1))
        || positions.contains(std::make_pair(current.first, current.second - 1))
    );
}

std::map<std::pair<int, int>, std::pair<int, int>> getNextMoves(
    const std::set<std::pair<int, int>>& positions,
    std::deque<char>& directionOrder
)
{
    std::vector<char> directions = std::vector(directionOrder.begin(), directionOrder.end());
    std::optional<std::pair<int, int>> next = {};
    std::map<std::pair<int, int>, std::pair<int, int>> resultingMoves = std::map<std::pair<int, int>, std::pair<int, int>>();
    std::set<std::pair<int, int>> duplicates = std::set<std::pair<int, int>>();
    int dirIndex;
    for (auto& start: positions)
    {
        if (shouldMove(positions, start))
        {
            dirIndex = 0;
            next = {};
            while (!next.has_value() && dirIndex < directions.size()) {
                next = move(positions, start, directions[dirIndex]);
                dirIndex++;
            }
            if (next.has_value())
            {
                // std::cout << "(" << start.first << "," << start.second << ") -> (" << next.value().first << "," << next.value().second << ")" << std::endl;
                if (resultingMoves.contains(next.value())) duplicates.insert(next.value());
                else resultingMoves.insert(std::make_pair(next.value(), start));
            }
        }
    }
    for (auto &dupe: duplicates) resultingMoves.erase(dupe);
    char toMoveBack(directionOrder.front());
    directionOrder.pop_front();
    directionOrder.push_back(toMoveBack);
    return resultingMoves;
}

void print(
    const std::set<std::pair<int, int>>& positions,
    int minRow,
    int minCol,
    int maxRow,
    int maxCol
) {
    for (auto r = minRow - 1; r <= maxRow + 1; r++)
    {
        for (auto c = minCol - 1; c <= maxCol + 1; c++)
        {
            if (positions.contains(std::make_pair(c, r))) std::cout << "#";
            else std::cout << ".";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <>
void CharGridExercise::runPart1()
{
    std::set<std::pair<int, int>> currentPositions = getStartingPositions(this -> data);
    std::deque<char> directionOrder = std::deque<char>({'N', 'S', 'W', 'E'});

    for (auto i = 0; i < 10; i++)
    {
        std::map<std::pair<int, int>, std::pair<int, int>> moves = getNextMoves(
            currentPositions,
            directionOrder
        );
        for (auto& [to, from]: moves)
        {
            currentPositions.erase(from);
            currentPositions.insert(to);
        }
    }
    int minRow(INT_MAX), minCol(INT_MAX), maxRow(0), maxCol(0);
    for (auto &p: currentPositions)
    {
        minRow = std::min(minRow, p.second);
        maxRow = std::max(maxRow, p.second);
        minCol = std::min(minCol, p.first);
        maxCol = std::max(maxCol, p.first);
    }
    std::cout << "Part 1: Empty ground tiles " << (maxRow - minRow + 1) * (maxCol - minCol + 1) - currentPositions.size() << std::endl;
}

template <>
void CharGridExercise::runPart2()
{
    std::set<std::pair<int, int>> currentPositions = getStartingPositions(this -> data);
    std::deque<char> directionOrder = std::deque<char>({'N', 'S', 'W', 'E'});
    
    int stationaryIter(-1);
    for (auto i = 0; i < 1000; i++)
    {
        std::map<std::pair<int, int>, std::pair<int, int>> moves = getNextMoves(
            currentPositions,
            directionOrder
        );
        if (moves.size() == 0) {
            stationaryIter = i + 1;
            break;
        }
        for (auto& [to, from]: moves)
        {
            currentPositions.erase(from);
            currentPositions.insert(to);
        }
    }
    std::cout << "Part 2: First round no movement " << stationaryIter << std::endl;
}


int main() {
    CharGridExercise exerciseRunner(23);
    std::cout << "Day 23 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}