#include <iostream>
#include <set>
#include <queue>
#include <map>
#include <exercise.hpp>

using namespace exercise;


class UnitSquare
{   
    public:
        std::pair<long, long> topLeft;
        int inputIndex;
        UnitSquare(std::pair<long, long> topLeft, int inputIndex): topLeft(topLeft), inputIndex(inputIndex) {}
        bool isNextTo(
            UnitSquare other
        )
        {
            std::vector<std::pair<int, int>> dirs = {
                std::make_pair(-1, 0),
                std::make_pair(1, 0),
                std::make_pair(0, -1),
                std::make_pair(0, 1)
            };
            for (auto dir: dirs)
            {
                std::pair<int, int> nextTopLeft = std::make_pair(
                    this -> topLeft.first + dir.first, this -> topLeft.second + dir.second
                );
                if (
                    nextTopLeft.first == other.topLeft.first
                    && nextTopLeft.second == other.topLeft.second
                ) return true;
            }
            return false;
        }
};

template <>
void StringVectorExercise::runPart1()
{
    std::map<int, std::vector<UnitSquare>> squaresByX = std::map<int, std::vector<UnitSquare>>();
    std::map<int, std::vector<UnitSquare>> squaresByY = std::map<int, std::vector<UnitSquare>>();
    std::map<int, std::vector<UnitSquare>> squaresByZ = std::map<int, std::vector<UnitSquare>>();

    int totalNumSquares(0);
    for (auto l: this -> data)
    {
        int firstIndex(l.find_first_of(',')), secondIndex(l.find_last_of(','));
        int x(std::stoi(l.substr(0, firstIndex))), y(std::stoi(l.substr(firstIndex + 1, secondIndex))), z(std::stoi(l.substr(secondIndex + 1, l.length())));
        if (!squaresByX.contains(x)) squaresByX.insert(std::make_pair(x, std::vector<UnitSquare>()));
        squaresByX.at(x).push_back(UnitSquare(std::make_pair(y, z), totalNumSquares));
        if (!squaresByY.contains(y)) squaresByY.insert(std::make_pair(y, std::vector<UnitSquare>()));
        squaresByY.at(y).push_back(UnitSquare(std::make_pair(x, z), totalNumSquares));
        if (!squaresByZ.contains(z)) squaresByZ.insert(std::make_pair(z, std::vector<UnitSquare>()));
        squaresByZ.at(z).push_back(UnitSquare(std::make_pair(x, y), totalNumSquares));
        totalNumSquares ++;
    }
    int totalNumFaces = 6 * totalNumSquares;
    std::set<std::pair<int, int>> matched = std::set<std::pair<int, int>>();

    for (auto& m: {squaresByX, squaresByY, squaresByZ})
    {
        for (auto& p: m)
        {
            for (int i = 0; i < p.second.size() - 1; i++)
            {
                for (int j = i + 1; j < p.second.size(); j++)
                {
                    UnitSquare s1(p.second[i]), s2(p.second[j]);
                    if (s1.isNextTo(s2)) matched.insert(std::make_pair(s1.inputIndex, s2.inputIndex));
                }
            }
        }
    }

    std::cout << "Part 1: Total surface area " << totalNumFaces - 2 * matched.size() << std::endl;
    
}


