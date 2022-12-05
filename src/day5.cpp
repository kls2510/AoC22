#include <iostream>
#include <vector>
#include <sstream>
#include <exercise.hpp>

using namespace exercise;

std::pair<std::vector<std::stack<char>>, int> parseStacks(std::vector<std::string> lines)
{
    std::vector<std::stack<char>> stacks;
    int index(0);
    bool isNumberLine;
    std::istringstream ss;
    std::string word;

    // find line with numbers of stacks in it
    while (index < lines.size()) {
        ss = std::istringstream(lines[index]); 
        isNumberLine = true;
        while (ss >> word) {
            if (!isdigit(atoi(word.c_str()))) isNumberLine = false;
        }
        if (isNumberLine) {
            for (auto i=0; i < std::stoi(word); i++) stacks.push_back(std::stack<char>());
            break;
        }
        index++;
    }

    // read each line to fill stacks
    int stackLinesIndex(index - 2);
    int offset;
    std::string line;
    char content;
    while (stackLinesIndex >= 0) {
        offset = 1;
        line = lines[stackLinesIndex];
        for (auto& stack: stacks) {
            content = line[offset];
            if (content != ' ') {
                stack.push(content);
            }
            offset += 4;
        }
        stackLinesIndex--;
    }

    return std::make_pair(stacks, index++);
}

// (amount, from, to)
std::vector<std::tuple<int, int, int>> parseInstructions(
    std::vector<std::string> lines,
    int start
)
{
    std::vector<std::tuple<int, int, int>> instructions = std::vector<std::tuple<int, int, int>>();
    int index(start + 1);
    std::istringstream ss;
    std::string word;
    int wordNo, i1, i2, i3;

    while (index < lines.size()) {
        ss = std::istringstream(lines[index]);
        wordNo = 0;
        while (ss >> word) {
            if (wordNo == 1) i1 = std::stoi(word);
            else if (wordNo == 3) i2 = std::stoi(word) - 1;
            else if (wordNo == 5) i3 = std::stoi(word) - 1;
            wordNo++;
        }
        instructions.push_back(std::make_tuple(i1, i2, i3));
        index++;
    }

    return instructions;
}

template <>
void StringVectorExercise::runPart1()
{
    std::pair<std::vector<std::stack<char>>, int> stackInfo = parseStacks(this->data);
    std::vector<std::tuple<int, int, int>> instructions = parseInstructions(this->data, std::get<1>(stackInfo));
    std::vector<std::stack<char>> stackState = std::get<0>(stackInfo);
    int amt, from, to;
    for (auto i: instructions) {
        amt = std::get<0>(i);
        from = std::get<1>(i);
        to = std::get<2>(i);
        for(auto j=0; j < amt; j++) {
            stackState[to].push(stackState[from].top());
            stackState[from].pop();
        }
    }
    std::cout << "Part 1: Tops of stacks state ";
    for (auto s: stackState) std::cout << s.top();
    std::cout << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::pair<std::vector<std::stack<char>>, int> stackInfo = parseStacks(this->data);
    std::vector<std::tuple<int, int, int>> instructions = parseInstructions(this->data, std::get<1>(stackInfo));
    std::vector<std::stack<char>> stackState = std::get<0>(stackInfo);
    std::stack<char> tmpStack;
    int amt, from, to;
    for (auto i: instructions) {
        amt = std::get<0>(i);
        from = std::get<1>(i);
        to = std::get<2>(i);
        for(auto j=0; j < amt; j++) {
            tmpStack.push(stackState[from].top());
            stackState[from].pop();
        }
        while (!tmpStack.empty()) {
            stackState[to].push(tmpStack.top());
            tmpStack.pop();
        }
    }
    std::cout << "Part 2: Tops of stacks state ";
    for (auto s: stackState) std::cout << s.top();
    std::cout << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(5);
    std::cout << "Day 5 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
