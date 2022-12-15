#include <iostream>
#include <sstream>
#include <set>
#include <list>
#include <exercise.hpp>

using namespace exercise;

class Sensor
{
    private:
        // x across, y down
        std::pair<int, int> loc;
        std::pair<int, int> closestBeaconLoc;
        int manhattanDistance;

    public:
        Sensor(std::string description)
        {
            std::istringstream ss = std::istringstream(description);
            std::string word;
            int wordIndex(0), currentCoord;
            while (ss >> word) {
                if (wordIndex == 2 || wordIndex == 8) currentCoord = std::stoi(word.substr(2, word.length() - 1));
                else if (wordIndex == 3) this -> loc = std::make_pair(currentCoord, std::stoi(word.substr(2, word.length() - 1)));
                else if (wordIndex == 9) this -> closestBeaconLoc = std::make_pair(currentCoord, std::stoi(word.substr(2, word.length() - 1)));
                wordIndex++;
            }
            this -> manhattanDistance = std::abs(this -> loc.first - this -> closestBeaconLoc.first) + std::abs(this -> loc.second - this -> closestBeaconLoc.second);
        }
        bool isMinDistanceToRowShorterThanBeacon(int row)
        {
            if (this -> closestBeaconLoc == std::make_pair(this -> loc.first, row)) return false;
            int manhattanDistToRow(std::abs(this -> loc.second - row));
            return manhattanDistToRow <= this -> manhattanDistance;
        }
        std::optional<std::pair<int, int>> getColRangeWhereMinDistanceToRowShorterThanBeacon(int row)
        {
            if (!this -> isMinDistanceToRowShorterThanBeacon(row))
            {
                return {};
            }
            int manhattanDistToRow(std::abs(this -> loc.second - row)), manhattanDistToBeacon(this -> manhattanDistance);
            int remainder = manhattanDistToBeacon - manhattanDistToRow;
            int lowerRemainder = this->closestBeaconLoc.first == loc.first - remainder ? remainder - 1: remainder;
            int upperRemainder = this->closestBeaconLoc.first == loc.first + remainder ? remainder - 1: remainder;
            std::pair<int, int> result = std::make_pair(
                this -> loc.first - lowerRemainder, this -> loc.first + upperRemainder
            );
            return result;
        }
        std::pair<int, int> getLoc() const { return this -> loc; }
        std::pair<int, int> getBeaconLoc() const { return this -> closestBeaconLoc; }
};

std::vector<Sensor> parseInputLines(std::vector<std::string> inp)
{
    std::vector<Sensor> sensors = std::vector<Sensor>();
    for (auto l: inp) sensors.push_back(Sensor(l));
    return sensors;
}

void getDistinctRangesFromAllRanges(
    std::vector<std::pair<int, int>>& allRanges,
    std::vector<std::pair<int, int>>& distinctRanges
)
{
    distinctRanges.clear();
    // sort ranges by start
    std::sort(allRanges.begin(), allRanges.end());
    // merge ranges
    for (auto range: allRanges)
    {
        if (distinctRanges.empty()) distinctRanges.push_back(range);
        else
        {
            int numRanges = distinctRanges.size();
            std::pair<int, int> previous = distinctRanges[numRanges - 1];
            if (range.first - 1 > previous.second) distinctRanges.push_back(range);
            else distinctRanges[numRanges - 1] = std::make_pair(
                distinctRanges[numRanges - 1].first,
                std::max(distinctRanges[numRanges - 1].second, range.second)
            );
        }
    }
}

template <>
void StringVectorExercise::runPart1()
{
    std::vector<Sensor> sensors = parseInputLines(this -> data);
    int row(2000000);
    std::vector<std::pair<int, int>> allRangesWithoutBeacon = std::vector<std::pair<int, int>>();
    std::vector<std::pair<int, int>> distinctRangesWithoutBeacon = std::vector<std::pair<int, int>>();
    for (auto sensor: sensors)
    {
        std::optional<std::pair<int, int>> maybeColRange = sensor.getColRangeWhereMinDistanceToRowShorterThanBeacon(row);
        if (maybeColRange.has_value())
        {
            std::pair<int, int> colRange = maybeColRange.value();
            allRangesWithoutBeacon.push_back(colRange);
        }
    }
    getDistinctRangesFromAllRanges(
        allRangesWithoutBeacon, distinctRangesWithoutBeacon
    );
    // sum ranges
    int numColsWithoutBeacon(0);
    for (auto p: distinctRangesWithoutBeacon) numColsWithoutBeacon += p.second - p.first + 1;
    std::cout << "Part 1: Number positions cannot have beacon " << numColsWithoutBeacon << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::vector<Sensor> sensors = parseInputLines(this -> data);
    std::pair<int, int> beaconPos;
    std::set<std::pair<int, int>> alreadyTaken = std::set<std::pair<int, int>>();
    bool found(false);
    int rowLimit(4000000);

    for (auto sensor: sensors)
    {
        alreadyTaken.insert(sensor.getLoc());
        alreadyTaken.insert(sensor.getBeaconLoc());
    }

    std::vector<std::pair<int, int>> allRangesWithoutBeacon = std::vector<std::pair<int, int>>();
    std::vector<std::pair<int, int>> distinctRangesWithoutBeacon = std::vector<std::pair<int, int>>();
    std::pair<int, int> range, toCheck;
    for (auto row=0; row <= rowLimit; row++)
    {
        for (auto sensor: sensors)
        {
            std::optional<std::pair<int, int>> maybeColRange = sensor.getColRangeWhereMinDistanceToRowShorterThanBeacon(row);
            if (maybeColRange.has_value()) allRangesWithoutBeacon.push_back(maybeColRange.value());
        }
        getDistinctRangesFromAllRanges(
            allRangesWithoutBeacon, distinctRangesWithoutBeacon
        );
        int currentCol(0), rangesIndex(0);
        while (currentCol <= rowLimit && rangesIndex < distinctRangesWithoutBeacon.size())
        {
            range = distinctRangesWithoutBeacon[rangesIndex];
            if (range.second < currentCol)
            {
                if (rangesIndex == distinctRangesWithoutBeacon.size() - 1)
                {
                    toCheck = std::make_pair(currentCol, row);
                    if (!alreadyTaken.contains(toCheck))
                    {
                        found = true;
                        beaconPos = toCheck;
                        break;
                    }
                }
                rangesIndex ++;
            }
            else if (range.first > currentCol)
            {
                toCheck = std::make_pair(currentCol, row);
                if (!alreadyTaken.contains(toCheck))
                {
                    found = true;
                    beaconPos = toCheck;
                    break;
                }
                currentCol ++;
            }
            else {
                currentCol = range.second + 1;
                rangesIndex++;
            }
        }
        if (found) break;
        allRangesWithoutBeacon.clear();
    }
    long result = (long)beaconPos.first * rowLimit + (long)beaconPos.second;
    std::cout << "Part 2: Tuning frequency for beacon " << result << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(15);
    std::cout << "Day 15 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
