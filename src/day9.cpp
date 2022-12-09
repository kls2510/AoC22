#include <iostream>
#include <vector>
#include <set>
#include <exercise.hpp>

using namespace exercise;

void moveHead(std::pair<int, int>& headPos, char dir, int distance)
{
    if (dir == 'U') headPos = std::make_pair(headPos.first, headPos.second + distance);
    else if (dir == 'D') headPos = std::make_pair(headPos.first, headPos.second - distance);
    else if (dir == 'L') headPos = std::make_pair(headPos.first - distance, headPos.second);
    else headPos = std::make_pair(headPos.first + distance, headPos.second);
}

void moveTail(std::pair<int, int>& tailPos, std::pair<int, int> headPos)
{
    std::pair<int, int> diff = std::make_pair(
        headPos.first - tailPos.first, headPos.second - tailPos.second
    );
    if (diff.second == 0 && std::abs(diff.first) == 2) tailPos = std::make_pair(
        tailPos.first + diff.first / std::abs(diff.first),
        tailPos.second
    );
    else if (diff.first == 0 && std::abs(diff.second) == 2) tailPos = std::make_pair(
        tailPos.first,
        tailPos.second + diff.second / std::abs(diff.second)
    );
    else if (
        std::abs(diff.first) >= 2 || std::abs(diff.second) >= 2
    ) tailPos = std::make_pair(
        tailPos.first + diff.first / std::abs(diff.first),
        tailPos.second + diff.second / std::abs(diff.second)
    );
}

int simulateDistinctTailPositions(
    int numKnots, 
    const std::vector<std::pair<char, int>>& moves
)
{
    std::set<std::pair<int, int>> distinctTailLocations = std::set<std::pair<int, int>>();
    std::vector<std::pair<int, int>> knots = std::vector<std::pair<int, int>>(numKnots);
    for (auto i = 0; i < numKnots; i++) knots[i] = std::make_pair(0, 0);
    int tailIndex(numKnots - 1);
    distinctTailLocations.insert(knots[tailIndex]);
    for (auto move: moves)
    {
        for (auto step = 0; step < move.second; step ++) 
        {
            moveHead(knots[0], move.first, 1);
            for (auto i = 1; i < knots.size(); i++) moveTail(knots[i], knots[i - 1]);
            distinctTailLocations.insert(knots[tailIndex]);
        }
    }
    return distinctTailLocations.size();
}

template <>
void CharIntPairExercise::runPart1()
{
    std::cout << "Part 1: Num tail locations " << std::to_string(simulateDistinctTailPositions(
        2, this -> data
    )) << std::endl;
}

template <>
void CharIntPairExercise::runPart2()
{
    std::cout << "Part 2: Num tail locations " << std::to_string(simulateDistinctTailPositions(
        10, this -> data
    )) << std::endl;
}

int main() {
    CharIntPairExercise exerciseRunner(9);
    std::cout << "Day 9 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
