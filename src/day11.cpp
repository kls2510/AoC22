#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <map>
#include <exercise.hpp>

using namespace exercise;


class Monkey
{
    private:
        std::queue<long> items = std::queue<long>();
        std::function<long(long, int)> transform;
        int otherParam = 0;
        std::shared_ptr<Monkey> monkeyTrue = nullptr;
        std::shared_ptr<Monkey> monkeyFalse = nullptr;
        long numInspections = 0;

        void parseFormula(std::string operationStr)
        {
            int indexFirstSpace(operationStr.find_first_of(' '));
            std::string leftOp(operationStr.substr(0, indexFirstSpace));
            char comb(operationStr[indexFirstSpace + 1]);
            std::string rightOp(operationStr.substr(indexFirstSpace + 3, operationStr.length()));
            if (leftOp == "old" && rightOp == "old") 
            {
                if (comb == '+') this -> transform = [](long worry, int other) { return worry + worry; };
                else this -> transform = [](long worry, int other) { return worry * worry; };
            }
            else
            {
                this -> otherParam = leftOp == "old" ? std::stoi(rightOp): std::stoi(leftOp);
                if (comb == '+') this -> transform = [](long worry, int other) { return worry + other; };
                else this -> transform = [](long worry, int other) { return worry * other; };
            }
        }

        void pushItem(long prio)
        {
            (this -> items).push(prio);
        }

    public:
        const int divisibleTest;

        Monkey(
            std::string operationStr,
            std::string divisibleTestString,
            std::string initialItems
        ) : divisibleTest(std::stoi(divisibleTestString))
        {
            this -> parseFormula(operationStr);
            std::istringstream ss;
            ss = std::istringstream(initialItems); 
            std::string word;
            while (ss >> word) this -> pushItem(
                std::stoi(word.substr(0, word.find(",") != -1 ? word.length() - 1: word.length()))
            );
        }

        void setPassOnMonkeys(
            std::shared_ptr<Monkey> monkeyTrue,
            std::shared_ptr<Monkey> monkeyFalse
        )
        {
            this -> monkeyTrue = monkeyTrue;
            this -> monkeyFalse = monkeyFalse;
        }

        long runMonkeyTurn(bool incRelief, long mod = 1)
        {
            long numToInspect((this -> items).size());
            while (!(this -> items).empty())
            {
                long itemWorry = (this -> items).front();
                (this -> items).pop();
                long newWorry = incRelief ? (this -> transform)(itemWorry, this -> otherParam) / 3: (this -> transform)(itemWorry, this -> otherParam) % mod;
                if (newWorry % (this -> divisibleTest) == 0) (this -> monkeyTrue) -> pushItem(newWorry);
                else (this -> monkeyFalse) -> pushItem(newWorry);
            }
            this -> numInspections += numToInspect;
            return this -> numInspections;
        } 

        void print()
        {
            std::queue<long> copy = this -> items;
            while (!copy.empty())
            {
                long i = copy.front();
                copy.pop();
                std::cout << std::to_string(i) << " ";
            }
            std::cout << std::endl;
        }

};

std::vector<std::shared_ptr<Monkey>> parseMonkeys(std::vector<std::string> lines)
{
    int lineIndex(0);
    std::vector<std::shared_ptr<Monkey>> monkeys = std::vector<std::shared_ptr<Monkey>>();
    std::map<int, std::pair<int, int>> children = std::map<int, std::pair<int, int>>();
    while (lineIndex < lines.size())
    {
        std::string itemLine = lines[lineIndex + 1];
        std::string items = itemLine.substr(itemLine.find_first_of(':') + 2, itemLine.length());
        std::string opLine = lines[lineIndex + 2];
        std::string op = opLine.substr(opLine.find_first_of('=') + 2, opLine.length());
        std::string divLine = lines[lineIndex + 3];
        std::string div = divLine.substr(divLine.find_last_of(' ') + 1, divLine.length());
        monkeys.push_back(
            std::make_shared<Monkey>(
                op, div, items
            )
        );
        std::string p1Line = lines[lineIndex + 4];
        int p1 = std::stoi(p1Line.substr(p1Line.find_last_of(' ') + 1, p1Line.length()));
        std::string p2Line = lines[lineIndex + 5];
        int p2 = std::stoi(p2Line.substr(p2Line.find_last_of(' ') + 1, p2Line.length()));
        children[monkeys.size() - 1] = std::make_pair(p1, p2);
        lineIndex += 7;
    }
    for (auto childInfo: children)
    {
        monkeys[childInfo.first] -> setPassOnMonkeys(
            monkeys[childInfo.second.first],
            monkeys[childInfo.second.second]
        );
    }
    return monkeys;
}

template <>
void StringVectorExercise::runPart1()
{
    int monkeyBusiness, numRounds(20);
    std::vector<std::shared_ptr<Monkey>> monkeys = parseMonkeys(this -> data);
    std::vector<int> monkeyInspections = std::vector<int>(monkeys.size());
    for (int round = 0; round < numRounds; round++)
    {
        int monkeyNo(0);
        for (auto monkey: monkeys) 
        {
            monkeyInspections[monkeyNo] = monkey -> runMonkeyTurn(true);
            monkeyNo++;
        }    
    }
    std::sort(monkeyInspections.begin(), monkeyInspections.end());
    monkeyBusiness = monkeyInspections[monkeyInspections.size() - 2] * monkeyInspections[monkeyInspections.size() - 1];
    std::cout << "Part 1: Monkey business " << std::to_string(monkeyBusiness) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    long monkeyBusiness;
    int numRounds(10000);
    std::vector<std::shared_ptr<Monkey>> monkeys = parseMonkeys(this -> data);
    long mod(1);
    for (auto monkey: monkeys) mod *= monkey -> divisibleTest;
    std::vector<long> monkeyInspections = std::vector<long>(monkeys.size());
    for (int round = 0; round < numRounds; round++)
    {
        int monkeyNo(0);
        for (auto monkey: monkeys) 
        {
            monkeyInspections[monkeyNo] = monkey -> runMonkeyTurn(false, mod);
            monkeyNo++;
        }    
    }
    std::sort(monkeyInspections.begin(), monkeyInspections.end());
    monkeyBusiness = monkeyInspections[monkeyInspections.size() - 2] * monkeyInspections[monkeyInspections.size() - 1];
    std::cout << "Part 2: Monkey business " << std::to_string(monkeyBusiness) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(11);
    std::cout << "Day 11 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
