#include <iostream>
#include <set>
#include <queue>
#include <unordered_map>
#include <exercise.hpp>

using namespace exercise;


class Instruction
{
    public:
        bool isDir;
        int numSteps;
        char dir;

        Instruction(int steps): isDir(false), numSteps(steps) {};
        Instruction(char dir): isDir(true), dir(dir) {};
};

class MapRectangle
{
    public:
        int numberFromTop;
        std::pair<int, int> topLeft;
        std::pair<int, int> bottomRight;
        std::set<std::pair<int, int>> walls = std::set<std::pair<int, int>>();

        MapRectangle() {}
        MapRectangle(int rowStart, int colStart, int numberFromTop): topLeft(std::make_pair(rowStart, colStart)), numberFromTop(numberFromTop) {}
        void addWall(int row, int col) { this -> walls.insert(std::make_pair(row, col)); }
        void setEnd(int rowEnd, int colEnd) { this -> bottomRight = std::make_pair(rowEnd, colEnd); }
        void print()
        {
            std::cout << "Grid number " << this -> numberFromTop << std::endl;
            std::cout << "Starts (" << this -> topLeft.first << ", " << this -> topLeft.second << ")" << std::endl;
            std::cout << "Ends (" << this -> bottomRight.first << ", " << this -> bottomRight.second << ")" << std::endl;
            std::cout << "Has " << this -> walls.size() << " walls" << std::endl;
            std::cout << std::endl;
        }
        // return (new grid index, new coordinates, new direction)
        std::tuple<int, std::pair<int, int>, int> moveByInstruction(
            std::unordered_map<int, MapRectangle>& grids,
            Instruction inst,
            std::pair<int, int> currentPosition,
            int currentDirection
        ) {
            if (inst.isDir)
            {
                if (inst.dir == 'L') return std::make_tuple(
                    this -> numberFromTop, 
                    currentPosition, 
                    currentDirection - 1 == -1 ? 3: currentDirection - 1
                );
                else return (
                    std::make_tuple(
                        this -> numberFromTop, 
                        currentPosition, 
                        currentDirection + 1 == 4 ? 0: currentDirection + 1
                    )
                );
            }
            else
            {
                int distance(inst.numSteps);
                std::pair<int, int> nextPos(currentPosition);
                int nextGrid(this -> numberFromTop);
                if (currentDirection == 0 || currentDirection == 2)
                {
                    while (distance > 0) {
                        int nextCol = nextPos.second + (currentDirection == 0 ? 1: -1);
                        if (nextCol == this -> bottomRight.second + 1) nextCol = this -> topLeft.second;
                        else if (nextCol == this -> topLeft.second - 1) nextCol = this -> bottomRight.second;
                        if (this -> walls.contains(std::make_pair(
                            currentPosition.first,
                            nextCol
                        ))) break;
                        nextPos = std::make_pair(
                            currentPosition.first,
                            nextCol
                        );
                        distance--;
                    }
                }
                else
                {
                    while (distance > 0) {
                        int nextRow = nextPos.first + (currentDirection == 1 ? 1: -1);
                        if (nextRow >= this -> topLeft.first && nextRow <= this -> bottomRight.first)
                        {
                            if (this -> walls.contains(std::make_pair(
                                nextRow,
                                currentPosition.second
                            ))) break;
                            nextPos = std::make_pair(
                                nextRow,
                                currentPosition.second
                            );
                            distance--;
                        }
                        else
                        {
                            if (currentDirection == 1)
                            {
                                for (int i = 0; i < grids.size(); i++)
                                {
                                    MapRectangle toTry = grids.at((i + this -> numberFromTop + 1) % grids.size());
                                    if (currentPosition.second <= toTry.bottomRight.second && currentPosition.second >= toTry.topLeft.second) {
                                        if (toTry.walls.contains(
                                            std::make_pair(toTry.topLeft.first, currentPosition.second)
                                        )) {
                                            distance = 0;
                                            break;
                                        }
                                        std::tuple<int, std::pair<int, int>, int> result = toTry.moveByInstruction(
                                            grids, Instruction(distance - 1), std::make_pair(toTry.topLeft.first, currentPosition.second), currentDirection
                                        );
                                        nextGrid = std::get<0>(result);
                                        nextPos = std::get<1>(result);
                                        distance = 0;
                                        break;
                                    }
                                }
                            }
                            else {
                                for (int i = grids.size() - 1; i >= 0; i--)
                                {
                                    MapRectangle toTry = grids.at((i + this -> numberFromTop) % grids.size());
                                    if (currentPosition.second <= toTry.bottomRight.second && currentPosition.second >= toTry.topLeft.second) {
                                        if (toTry.walls.contains(
                                            std::make_pair(toTry.bottomRight.first, currentPosition.second)
                                        )) {
                                            distance = 0;
                                            break;
                                        }
                                        std::tuple<int, std::pair<int, int>, int> result = toTry.moveByInstruction(
                                            grids, Instruction(distance - 1), std::make_pair(toTry.bottomRight.first, currentPosition.second), currentDirection
                                        );
                                        nextGrid = std::get<0>(result);
                                        nextPos = std::get<1>(result);
                                        distance = 0;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                return std::make_tuple(nextGrid, nextPos, currentDirection);
            }
        }

        // return (new grid index, new coordinates, new direction)
        std::tuple<int, std::pair<int, int>, int> moveByInstructionInCube(
            std::unordered_map<int, MapRectangle>& grids,
            Instruction inst,
            std::pair<int, int> currentPosition,
            int currentDirection
        ) {
            if (inst.isDir)
            {
                if (inst.dir == 'L') return std::make_tuple(
                    this -> numberFromTop, 
                    currentPosition, 
                    currentDirection - 1 == -1 ? 3: currentDirection - 1
                );
                else return (
                    std::make_tuple(
                        this -> numberFromTop, 
                        currentPosition, 
                        currentDirection + 1 == 4 ? 0: currentDirection + 1
                    )
                );
            }
            else
            {
                int distance(inst.numSteps);
                std::pair<int, int> nextPos(currentPosition);
                int nextGrid(this -> numberFromTop);
                int nextDir(currentDirection);
                if (currentDirection == 0 || currentDirection == 2)
                {
                    while (distance > 0) {
                        int nextCol = nextPos.second + (currentDirection == 0 ? 1: -1);
                        if (nextCol >= this -> topLeft.second && nextCol <= this -> bottomRight.second)
                        {
                            if (this -> walls.contains(std::make_pair(
                                currentPosition.first,
                                nextCol
                            ))) break;
                            nextPos = std::make_pair(
                                currentPosition.first,
                                nextCol
                            );
                            distance--;
                        }
                        else
                        {
                            MapRectangle toTry;
                            int newDir;
                            std::pair<int, int> newStart;

                            int distanceFromTop(currentPosition.first - this -> topLeft.first);
                            int distanceFromLeft(currentPosition.second - this -> topLeft.second);
                            switch (this -> numberFromTop)
                            {
                                case 0: {
                                    if (currentDirection == 0) {
                                        toTry = grids.at(1);
                                        newDir = 0;
                                        newStart = std::make_pair(currentPosition.first, toTry.topLeft.second);
                                    }
                                    else {
                                        toTry = grids.at(3);
                                        newDir = 0;
                                        newStart = std::make_pair(toTry.bottomRight.first - distanceFromTop, toTry.topLeft.second);
                                    }
                                    break;
                                }
                                case 1: {
                                    if (currentDirection == 0) {
                                        toTry = grids.at(4);
                                        newDir = 2;
                                        newStart = std::make_pair(toTry.bottomRight.first - distanceFromTop, toTry.bottomRight.second);
                                    }
                                    else {
                                        toTry = grids.at(0);
                                        newDir = 2;
                                        newStart = std::make_pair(currentPosition.first, toTry.bottomRight.second);
                                    }
                                    break;
                                }
                                case 2: {
                                    if (currentDirection == 0) {
                                        toTry = grids.at(1);
                                        newDir = 3;
                                        newStart = std::make_pair(toTry.bottomRight.first, toTry.topLeft.second + distanceFromTop);
                                    }
                                    else {
                                        toTry = grids.at(3);
                                        newDir = 1;
                                        newStart = std::make_pair(toTry.topLeft.first, toTry.topLeft.second + distanceFromTop);
                                    }
                                    break;
                                }
                                case 3: {
                                    if (currentDirection == 0) {
                                        toTry = grids.at(4);
                                        newDir = 0;
                                        newStart = std::make_pair(currentPosition.first, toTry.topLeft.second);
                                    }
                                    else {
                                        toTry = grids.at(0);
                                        newDir = 0;
                                        newStart = std::make_pair(toTry.bottomRight.first - distanceFromTop, toTry.topLeft.second);
                                    }
                                    break;
                                }
                                case 4: {
                                    if (currentDirection == 0) {
                                        toTry = grids.at(1);
                                        newDir = 2;
                                        newStart = std::make_pair(toTry.bottomRight.first - distanceFromTop, toTry.bottomRight.second);
                                    }
                                    else {
                                        toTry = grids.at(3);
                                        newDir = 2;
                                        newStart = std::make_pair(currentPosition.first, toTry.bottomRight.second);
                                    }
                                    break;
                                }
                                case 5: {
                                    if (currentDirection == 0) {
                                        toTry = grids.at(4);
                                        newDir = 3;
                                        newStart = std::make_pair(toTry.bottomRight.first, toTry.topLeft.second + distanceFromTop);
                                    }
                                    else {
                                        toTry = grids.at(0);
                                        newDir = 1;
                                        newStart = std::make_pair(toTry.topLeft.first, toTry.topLeft.second + distanceFromTop);
                                    }
                                    break;
                                }
                            };
                            if (toTry.walls.contains(newStart)) {
                                distance = 0;
                                break;
                            }
                            std::tuple<int, std::pair<int, int>, int> result = toTry.moveByInstructionInCube(
                                grids, Instruction(distance - 1), newStart, newDir
                            );
                            nextGrid = std::get<0>(result);
                            nextPos = std::get<1>(result);
                            nextDir = std::get<2>(result);
                            distance = 0;
                            break;
                        }
                    }
                }
                else
                {
                    while (distance > 0) {
                        int nextRow = nextPos.first + (currentDirection == 1 ? 1: -1);
                        if (nextRow >= this -> topLeft.first && nextRow <= this -> bottomRight.first)
                        {
                            if (this -> walls.contains(std::make_pair(
                                nextRow,
                                currentPosition.second
                            ))) break;
                            nextPos = std::make_pair(
                                nextRow,
                                currentPosition.second
                            );
                            distance--;
                        }
                        else
                        {
                            MapRectangle toTry;
                            int newDir;
                            std::pair<int, int> newStart;
                            int distanceFromTop(currentPosition.first - this -> topLeft.first);
                            int distanceFromLeft(currentPosition.second - this -> topLeft.second);
                            switch (this -> numberFromTop)
                            {
                                case 0: {
                                    if (currentDirection == 1) {
                                        toTry = grids.at(2);
                                        newDir = 1;
                                        newStart = std::make_pair(toTry.topLeft.first, currentPosition.second);
                                    }
                                    else {
                                        toTry = grids.at(5);
                                        newDir = 0;
                                        newStart = std::make_pair(toTry.topLeft.first + distanceFromLeft, toTry.topLeft.second);
                                    }
                                    break;
                                }
                                case 1: {
                                    if (currentDirection == 1) {
                                        toTry = grids.at(2);
                                        newDir = 2;
                                        newStart = std::make_pair(toTry.topLeft.first + distanceFromLeft, toTry.bottomRight.second);
                                    }
                                    else {
                                        toTry = grids.at(5);
                                        newDir = 3;
                                        newStart = std::make_pair(toTry.bottomRight.first, toTry.topLeft.second + distanceFromLeft);
                                    }
                                    break;
                                }
                                case 2: {
                                    if (currentDirection == 1) {
                                        toTry = grids.at(4);
                                        newDir = 1;
                                        newStart = std::make_pair(toTry.topLeft.first, currentPosition.second);
                                    }
                                    else {
                                        toTry = grids.at(0);
                                        newDir = 3;
                                        newStart = std::make_pair(toTry.bottomRight.first, currentPosition.second);
                                    }
                                    break;
                                }
                                case 3: {
                                    if (currentDirection == 1) {
                                        toTry = grids.at(5);
                                        newDir = 1;
                                        newStart = std::make_pair(toTry.topLeft.first, currentPosition.second);
                                    }
                                    else {
                                        toTry = grids.at(2);
                                        newDir = 0;
                                        newStart = std::make_pair(toTry.topLeft.first + distanceFromLeft, toTry.topLeft.second);
                                    }
                                    break;
                                }
                                case 4: {
                                    if (currentDirection == 1) {
                                        toTry = grids.at(5);
                                        newDir = 2;
                                        newStart = std::make_pair(toTry.topLeft.first + distanceFromLeft, toTry.bottomRight.second);
                                    }
                                    else {
                                        toTry = grids.at(2);
                                        newDir = 3;
                                        newStart = std::make_pair(toTry.bottomRight.first, currentPosition.second);
                                    }
                                    break;
                                }
                                case 5: {
                                    if (currentDirection == 1) {
                                        toTry = grids.at(1);
                                        newDir = 1;
                                        newStart = std::make_pair(toTry.topLeft.first, toTry.topLeft.second + distanceFromLeft);
                                    }
                                    else {
                                        toTry = grids.at(3);
                                        newDir = 3;
                                        newStart = std::make_pair(toTry.bottomRight.first, currentPosition.second);
                                    }
                                    break;
                                }
                            };
                            if (toTry.walls.contains(newStart)) {
                                distance = 0;
                                break;
                            }
                            std::tuple<int, std::pair<int, int>, int> result = toTry.moveByInstructionInCube(
                                grids, Instruction(distance - 1), newStart, newDir
                            );
                            nextGrid = std::get<0>(result);
                            nextPos = std::get<1>(result);
                            nextDir = std::get<2>(result);
                            distance = 0;
                            break;
                        }
                    }
                }
                return std::make_tuple(nextGrid, nextPos, nextDir);
            }
        }
};


std::pair<std::unordered_map<int, MapRectangle>, std::queue<Instruction>> parseGrids(std::vector<std::vector<char>> lines)
{
    std::unordered_map<int, MapRectangle> grids = std::unordered_map<int, MapRectangle>();
    
    int previousColStart(0), previousColEnd(0), currentColStart, currentColEnd;
    MapRectangle currentMapRectangle;
    int rectangleNo(0), rowNo(1);
    for (auto l: lines)
    {
        if (l.size() == 0)
        {
            currentMapRectangle.setEnd(rowNo - 1, currentColEnd);
            grids.insert(std::make_pair(currentMapRectangle.numberFromTop, currentMapRectangle));
            break;
        }
        int charOffset(1), colStart, colEnd;
        while (l[charOffset - 1] == ' ') charOffset++;
        currentColStart = charOffset;
        charOffset = l.size();
        while (l[charOffset - 1] == ' ') charOffset--;
        currentColEnd = charOffset;

        if (previousColStart == 0 && previousColEnd == 0) {
            previousColStart = currentColStart;
            previousColEnd = currentColEnd;
            currentMapRectangle = MapRectangle(rowNo, previousColStart, rectangleNo);
            rectangleNo++;
        } else if (previousColStart != currentColStart || previousColEnd != currentColEnd) {
            currentMapRectangle.setEnd(rowNo - 1, previousColEnd);
            grids.insert(std::make_pair(currentMapRectangle.numberFromTop, currentMapRectangle));
            previousColStart = currentColStart;
            previousColEnd = currentColEnd;
            currentMapRectangle = MapRectangle(rowNo, previousColStart, rectangleNo);
            rectangleNo++;
        }
        charOffset = 1;
        for (auto c: l) {
            if (c == '#') currentMapRectangle.addWall(rowNo, charOffset);
            charOffset++;
        }

        rowNo++;
    }

    std::string currentNumber("");
    std::vector<char> instructions(lines[rowNo]);
    std::queue<Instruction> is = std::queue<Instruction>();
    for (auto c: instructions)
    {
        if (std::isdigit(c)) currentNumber += c;
        else {
            if (currentNumber.length() > 0) is.push(Instruction(std::stoi(currentNumber)));
            currentNumber = "";
            is.push(Instruction(c));
        }
    }
    if (currentNumber.length() > 0) is.push(Instruction(std::stoi(currentNumber)));
    return std::make_pair(grids, is);
}

template <>
void CharGridExercise::runPart1()
{
    std::pair<std::unordered_map<int, MapRectangle>, std::queue<Instruction>> gridsAndInstructions = parseGrids(this -> data);
    
    std::unordered_map<int, MapRectangle> grids = gridsAndInstructions.first;
    std::queue<Instruction> q = gridsAndInstructions.second;
    std::tuple<int, std::pair<int, int>, int> result = std::make_tuple(0, grids.at(0).topLeft, 0);
    while (!q.empty())
    {
        Instruction inst = q.front();
        q.pop();
        result = grids.at(std::get<0>(result)).moveByInstruction(
            grids,
            inst,
            std::get<1>(result),
            std::get<2>(result)
        );
    }
    std::cout << "Part 1: Password is " << 1000 * std::get<1>(result).first + 4 * std::get<1>(result).second + std::get<2>(result) << std::endl;
}

template <>
void CharGridExercise::runPart2()
{
    std::pair<std::unordered_map<int, MapRectangle>, std::queue<Instruction>> gridsAndInstructions = parseGrids(this -> data);
    
    std::unordered_map<int, MapRectangle> grids = gridsAndInstructions.first;
    std::unordered_map<int, MapRectangle> squares = std::unordered_map<int, MapRectangle>();
    int faceNo(0);
    for (auto k = 0; k < grids.size(); k++) {
        MapRectangle g = grids.at(k);
        for (int i = g.topLeft.second; i <= g.bottomRight.second; i += 50) {
            MapRectangle n = MapRectangle(g.topLeft.first, i, faceNo);
            n.setEnd(g.bottomRight.first, i + 49);
            for (auto c: g.walls)
            {
                if (c.second <= n.bottomRight.second && c.second >= n.topLeft.second) n.addWall(c.first, c.second);
            }
            squares.insert(std::make_pair(faceNo, n));
            faceNo ++;
        }
    }

    std::queue<Instruction> q = gridsAndInstructions.second;
    std::tuple<int, std::pair<int, int>, int> result = std::make_tuple(0, squares.at(0).topLeft, 0);
    while (!q.empty())
    {
        Instruction inst = q.front();
        q.pop();
        result = squares.at(std::get<0>(result)).moveByInstructionInCube(
            squares,
            inst,
            std::get<1>(result),
            std::get<2>(result)
        );
    }
    std::cout << "Part 2: Password is " << 1000 * std::get<1>(result).first + 4 * std::get<1>(result).second + std::get<2>(result) << std::endl;
}


int main() {
    CharGridExercise exerciseRunner(22);
    std::cout << "Day 22 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}