#include <iostream>
#include <set>
#include <queue>
#include <map>
#include <exercise.hpp>

using namespace exercise;


class Rectangle
{
    private:
        std::pair<long, long> topLeft;
        std::pair<long, long> bottomRight;
    
    public:
        Rectangle(std::pair<long, long> topLeft, std::pair<long, long> bottomRight): topLeft(topLeft), bottomRight(bottomRight) {}
        bool canMoveLeft(std::vector<Rectangle>& allObjectsAtRest)
        {
            if (this -> topLeft.first == 0) return false;
            std::pair<long, long> nextTopLeft(std::make_pair(this -> topLeft.first - 1, this -> topLeft.second));
            std::pair<long, long> nextBottomRight(std::make_pair(this -> bottomRight.first - 1, this -> bottomRight.second));
            return !this->hasCollision(allObjectsAtRest, nextTopLeft, nextBottomRight);
        }
        void moveLeft()
        {
            this -> topLeft = std::make_pair(this -> topLeft.first - 1, this -> topLeft.second);
            this -> bottomRight = std::make_pair(this -> bottomRight.first - 1, this -> bottomRight.second);
        }
        bool canMoveRight(std::vector<Rectangle>& allObjectsAtRest, int maxWidth)
        {
            if (this -> bottomRight.first == maxWidth - 1) return false;
            std::pair<long, long> nextTopLeft(std::make_pair(this -> topLeft.first + 1, this -> topLeft.second));
            std::pair<long, long> nextBottomRight(std::make_pair(this -> bottomRight.first + 1, this -> bottomRight.second));
            return !this->hasCollision(allObjectsAtRest, nextTopLeft, nextBottomRight);
        }
        void moveRight()
        {
            this -> topLeft = std::make_pair(this -> topLeft.first + 1, this -> topLeft.second);
            this -> bottomRight = std::make_pair(this -> bottomRight.first + 1, this -> bottomRight.second);
        }
        bool canMoveDown(std::vector<Rectangle>& allObjectsAtRest, long floor)
        {
            if (this -> bottomRight.second == floor) return false;
            std::pair<long, long> nextTopLeft(std::make_pair(this -> topLeft.first, this -> topLeft.second - 1));
            std::pair<long, long> nextBottomRight(std::make_pair(this -> bottomRight.first, this -> bottomRight.second - 1));
            return !this->hasCollision(allObjectsAtRest, nextTopLeft, nextBottomRight);
        }
        void moveDown()
        {
            this -> topLeft = std::make_pair(this -> topLeft.first, this -> topLeft.second - 1);
            this -> bottomRight = std::make_pair(this -> bottomRight.first, this -> bottomRight.second - 1);
        }
        long getHighestY()
        {
            return this -> topLeft.second;
        }
        long getLowestY()
        {
            return this -> bottomRight.second;
        }
        void draw(std::vector<std::vector<char>>& grid)
        {
            for (long x=this->topLeft.first; x <= this->bottomRight.first; x++)
            {
                for (long y=this->topLeft.second; y >= this->bottomRight.second; y--)
                {
                    grid[grid.size() - 1 - y][x] = '#';
                }
            }
        }
        bool hasCollision(
            const std::vector<Rectangle>& allObjectsAtRest, 
            std::pair<long, long> nextTopLeft,
            std::pair<long, long> nextBottomRight
        )
        {
            for (auto& r: allObjectsAtRest)
            {
                if (!(
                    nextTopLeft.first > r.bottomRight.first
                    || nextBottomRight.first < r.topLeft.first
                    || nextTopLeft.second < r.bottomRight.second
                    || nextBottomRight.second > r.topLeft.second
                )) return true;
            }
            return false;
        }
        std::vector<long> getCoveredX()
        {
            std::vector<long> result = std::vector<long>();
            for (long x=this->topLeft.first; x <= this->bottomRight.first; x++) result.push_back(x);
            return result;
        }
};

class Rock
{
    protected:
        static const int distanceFromLeft = 2;
        static const int distanceFromBottom = 3;

        virtual std::vector<Rectangle> genShape(long currentHighestRow) = 0;

