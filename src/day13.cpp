#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <exercise.hpp>

using namespace exercise;

class PacketNode
{
    private:
        bool hasChildren;
        int i;
        std::vector<std::shared_ptr<PacketNode>> ilist;
    
    public:
        bool isDivider = false;
        PacketNode(int i): hasChildren(false), i(i) {}
        PacketNode(std::vector<std::shared_ptr<PacketNode>> children): hasChildren(true), ilist(children) {}
        bool isList() const { return this -> hasChildren; }
        PacketNode& getValueAt(int pos) const { return *(this -> ilist).at(pos); }
        int getValue() const { return this -> i; }
        int numChildren() const { return (this->ilist).size(); }
        void print()
        {
            if (this->hasChildren)
            {
                std::cout << "[";
                if (this -> numChildren() > 0)
                {
                    for (auto i=0; i<(this->numChildren() - 1); i++)
                    {
                        this->getValueAt(i).print();
                        std::cout << ", ";
                    }
                    this->getValueAt(this->numChildren() - 1).print();
                }
                std::cout << "]";
            }
            else std::cout << this->getValue();
        }
};

//(carry on, isValid)
std::pair<bool, bool> isValid(
    const PacketNode& left, 
    const PacketNode& right
)
{
    if (!(left.isList() || right.isList()))
    {
        int i1(left.getValue()), i2(right.getValue());
        if (i1 < i2) return std::make_pair(false, true);
        if (i1 > i2) return std::make_pair(false, false);
    }
    else if (left.isList() && right.isList())
    {
        int n1(left.numChildren()), n2(right.numChildren());
        for (auto c = 0; c < std::min(n1, n2); c++)
        {
            std::pair<bool, bool> childResult = isValid(
                left.getValueAt(c), right.getValueAt(c)
            );
            if (!childResult.first) return childResult;
        }
        if (n1 < n2) return std::make_pair(false, true);
        if (n1 > n2) return std::make_pair(false, false);
    }
    else
    {
        int n1(left.isList() ? left.numChildren() : 1), n2(right.isList() ? right.numChildren(): 1);
        if ((left.isList() ? n1 : n2) > 0)
        {
            std::pair<bool, bool> childResult = isValid(
                left.isList() ? left.getValueAt(0) : left, 
                right.isList() ? right.getValueAt(0) : right
            );
            if (!childResult.first) return childResult;
        }
        if (n1 < n2) return std::make_pair(false, true);
        if (n1 > n2) return std::make_pair(false, false);
    }
    return std::make_pair(true, false);
}

std::shared_ptr<PacketNode> parsePacket(std::string packetStr)
{
    std::stack<std::shared_ptr<PacketNode>> s = std::stack<std::shared_ptr<PacketNode>>();
    std::stack<int> toPop = std::stack<int>();
    std::string currentNum("");
    for (auto c: packetStr)
    {
        if (std::isdigit(c)) currentNum += c;
        else if (c == ',' && currentNum.length() > 0)
        {
            s.push(std::make_shared<PacketNode>(std::stoi(currentNum)));
            toPop.top()++;
            currentNum = "";
        }
        else if (c == '[') toPop.push(0);
        else if (c == ']')
        {
            if (currentNum.length() > 0)
            {
                s.push(std::make_shared<PacketNode>(std::stoi(currentNum)));
                toPop.top()++;
                currentNum = "";
            }
            std::vector<std::shared_ptr<PacketNode>> v = std::vector<std::shared_ptr<PacketNode>>();
            int popAmt = toPop.top();
            toPop.pop();
            for (auto i=0; i < popAmt; i++)
            {
                v.push_back(s.top());
                s.pop();
            }
            std::reverse(v.begin(), v.end());
            s.push(std::make_shared<PacketNode>(v));
            if (!toPop.empty()) toPop.top()++;
        }
    }
    return s.top();
}


template <>
void StringVectorExercise::runPart1()
{
    std::vector<std::string> lines = this -> data;
    int lineNo(0), sumOfOrderedIndices(0), currentIndex(1);

    while (lineNo < lines.size())
    {
        std::string p1Str(lines[lineNo]), p2Str(lines[lineNo + 1]);
        std::shared_ptr<PacketNode> p1Tree(parsePacket(p1Str)), p2Tree(parsePacket(p2Str));
        if (isValid(*p1Tree, *p2Tree).second) sumOfOrderedIndices += currentIndex;
        currentIndex += 1;
        lineNo += 3;
    }
    std::cout << "Part 1: sum of ordered indicies " << std::to_string(sumOfOrderedIndices) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::vector<std::string> lines = this -> data;
    std::shared_ptr<PacketNode> divider1(parsePacket("[[2]]"));
    divider1->isDivider = true;
    std::shared_ptr<PacketNode> divider2(parsePacket("[[6]]"));
    divider2->isDivider = true;
    std::vector<std::shared_ptr<PacketNode>> parsedPackets = std::vector<std::shared_ptr<PacketNode>>();
    parsedPackets.push_back(divider1);
    parsedPackets.push_back(divider2);
    int lineNo(0), result(1), currentIndex(1);

    while (lineNo < lines.size())
    {
        std::string p1Str(lines[lineNo]), p2Str(lines[lineNo + 1]);
        std::shared_ptr<PacketNode> p1Tree(parsePacket(p1Str)), p2Tree(parsePacket(p2Str));
        parsedPackets.push_back(p1Tree);
        parsedPackets.push_back(p2Tree);
        lineNo += 3;
    }
    std::sort(
        parsedPackets.begin(),
        parsedPackets.end(),
        [](const std::shared_ptr<PacketNode>& lhs, const std::shared_ptr<PacketNode>& rhs) {
            return isValid(*lhs, *rhs).second;
        }
    );
    for (auto n: parsedPackets)
    {
        if (n -> isDivider) result *= currentIndex;
        currentIndex++;
    }
    std::cout << "Part 2: multiplication of divider indexes " << std::to_string(result) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(13);
    std::cout << "Day 13 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
