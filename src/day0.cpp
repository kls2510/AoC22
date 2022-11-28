#include <iostream>
#include <exercise.hpp>

using namespace exercise;


template <>
void IntVectorExercise::runPart1()
{
    int total = 0;
    for (auto n: this->data) {
        total += n;
    }
    std::cout << "Part 1: total sum " << std::to_string(total) << std::endl;
}

int main() {
    IntVectorExercise exerciseRunner(0);
    std::cout << "Example exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
