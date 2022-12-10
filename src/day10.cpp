#include <iostream>
#include <vector>
#include <exercise.hpp>

using namespace exercise;


bool isRelevantCycle(int cycleNo) 
{
    return (cycleNo - 20) % 40 == 0;
}

int addCycle2(int& reg, int& cycleNo, int v)
{
    cycleNo += 1;
    reg += v;
    if (isRelevantCycle(cycleNo)) return cycleNo * reg;
    else return 0;
}

int noopOrAddCycle1(int reg, int& cycleNo) 
{ 
    cycleNo += 1;
    if (isRelevantCycle(cycleNo)) return cycleNo * reg;
    else return 0;
}

bool isHit(int reg, int cycleNo, int screenWidth)
{
    int horizontalCRTPos(cycleNo % screenWidth - 1 < 0 ? 39: cycleNo % screenWidth - 1);
    return (
        reg - 1 <= horizontalCRTPos && reg + 1 >= horizontalCRTPos
    );
}

void printStep(int reg, int cycleNo, int screenWidth)
{
    if (isHit(reg, cycleNo, screenWidth)) std::cout << '#';
    else std::cout << '.';
    if (cycleNo % screenWidth == 0) std::cout << std::endl;
}

template <>
void StringVectorExercise::runPart1()
{
    int signalStrengthSum(0), reg(1), cycleNo(1);
    for (auto s: this -> data)
    {
        if (s.substr(0, 4) == "addx") {
            signalStrengthSum += noopOrAddCycle1(reg, cycleNo);
            signalStrengthSum += addCycle2(reg, cycleNo, std::stoi(s.substr(5, s.length())));
        }
        else signalStrengthSum += noopOrAddCycle1(reg, cycleNo);
    }
    std::cout << "Part 1: Total signal strength " << std::to_string(signalStrengthSum) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::cout << "Part 2: CRT image" << std::endl;
    int reg(1), cycleNo(1), screenWidth(40);
    printStep(reg, cycleNo, screenWidth);
    for (auto s: this -> data)
    {
        if (s.substr(0, 4) == "addx") {
            noopOrAddCycle1(reg, cycleNo);
            printStep(reg, cycleNo, screenWidth);
            addCycle2(reg, cycleNo, std::stoi(s.substr(5, s.length())));
            printStep(reg, cycleNo, screenWidth);
        }
        else 
        {
            noopOrAddCycle1(reg, cycleNo);
            printStep(reg, cycleNo, screenWidth);
        };
    }
}

int main() {
    StringVectorExercise exerciseRunner(10);
    std::cout << "Day 10 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
