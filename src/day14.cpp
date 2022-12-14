#include <iostream>
#include <sstream>
#include <set>
#include <list>
#include <exercise.hpp>

using namespace exercise;

class Line
{
    private:
        // x across, y down
        std::pair<int, int> start;
        std::pair<int, int> end;

    public:
        Line(std::string from, std::string to)
        {
            int fromSplit(from.find_first_of(',')), toSplit(to.find_first_of(','));
            this -> start = std::make_pair(
                std::stoi(from.substr(0, fromSplit)), std::stoi(from.substr(fromSplit+1, from.length())) 
            );
            this -> end = std::make_pair(
                std::stoi(to.substr(0, toSplit)), std::stoi(to.substr(toSplit+1, to.length())) 
            );
        }
        void drawLine(
            std::set<std::pair<int, int>>& filled
        )
        {
            if (this->start.first == this->end.first)
            {
                // vertical
                int start(std::min(this->start.second, this->end.second));
                int end(std::max(this->start.second, this->end.second));
                for (auto i = start; i <= end; i++) filled.insert(
                    std::make_pair(this->start.first, i)
                );
            }
            else
            {
                // horizontal
                int start(std::min(this->start.first, this->end.first));
                int end(std::max(this->start.first, this->end.first));
                for (auto i = start; i <= end; i++) filled.insert(
                    std::make_pair(i, this->start.second)
                );
            }
        }
        int maxY() { return std::max(this->start.second, this->end.second); }
};

std::vector<Line> parseInputLines(std::vector<std::string> inp)
{
    std::istringstream ss;
    std::string word;
    int wordIndex;
    std::list<std::string> q;
    std::vector<Line> lines = std::vector<Line>();

    for (auto l: inp)
    {
        wordIndex = 0;
        ss = std::istringstream(l);
        q = std::list<std::string>();
        while (ss >> word) {
            if (wordIndex % 2 == 0)
            {
                if (q.size() == 2) q.pop_front();
                q.push_back(word);
                if (q.size() == 2) lines.push_back(Line(q.front(), q.back()));
            }
            wordIndex++;
        }
    }

    return lines;
}

std::vector<std::pair<int, int>> getMovesInOrder(std::pair<int, int> start)
{
    return {
        std::make_pair(start.first, start.second + 1),
        std::make_pair(start.first - 1, start.second + 1),
        std::make_pair(start.first + 1, start.second + 1)
    };
}

bool dropSand(
    std::pair<int, int> start,
    int maxY,
    std::set<std::pair<int, int>>& filled,
    bool includeFloor = false
)
{
    bool canMove(true);
    std::pair<int, int> currentPos(start);
    while (canMove && (includeFloor || currentPos.second < maxY))
    {
        std::vector<std::pair<int, int>> nextMoves = getMovesInOrder(currentPos);
        canMove = false;
        for (auto move: nextMoves)
        {
            if (!filled.contains(move) && (!includeFloor || move.second < maxY + 2))
            {
                canMove = true;
                currentPos = move;
                break;
            }
        }
    }
    if (!canMove) {
        filled.insert(currentPos);
    };
    return canMove;
}

template <>
void StringVectorExercise::runPart1()
{
    std::set<std::pair<int, int>> filled = std::set<std::pair<int, int>>();
    std::pair<int, int> sandStart = std::make_pair(500, 0);
    int numUnitsSand(0), maxY(0);
    std::vector<Line> lines = parseInputLines(this -> data);
    for (auto line: lines)
    {
        line.drawLine(filled);
        maxY = std::max(maxY, line.maxY());
    }

    while ( !dropSand(sandStart, maxY, filled) ) numUnitsSand++;

    std::cout << "Part 1: Number grains of falling sand " << std::to_string(numUnitsSand) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::set<std::pair<int, int>> filled = std::set<std::pair<int, int>>();
    std::pair<int, int> sandStart = std::make_pair(500, 0);
    int numUnitsSand(0), maxY(0);
    std::vector<Line> lines = parseInputLines(this -> data);
    for (auto line: lines)
    {
        line.drawLine(filled);
        maxY = std::max(maxY, line.maxY());
    }

    while ( !filled.contains(sandStart) ) 
    {
        dropSand(sandStart, maxY, filled, true);
        numUnitsSand++;
    };

    std::cout << "Part 2: Number grains of falling sand with floor " << std::to_string(numUnitsSand) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(14);
    std::cout << "Day 14 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
