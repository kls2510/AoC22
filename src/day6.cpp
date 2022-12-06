#include <iostream>
#include <vector>
#include <map>
#include <exercise.hpp>

using namespace exercise;

void upsert(char value, std::map<char, int>& mp, bool inc) 
{
    if (mp.contains(value)) inc ? ++mp.at(value): (mp.at(value) == 1) ? mp.erase(value) : --mp.at(value);
    else if (inc) mp.insert(std::make_pair(value, 1));
}

int numCharsToProcess(std::string signal, int distinctGroupSize)
{
    int start(0), windowLen(distinctGroupSize);
    std::map<char, int> window = std::map<char, int>();
    for (auto i=start; i < windowLen; i++) upsert(signal[i], window, true);
    while (window.size() < windowLen && start < signal.length() - windowLen) {
        upsert(signal[start], window, false);
        start++;
        upsert(signal[start + windowLen - 1], window, true);
    }
    return start + windowLen;
}

template <>
void StringVectorExercise::runPart1()
{
    int numChars = numCharsToProcess((this->data)[0], 4);
    std::cout << "Part 1: First place all different " << std::to_string(numChars) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    int numChars = numCharsToProcess((this->data)[0], 14);
    std::cout << "Part 2: First place all different " << std::to_string(numChars) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(6);
    std::cout << "Day 6 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
