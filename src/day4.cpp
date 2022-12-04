#include <iostream>
#include <exercise.hpp>

using namespace exercise;

std::tuple<int, int, int, int> parseElfPair(std::string line)
{
    int indexBetweenElves = line.find(',');
    int indexFirstElfSplit = line.find_first_of('-');
    int indexSecondElfSplit = line.find_last_of('-');
    return std::make_tuple(
        std::stoi(line.substr(0, indexFirstElfSplit)),
        std::stoi(line.substr(indexFirstElfSplit + 1, indexBetweenElves)),
        std::stoi(line.substr(indexBetweenElves + 1, indexSecondElfSplit)),
        std::stoi(line.substr(indexSecondElfSplit + 1, line.length()))
    );
}

std::tuple<std::tuple<int, int>, std::tuple<int, int>> getOrderedAssignments(
    std::tuple<int, int, int, int> pairAssignments
)
{
    int elf1Start = std::get<0>(pairAssignments);
    int elf1End = std::get<1>(pairAssignments);
    int elf2Start = std::get<2>(pairAssignments);
    int elf2End = std::get<3>(pairAssignments);
    std::tuple<int, int> elf1Assignment = std::make_tuple(elf1Start, elf1End);
    std::tuple<int, int> elf2Assignment = std::make_tuple(elf2Start, elf2End);

    std::tuple<int, int> earlierAssignment = (
        elf1Start < elf2Start || (elf1Start == elf2Start && elf1End > elf2End)
    ) ? elf1Assignment: elf2Assignment;
    std::tuple<int, int> laterAssignment = (
        earlierAssignment == elf1Assignment
    ) ? elf2Assignment : elf1Assignment;

    return std::make_tuple(
        earlierAssignment, laterAssignment
    );
}

template <>
void StringVectorExercise::runPart1()
{
    int currentTotal(0);
    for (auto n: this->data) {
        std::tuple<int, int, int, int> pairAssignments = parseElfPair(n);
        std::tuple<std::tuple<int, int>, std::tuple<int, int>> orderedAssignments = getOrderedAssignments(pairAssignments);
        std::tuple<int, int> earlierAssignment = std::get<0>(orderedAssignments);
        std::tuple<int, int> laterAssignment = std::get<1>(orderedAssignments);

        int earlierEnd = std::get<1>(earlierAssignment);
        int lateStart = std::get<0>(laterAssignment);
        int lateEnd = std::get<1>(laterAssignment);
        if (lateStart <= earlierEnd && lateEnd <= earlierEnd) currentTotal++;
    }
    std::cout << "Part 1: Number completely overlapped " << std::to_string(currentTotal) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    int currentTotal(0);
    for (auto n: this->data) {
        std::tuple<int, int, int, int> pairAssignments = parseElfPair(n);
        std::tuple<std::tuple<int, int>, std::tuple<int, int>> orderedAssignments = getOrderedAssignments(pairAssignments);
        std::tuple<int, int> earlierAssignment = std::get<0>(orderedAssignments);
        std::tuple<int, int> laterAssignment = std::get<1>(orderedAssignments);

        int earlierEnd = std::get<1>(earlierAssignment);
        int lateStart = std::get<0>(laterAssignment);
        if (lateStart <= earlierEnd) currentTotal++;
    }
    std::cout << "Part 2: Number overlapping at all " << std::to_string(currentTotal) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(4);
    std::cout << "Day 4 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