    public:
        std::vector<Rectangle> stationaryObjects = std::vector<Rectangle>();
        std::pair<long, int> drop(
            long currentHighestRow, 
            std::vector<Rectangle>& allObjectsAtRest,
            std::queue<char>& directions,
            int maxWidth = 7,
            long floor = 0
        )
        {
            std::vector<Rectangle> shapeDesc = this -> genShape(currentHighestRow);
            bool cont(true);
            int numInstructions(0);
            while (cont)
            {
                numInstructions++;
                char dir(directions.front());
                directions.pop();
                directions.push(dir);
                if (dir == '<') 
                {
                    bool canMoveLeft(true);
                    for (auto& r: shapeDesc) canMoveLeft &= r.canMoveLeft(allObjectsAtRest);
                    if (canMoveLeft) for (auto& r: shapeDesc) r.moveLeft();
                }
                else
                {
                    bool canMoveRight(true);
                    for (auto& r: shapeDesc) canMoveRight &= r.canMoveRight(allObjectsAtRest, maxWidth);
                    if (canMoveRight) for (auto& r: shapeDesc) r.moveRight();
                }
                for (auto& r: shapeDesc) cont &= r.canMoveDown(allObjectsAtRest, floor);
                if (cont) for (auto& r: shapeDesc) r.moveDown();
            }
            long maxHeight(currentHighestRow);
            this -> stationaryObjects.clear();
            for (auto r: shapeDesc)
            {
                this -> stationaryObjects.push_back(r);
                allObjectsAtRest.push_back(r);
                maxHeight = std::max(maxHeight, r.getHighestY() + 1);
            }
            return std::make_pair(maxHeight, numInstructions);
        }

        void draw(std::vector<std::vector<char>>& grid)
        {
            std::vector<Rectangle> shapeDesc = this -> genShape(0);
            for (auto r: shapeDesc) r.draw(grid);
        }
};

class RockDash : public Rock
{
    // ####
    private:
        std::vector<Rectangle> genShape(long currentHighestRow)
        {
            long xStart(this -> distanceFromLeft), xEnd(this -> distanceFromLeft + 3), y(currentHighestRow + this -> distanceFromBottom);
            return {
                Rectangle(std::pair(xStart, y), std::pair(xEnd, y))
            };
        }
};

class RockPlus : public Rock
{
    // .#.
    // ###
    // .#.
    private:
        std::vector<Rectangle> genShape(long currentHighestRow)
        {
            long xStart(
                this -> distanceFromLeft
            ), xEnd(this -> distanceFromLeft + 2), yStart(
                currentHighestRow + this -> distanceFromBottom
            ), yEnd(yStart + 2);
            return {
                Rectangle(std::pair(xStart, yStart + 1), std::pair(xEnd, yStart + 1)),
                Rectangle(std::pair(xStart + 1, yEnd), std::pair(xStart + 1, yStart))
            };
        }
};

class RockL : public Rock
{
    // ..#
    // ..#
    // ###
    private:
        std::vector<Rectangle> genShape(long currentHighestRow)
        {
            long xStart(
                this -> distanceFromLeft
            ), xEnd(this -> distanceFromLeft + 2), yStart(
                currentHighestRow + this -> distanceFromBottom
            ), yEnd(yStart + 2);
            return {
                Rectangle(std::pair(xStart, yStart), std::pair(xEnd, yStart)),
                Rectangle(std::pair(xEnd, yEnd), std::pair(xEnd, yStart))
            };
        }
};

class RockPipe : public Rock
{
    // #
    // #
    // #
    // #
    private:
        std::vector<Rectangle> genShape(long currentHighestRow)
        {
            long xStart(
                this -> distanceFromLeft
            ), yStart(
                currentHighestRow + this -> distanceFromBottom
            ), yEnd(yStart + 3);
            return {
                Rectangle(std::pair(xStart, yEnd), std::pair(xStart, yStart)),
            };
        }
};

class RockSquare : public Rock
{
    // ##
    // ##
    private:
        std::vector<Rectangle> genShape(long currentHighestRow)
        {
            long xStart(
                this -> distanceFromLeft
            ), xEnd(xStart + 1), yStart(
                currentHighestRow + this -> distanceFromBottom
            ), yEnd(yStart + 1);
            return {
                Rectangle(std::pair(xStart, yEnd), std::pair(xEnd, yStart)),
            };
        }
};


template <>
void StringVectorExercise::runPart1()
{
    std::vector<std::shared_ptr<Rock>> rockOrder = {
        std::make_shared<RockDash>(), 
        std::make_shared<RockPlus>(), 
        std::make_shared<RockL>(), 
        std::make_shared<RockPipe>(), 
        std::make_shared<RockSquare>()
    };
    int numRocks(rockOrder.size());
    long currentHighestRow(0);
    std::vector<Rectangle> allObjectsAtRest;
    std::string windDirs = (this -> data)[0];
    std::queue<char> directions = std::queue<char>();
    for (auto c: windDirs) directions.push(c);

    for (int i=0; i<2022; i++) {
        currentHighestRow = rockOrder[i % numRocks] -> drop(
            currentHighestRow, 
            allObjectsAtRest,
            directions
        ).first;
    }

    std::cout << "Part 1: Highest rock position " << currentHighestRow << std::endl;
    
}

