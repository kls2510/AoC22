#include <iostream>
#include <exercise.hpp>

using namespace exercise;


template <>
void IntVectorExercise::runPart1()
{
    int currentTotal(0);
    int maxTotal(0);
    for (auto n: this->data) {
        if (n == -1) currentTotal = 0;
        else currentTotal += n;
        if (currentTotal > maxTotal) maxTotal = currentTotal;
    }
    if (currentTotal > maxTotal) maxTotal = currentTotal;
    std::cout << "Part 1: max sum " << std::to_string(maxTotal) << std::endl;
}

template <>
void IntVectorExercise::runPart2()
{
    std::vector<int> maxHeap = {};
    int currentTotal(0);
    for (auto n: this->data) {
        if (n == -1) {
            maxHeap.push_back(currentTotal);
            push_heap(maxHeap.begin(), maxHeap.end());
            currentTotal = 0;
        }
        else currentTotal += n;
    }
    maxHeap.push_back(currentTotal);
    push_heap(maxHeap.begin(), maxHeap.end());
    currentTotal = 0;

    int result(0);
    for (int i = 0; i < 3; i++) {
        result += maxHeap.front();
        pop_heap(maxHeap.begin(), maxHeap.end());
        maxHeap.pop_back();
    }

    std::cout << "Part 2: Top 3 elves sum: " << std::to_string(result) << std::endl;
}

int main() {
    IntVectorExercise exerciseRunner(1);
    std::cout << "Day 1 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