template <>
void StringVectorExercise::runPart2()
{
    std::map<int, std::vector<UnitSquare>> squaresByX = std::map<int, std::vector<UnitSquare>>();
    std::map<int, std::vector<UnitSquare>> squaresByY = std::map<int, std::vector<UnitSquare>>();
    std::map<int, std::vector<UnitSquare>> squaresByZ = std::map<int, std::vector<UnitSquare>>();
    std::set<std::tuple<int, int, int>> allCubes = std::set<std::tuple<int, int, int>>();

    int totalNumSquares(0);
    int minCoord(INT_MAX), maxCoord(0);
    for (auto l: this -> data)
    {
        int firstIndex(l.find_first_of(',')), secondIndex(l.find_last_of(','));
        int x(std::stoi(l.substr(0, firstIndex))), y(std::stoi(l.substr(firstIndex + 1, secondIndex))), z(std::stoi(l.substr(secondIndex + 1, l.length())));
        if (!squaresByX.contains(x)) squaresByX.insert(std::make_pair(x, std::vector<UnitSquare>()));
        squaresByX.at(x).push_back(UnitSquare(std::make_pair(y, z), totalNumSquares));
        if (!squaresByY.contains(y)) squaresByY.insert(std::make_pair(y, std::vector<UnitSquare>()));
        squaresByY.at(y).push_back(UnitSquare(std::make_pair(x, z), totalNumSquares));
        if (!squaresByZ.contains(z)) squaresByZ.insert(std::make_pair(z, std::vector<UnitSquare>()));
        squaresByZ.at(z).push_back(UnitSquare(std::make_pair(x, y), totalNumSquares));
        totalNumSquares ++;
        allCubes.insert(std::make_tuple(x, y, z));
        minCoord = std::min({minCoord, x, y, z});
        maxCoord = std::max({maxCoord, x, y, z});
    }
    int totalNumFaces = 6 * totalNumSquares;
    std::set<std::pair<int, int>> matched = std::set<std::pair<int, int>>();

    for (auto& m: {squaresByX, squaresByY, squaresByZ})
    {
        for (auto& p: m)
        {
            for (int i = 0; i < p.second.size() - 1; i++)
            {
                for (int j = i + 1; j < p.second.size(); j++)
                {
                    UnitSquare s1(p.second[i]), s2(p.second[j]);
                    if (s1.isNextTo(s2)) matched.insert(std::make_pair(s1.inputIndex, s2.inputIndex));
                }
            }
        }
    }
    totalNumFaces -= 2 * matched.size();

    std::set<std::tuple<int, int, int>> allCoordsInsideBlob = std::set<std::tuple<int, int, int>>();
    std::set<std::tuple<int, int, int>> allCoordsOutsideBlob = std::set<std::tuple<int, int, int>>();
    for (auto i = minCoord; i <= maxCoord; i++)
    {
        for (auto j = minCoord; j <= maxCoord; j++)
        {
            for (auto k = minCoord; k <= maxCoord; k++)
            {
                std::tuple<int, int, int> c = std::make_tuple(i, j, k);
                if (allCubes.contains(c) || allCoordsInsideBlob.contains(c) || allCoordsOutsideBlob.contains(c)) continue;
                std::queue<std::tuple<int, int, int>> q = std::queue<std::tuple<int, int, int>>();
                std::set<std::tuple<int, int, int>> visited = std::set<std::tuple<int, int, int>>();
                q.push(c);
                visited.insert(c);
                bool canReachOutside = false;
                while (!q.empty())
                {
                    std::tuple<int, int, int> current = q.front();
                    q.pop();
                    int x(std::get<0>(current)), y(std::get<1>(current)), z(std::get<2>(current));
                    if (
                        x <= minCoord
                        || x >= maxCoord
                        || y <= minCoord
                        || y >= maxCoord
                        || z <= minCoord
                        || z >= maxCoord
                    ) {
                        canReachOutside = true;
                        continue;
                    }
                    else {
                        std::vector<std::tuple<int, int, int>> next = {
                            std::make_tuple(x + 1, y, z),
                            std::make_tuple(x - 1, y, z),
                            std::make_tuple(x, y + 1, z),
                            std::make_tuple(x, y - 1, z),
                            std::make_tuple(x, y, z + 1),
                            std::make_tuple(x, y, z - 1),
                        };
                        for (auto& n: next)
                        {
                            if (
                                !allCubes.contains(n) 
                                && !visited.contains(n)
                            )
                            {
                                visited.insert(n);
                                q.push(n);
                            }
                        }
                    }
                }
                if (!canReachOutside) allCoordsInsideBlob.insert(visited.begin(), visited.end());
                else allCoordsOutsideBlob.insert(visited.begin(), visited.end());
            }
        }
    }
    
    for (auto& c: allCoordsInsideBlob) {
        int x(std::get<0>(c)), y(std::get<1>(c)), z(std::get<2>(c));
        std::set<std::tuple<int, int, int>> next = {
            std::make_tuple(x + 1, y, z),
            std::make_tuple(x - 1, y, z),
            std::make_tuple(x, y + 1, z),
            std::make_tuple(x, y - 1, z),
            std::make_tuple(x, y, z + 1),
            std::make_tuple(x, y, z - 1),
        };
        std::vector<std::tuple<int, int, int>> intersection = std::vector<std::tuple<int, int, int>>();
        std::set_intersection(next.begin(), next.end(), allCubes.begin(), allCubes.end(), std::back_inserter(intersection));
        totalNumFaces -= intersection.size();
    }
    std::cout << "Part 2: Total surface area " << totalNumFaces << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(18);
    std::cout << "Day 18 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