template <>
void StringVectorExercise::runPart2()
{
    std::vector<std::shared_ptr<Rock>> rockOrder = {
        std::make_shared<RockDash>(), 
        std::make_shared<RockPlus>(), 
        std::make_shared<RockL>(), 
        std::make_shared<RockPipe>(), 
        std::make_shared<RockSquare>()
    };
    std::string windDirs = (this -> data)[0];
    std::queue<char> directions = std::queue<char>();
    for (auto c: windDirs) directions.push(c);
    int numRocks(rockOrder.size()), instructionIndex(0), numInstructions(directions.size());

    long currentHighestRow(0);
    std::vector<Rectangle> allObjectsAtRest;

    std::map<int, std::set<int>> seenInstructionsPerRock = std::map<int, std::set<int>>();
    for(int i=0; i<numRocks; i++) seenInstructionsPerRock.insert(std::make_pair(i, std::set<int>()));
    std::map<int, std::set<std::tuple<int, long, long>>> seenInstructionsAndXStartAndRelativeTopPerRock = std::map<int, std::set<std::tuple<int, long, long>>>();
    for(int i=0; i<numRocks; i++) seenInstructionsAndXStartAndRelativeTopPerRock.insert(std::make_pair(i, std::set<std::tuple<int, long, long>>()));
    std::map<std::tuple<int, int, long>, int> rockInstructionXStartToTime = std::map<std::tuple<int, int, long>, int>();

    int cycleDetectionRock;
    int cycleStart;
    int cycleEnd;
    int rockIndex;
    int newHighestRow(currentHighestRow);
    int stopAfter = 30;
    std::map<int, long> stepToHeight = std::map<int, long>();
    stepToHeight.insert(std::make_pair(0, 0));
    for (int i=0; i<100000; i++) {
        rockIndex = i % numRocks;
        std::pair<long, int> newStep = rockOrder[rockIndex] -> drop(
            currentHighestRow, 
            allObjectsAtRest,
            directions
        );
        long newXStart(LONG_MAX);
        long newYStart(0);
        for (auto r: rockOrder[rockIndex] -> stationaryObjects) {
            newXStart = std::min(newXStart, r.getCoveredX()[0]);
            newYStart = std::max(newYStart, r.getHighestY());
        }
        newHighestRow = newStep.first;
        if (
            seenInstructionsAndXStartAndRelativeTopPerRock.at(rockIndex).contains(std::make_tuple(instructionIndex, newXStart, newYStart - newHighestRow))
            && seenInstructionsPerRock.at((rockIndex + 1) % numRocks).contains((instructionIndex + newStep.second) % numInstructions)
        ) {
            cycleStart = rockInstructionXStartToTime.at(std::make_tuple(rockIndex, instructionIndex, newXStart));
            cycleEnd = i;
            std::cout << "Cycle detected between " << cycleStart << " " << cycleEnd << std::endl;
            break;
        }

        seenInstructionsPerRock.at(rockIndex).insert(instructionIndex);
        seenInstructionsAndXStartAndRelativeTopPerRock.at(rockIndex).insert(std::make_tuple(instructionIndex, newXStart, newYStart - newHighestRow));
        currentHighestRow = newHighestRow;
        rockInstructionXStartToTime.insert(std::make_pair(std::make_tuple(rockIndex, instructionIndex, newXStart), i + 1));
        instructionIndex += newStep.second;
        instructionIndex %= numInstructions;
        stepToHeight.insert(std::make_tuple(i + 1, currentHighestRow));
    }

    long targetRocks = 1000000000000L;
    long cycleHeight = stepToHeight.at(cycleEnd) - stepToHeight.at(cycleStart - 1);
    long cycleLength = cycleEnd - (cycleStart - 1);
    long heightUpToCycle = stepToHeight.at(cycleStart - 1);
    long numCycles = (targetRocks - (cycleStart - 1)) / cycleLength;
    long heightUpToEndCycles = heightUpToCycle + numCycles * cycleHeight;
    long remainingSteps = targetRocks - ((cycleStart - 1) + numCycles * cycleLength);
    long totalHeight = heightUpToEndCycles + stepToHeight.at(cycleStart - 1 + remainingSteps) - stepToHeight.at(cycleStart - 1);

    std::cout << "Part 2: Highest rock position " << totalHeight << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(17);
    std::cout << "Day 17 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
