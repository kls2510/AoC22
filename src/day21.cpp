#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <exercise.hpp>

using namespace exercise;


class Monkey
{
    public:
        std::string name;
        bool runsOp;
        std::pair<std::string, std::string> operands;
        char transformCode;
        std::function<long(long, long)> transform;
        long value;

        Monkey(
            std::string name,
            std::string numberStr
        )
        {
            this -> name = name;
            if (std::isdigit(numberStr[0]))
            {
                this -> runsOp = false;
                this -> value = std::stoi(numberStr);
            }
            else
            {
                this -> runsOp = true;
                std::istringstream ss = std::istringstream(numberStr); 
                std::string word, firstMonkeyName;
                int wordIndex(0);
                while (ss >> word)
                {
                    if (wordIndex == 0) firstMonkeyName = word;
                    if (wordIndex == 1)
                    {
                        this -> transformCode = word[0];
                        switch (this -> transformCode) {
                            case '+': {
                                this -> transform = [](long i1, long i2) { return i1 + i2; };
                                break;
                            }
                            case '-': {
                                this -> transform = [](long i1, long i2) { return i1 - i2; };
                                break;
                            }
                            case '/': {
                                this -> transform = [](long i1, long i2) { return i1 / i2; };
                                break;
                            }
                            case '*': {
                                this -> transform = [](long i1, long i2) { return i1 * i2; };
                                break;
                            }
                        };
                    }
                    if (wordIndex == 2) this -> operands = std::make_pair(firstMonkeyName, word);
                    wordIndex++;
                }
            }
        }

        long evaluate(
            const std::unordered_map<std::string, Monkey>& monkeys
        ) const
        {
            if (this -> runsOp) return this -> transform(
                monkeys.at(this -> operands.first).evaluate(monkeys),
                monkeys.at(this -> operands.second).evaluate(monkeys)
            );
            else return this -> value;
        }

        void setValue(long i) { this -> value = i; }

        bool humnIsAncestor(
            const std::unordered_map<std::string, Monkey>& monkeys
        ) const
        {
            if (this -> runsOp)
            {
                return (
                    monkeys.at(this -> operands.first).humnIsAncestor(monkeys)
                    || monkeys.at(this -> operands.second).humnIsAncestor(monkeys)
                );
            }
            else return this -> name == "humn";
        }

        long workBackFromTarget(
            long target,
            const std::unordered_map<std::string, Monkey>& monkeys
        ) const
        {
            if (this -> runsOp) {
                long newTarget;
                bool humnInOp1 = monkeys.at(this -> operands.first).humnIsAncestor(monkeys);
                long otherOpVal = monkeys.at(humnInOp1 ? this -> operands.second: this -> operands.first).evaluate(monkeys);
                switch (this -> transformCode) {
                    case '+': {
                        // target = op1 + op2
                        // op1 = target - op2 and op2 = target - op1
                        newTarget = target - otherOpVal;
                        break;
                    }
                    case '-': {
                        // target = op1 - op2
                        // op1 = target + op2 and op2 = op1 - target
                        newTarget = humnInOp1 ? target + otherOpVal : otherOpVal - target;
                        break;
                    }
                    case '/': {
                        // target = op1 / op2
                        // op1 = target * op2 and op2 = op1 / target
                        newTarget = humnInOp1 ? target * otherOpVal : otherOpVal / target;
                        break;
                    }
                    case '*': {
                        // target = op1 * op2
                        // op1 = target / op2 and op2 = target / op1
                        newTarget = target / otherOpVal;
                        break;
                    }
                };
                return monkeys.at(humnInOp1 ? this -> operands.first: this -> operands.second).workBackFromTarget(newTarget, monkeys);
            }
            else return target;
        }

};

std::unordered_map<std::string, Monkey> parseMonkeys(std::vector<std::string> lines)
{
    std::unordered_map<std::string, Monkey> monkeys = std::unordered_map<std::string, Monkey>();
    for (auto l: lines)
    {
        int splitIndex(l.find_first_of(' '));
        std::string name = l.substr(0, splitIndex - 1);
        monkeys.insert(
            std::make_pair(name, Monkey(name, l.substr(splitIndex + 1, l.length())))
        );
    }
    return monkeys;
}

template <>
void StringVectorExercise::runPart1()
{
    std::unordered_map<std::string, Monkey> monkeys = parseMonkeys(this -> data);
    std::cout << "Part 1: Root calls " << monkeys.at("root").evaluate(monkeys) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::unordered_map<std::string, Monkey> monkeys = parseMonkeys(this -> data);
    Monkey rootParent1 = monkeys.at(monkeys.at("root").operands.first);
    Monkey rootParent2 = monkeys.at(monkeys.at("root").operands.second);

    bool r1HasHumnAncestor = rootParent1.humnIsAncestor(monkeys);
    long targetVal = r1HasHumnAncestor ? rootParent2.evaluate(monkeys): rootParent1.evaluate(monkeys);
    long humnVal = r1HasHumnAncestor ? rootParent1.workBackFromTarget(targetVal, monkeys): rootParent2.workBackFromTarget(targetVal, monkeys); 

    std::cout << "Part 2: I call " << humnVal << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(21);
    std::cout << "Day 21 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
