#include <iostream>
#include <unordered_map>
#include <exercise.hpp>

using namespace exercise;


void mix(
    const std::vector<long>& originalNumbers, 
    std::unordered_map<int, int>& originalIndexToMixedIndex
)
{
    int originalIndex(0), mixedIndex, currentIndex, s(originalNumbers.size() - 1);
    for (auto mixSteps: originalNumbers)
    {
        mixedIndex = originalIndexToMixedIndex.at(originalIndex);
        currentIndex = (mixedIndex + mixSteps) <= 0 ? originalNumbers.size() - 1 - (std::abs(mixedIndex + mixSteps) % s): (mixedIndex + mixSteps) % s;
        for (auto p: originalIndexToMixedIndex)
        {
            if (
                (mixedIndex < currentIndex && p.second > mixedIndex && p.second <= currentIndex)
            ) originalIndexToMixedIndex[p.first] = p.second - 1;
            if (
                (currentIndex < mixedIndex && p.second >= currentIndex && p.second < mixedIndex)
            ) originalIndexToMixedIndex[p.first] = p.second + 1;
        }
        originalIndexToMixedIndex[originalIndex] = currentIndex;
        originalIndex++;
    }
}

std::tuple<long, long, long> coordinates(const std::vector<long>& numbersState)
{
    int indexOfZero(std::distance(numbersState.begin(), std::find(numbersState.begin(), numbersState.end(), 0)));
    int numNumbers(numbersState.size());
    return std::make_tuple(
        numbersState[(indexOfZero + 1000) % numNumbers],
        numbersState[(indexOfZero + 2000) % numNumbers],
        numbersState[(indexOfZero + 3000) % numNumbers]
    );
}

template <>
void IntVectorExercise::runPart1()
{
    std::vector<long> numbersState = std::vector<long>(this -> data.size());
    for (auto i=0; i<numbersState.size(); i++) numbersState[i] = this -> data[i];
    std::unordered_map<int, int> originalIndexToMixedIndex = std::unordered_map<int, int>();
    for (auto i=0; i<numbersState.size(); i++) originalIndexToMixedIndex.insert(std::make_pair(i, i));
    mix(numbersState, originalIndexToMixedIndex);

    std::vector<long> currentState = std::vector<long>(this -> data.size());
    for (auto p: originalIndexToMixedIndex) currentState[p.second] = numbersState[p.first];
    std::tuple<long, long, long> coords = coordinates(currentState);
    long coordSum = std::get<0>(coords) + std::get<1>(coords) + std::get<2>(coords);
    
    std::cout << "Part 1: Sum of coords " << coordSum << std::endl;
}


template <>
void IntVectorExercise::runPart2()
{
    std::vector<long> numbersState = std::vector<long>(this -> data.size());
    for (auto i=0; i<numbersState.size(); i++) numbersState[i] = this -> data[i] * 811589153L;
    std::unordered_map<int, int> originalIndexToMixedIndex = std::unordered_map<int, int>();
    for (auto i=0; i<numbersState.size(); i++) originalIndexToMixedIndex.insert(std::make_pair(i, i));

    for (int r = 0; r < 10; r++) mix(numbersState, originalIndexToMixedIndex);
    
    std::vector<long> currentState = std::vector<long>(this -> data.size());
    for (auto p: originalIndexToMixedIndex) currentState[p.second] = numbersState[p.first];
    std::tuple<long, long, long> coords = coordinates(currentState);
    long coordSum = std::get<0>(coords) + std::get<1>(coords) + std::get<2>(coords);
    std::cout << "Part 2: Sum of coords " << coordSum << std::endl;
}

int main() {
    IntVectorExercise exerciseRunner(20);
    std::cout << "Day 20 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
