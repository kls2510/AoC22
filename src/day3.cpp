#include <iostream>
#include <set>
#include <exercise.hpp>

using namespace exercise;


template <>
void StringVectorExercise::runPart1()
{
    int currentTotal(0);
    for (auto n: this->data) {
        int middle = n.length() / 2;
        std::string c1 = n.substr(0, middle);
        std::set<char> c1Unique(c1.begin(), c1.end());
        std::string c2 = n.substr(middle, n.length());
        std::set<char> c2Unique(c2.begin(), c2.end());
        std::vector<char> inBoth;
        std::set_intersection(
            c1Unique.begin(),
            c1Unique.end(),
            c2Unique.begin(),
            c2Unique.end(),
            std::back_inserter(inBoth)
        );
        char itemInBoth = inBoth[0]; // safe: guaranteed exactly 1 item type per rucksack
        char asciiOffset = std::isupper(itemInBoth) ? '&' : '`';
        currentTotal += itemInBoth - asciiOffset;
    }
    std::cout << "Part 1: total priority " << std::to_string(currentTotal) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    int currentTotal(0);
    std::set<char> currentSharedItems;
    int elfCounter(0);
    for (auto n: this->data) {
        if (elfCounter % 3 == 0) {
            if (elfCounter > 0) {
                char itemAcrossAll = *currentSharedItems.begin(); // safe: guaranteed exactly 1 item type per elf group
                char asciiOffset = std::isupper(itemAcrossAll) ? '&' : '`';
                currentTotal += itemAcrossAll - asciiOffset;
            }
            currentSharedItems = std::set<char>(n.begin(), n.end());
        }
        else {
            std::set<char> newSharedItems;
            std::set<char> thisElf(n.begin(), n.end());
            std::set_intersection(
                currentSharedItems.begin(),
                currentSharedItems.end(),
                thisElf.begin(),
                thisElf.end(),
                std::inserter(newSharedItems, newSharedItems.begin())
            );
            currentSharedItems = newSharedItems;
        }
        elfCounter++;
    }
    if (elfCounter > 0) {
        char itemAcrossAll = *currentSharedItems.begin(); // safe: guaranteed exactly 1 item type per elf group
        char asciiOffset = std::isupper(itemAcrossAll) ? '&' : '`';
        currentTotal += itemAcrossAll - asciiOffset;
    }
    std::cout << "Part 2: total priority " << std::to_string(currentTotal) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(3);
    std::cout << "Day 3 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
