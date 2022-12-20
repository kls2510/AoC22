#include <iostream>
#include <sstream>
#include <map>
#include <numeric>
#include <exercise.hpp>

using namespace exercise;


class Blueprint
{
    public:
        int number;
        // costs are x ore, y clay, z obsidian
        std::tuple<int, int, int> oreCost;
        std::tuple<int, int, int> clayCost;
        std::tuple<int, int, int> obsidianCost;
        std::tuple<int, int, int> geodeCost;
        Blueprint(std::string line)
        {
            std::istringstream ss(line);
            std::string word;
            int wordIndex(0);
            int x;
            while (ss >> word) {
                if (wordIndex == 1) this -> number = std::stoi(word.substr(0, word.find_first_of(':')));
                if (wordIndex == 6) this -> oreCost = std::make_tuple(std::stoi(word), 0, 0);
                if (wordIndex == 12) this -> clayCost = std::make_tuple(std::stoi(word), 0, 0);
                if (wordIndex == 18) x = std::stoi(word);
                if (wordIndex == 21) this -> obsidianCost = std::make_tuple(x, std::stoi(word), 0);
                if (wordIndex == 27) x = std::stoi(word);
                if (wordIndex == 30) this -> geodeCost = std::make_tuple(x, 0, std::stoi(word));
                wordIndex++;
            }
        }
        bool canAfford(std::tuple<int, int, int> availableResources, std::tuple<int, int, int> costs)
        {
            return (
                std::get<0>(availableResources) >= std::get<0>(costs)
                && std::get<1>(availableResources) >= std::get<1>(costs)
                && std::get<2>(availableResources) >= std::get<2>(costs)
            );
        }
        std::tuple<int, int, int> purchase(std::tuple<int, int, int> availableResources, std::tuple<int, int, int> costs)
        {
            return std::make_tuple(
                std::get<0>(availableResources) - std::get<0>(costs),
                std::get<1>(availableResources) - std::get<1>(costs),
                std::get<2>(availableResources) - std::get<2>(costs)
            );
        }
        bool canAffordOreRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> canAfford(availableResources, this -> oreCost);
        }
        std::tuple<int, int, int> purchaseOreRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> purchase(availableResources, this -> oreCost);
        }
        bool canAffordClayRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> canAfford(availableResources, this -> clayCost);
        }
        std::tuple<int, int, int> purchaseClayRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> purchase(availableResources, this -> clayCost);
        }
        bool canAffordObsidianRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> canAfford(availableResources, this -> obsidianCost);
        }
        std::tuple<int, int, int> purchaseObsidianRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> purchase(availableResources, this -> obsidianCost);
        }
        bool canAffordGeodeRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> canAfford(availableResources, this -> geodeCost);
        }
        std::tuple<int, int, int> purchaseGeodeRobot(std::tuple<int, int, int> availableResources)
        {
            return this -> purchase(availableResources, this -> geodeCost);
        }
        void printCost(std::tuple<int, int, int> cost)
        {
            std::cout << std::get<0>(cost) << " ore " << std::get<1>(cost) << " clay " << std::get<2>(cost) << " obsidian." << std::endl;
        }
        void print() 
        {
            std::cout << "BLUEPRINT " << this -> number << std::endl;
            std::cout << "\tOre cost -> ";
            this -> printCost(this->oreCost);
            std::cout << "\tClay cost -> ";
            this -> printCost(this->clayCost);
            std::cout << "\tObsidian cost -> ";
            this -> printCost(this->obsidianCost);
            std::cout << "\tGeode cost -> ";
            this -> printCost(this->geodeCost);
            std::cout << "\tMax costs -> ";
            this -> printCost(this->getMaxNeeded());
            std::cout << std::endl;
        }
        bool oreIsLeastCost()
        {
            return std::get<0>(this -> oreCost) <= std::get<0>(this -> clayCost);
        }
        std::tuple<int, int, int> getMaxNeeded()
        {
            return std::make_tuple(
                std::max({std::get<0>(this->oreCost), std::get<0>(this->clayCost), std::get<0>(this->obsidianCost), std::get<0>(this->geodeCost)}),
                std::max({std::get<1>(this->oreCost), std::get<1>(this->clayCost), std::get<1>(this->obsidianCost), std::get<1>(this->geodeCost)}),
                std::max({std::get<2>(this->oreCost), std::get<2>(this->clayCost), std::get<2>(this->obsidianCost), std::get<2>(this->geodeCost)})
            );
        }
};

int maxGeode(
    std::tuple<int, int, int> availableResources,
    std::tuple<int, int, int, int> numBuiltRobots,
    int ts,
    std::map<std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int, int>, int>, int>& memo,
    Blueprint& blueprint,
    int& maxTime
)
{
    std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int, int>, int> k =std::make_tuple(availableResources, numBuiltRobots, ts);
    if (ts == maxTime - 1) return std::get<3>(numBuiltRobots);
    if (memo.contains(k)) return memo.at(k);

    int oreRobots = std::get<0>(numBuiltRobots);
    int clayRobots = std::get<1>(numBuiltRobots);
    int obsidianRobots = std::get<2>(numBuiltRobots);
    int geodeRobots = std::get<3>(numBuiltRobots);

    int geodeFrom1(0), geodeFrom2(0), geodeFrom3(0), geodeFrom4(0), geodeFrom5(0);
    std::tuple<int, int, int> newResources = std::make_tuple(
        std::get<0>(availableResources) + oreRobots,
        std::get<1>(availableResources) + clayRobots,
        std::get<2>(availableResources) + obsidianRobots
    );

    std::tuple<int, int, int> limits = blueprint.getMaxNeeded();
    if (blueprint.canAffordGeodeRobot(availableResources))
    {
        geodeFrom4 = maxGeode(
            blueprint.purchaseGeodeRobot(newResources),
            std::make_tuple(oreRobots, clayRobots, obsidianRobots, geodeRobots + 1),
            ts + 1,
            memo,
            blueprint,
            maxTime
        );
    }
    else if (blueprint.canAffordObsidianRobot(availableResources) && obsidianRobots < std::get<2>(limits))
    {
        geodeFrom3 = maxGeode(
            blueprint.purchaseObsidianRobot(newResources),
            std::make_tuple(oreRobots, clayRobots, obsidianRobots + 1, geodeRobots),
            ts + 1,
            memo,
            blueprint,
            maxTime
        );
    }
    else
    {
        if (blueprint.canAffordOreRobot(availableResources) && oreRobots < std::get<0>(limits))
        {
            geodeFrom1 = maxGeode(
                blueprint.purchaseOreRobot(newResources),
                std::make_tuple(oreRobots + 1, clayRobots, obsidianRobots, geodeRobots),
                ts + 1,
                memo,
                blueprint,
                maxTime
            );
        }
        if (blueprint.canAffordClayRobot(availableResources) && clayRobots < std::get<1>(limits))
        {
            geodeFrom2 = maxGeode(
                blueprint.purchaseClayRobot(newResources),
                std::make_tuple(oreRobots, clayRobots + 1, obsidianRobots, geodeRobots),
                ts + 1,
                memo,
                blueprint,
                maxTime
            );
        }
        if (
            (blueprint.oreIsLeastCost() && !blueprint.canAffordClayRobot(availableResources))
            || (!blueprint.oreIsLeastCost() && !blueprint.canAffordOreRobot(availableResources))
            || (clayRobots + std::get<1>(availableResources) >= std::get<1>(blueprint.obsidianCost))
        )
        {
            geodeFrom5 = maxGeode(
                newResources,
                std::make_tuple(oreRobots, clayRobots, obsidianRobots, geodeRobots),
                ts + 1,
                memo,
                blueprint,
                maxTime
            );
        }
    }

    int numGeodes = geodeRobots + std::max(
        {geodeFrom1, geodeFrom2, geodeFrom3, geodeFrom4, geodeFrom5}
    );
    memo.insert(std::make_pair(k, numGeodes));
    return numGeodes;
}

template <>
void StringVectorExercise::runPart1()
{
    std::vector<Blueprint> blueprints = std::vector<Blueprint>();
    for (auto l: this -> data) blueprints.push_back(Blueprint(l));

    std::vector<int> qualityLevels = std::vector<int>();
    int maxTime(24);
    for (auto& bp: blueprints)
    {
        std::map<std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int, int>, int>, int> memo = std::map<std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int, int>, int>, int>();
        int maxGeodes = maxGeode(
            std::make_tuple(0, 0, 0),
            std::make_tuple(1, 0, 0, 0),
            0,
            memo,
            bp,
            maxTime
        );
        qualityLevels.push_back(
            bp.number * maxGeodes
        );
    }

    std::cout << "Part 1: Quality Levels " << std::accumulate(qualityLevels.begin(), qualityLevels.end(), 0) << std::endl;
    
}


template <>
void StringVectorExercise::runPart2()
{
    std::vector<Blueprint> blueprints = std::vector<Blueprint>();
    for (auto l: this -> data) blueprints.push_back(Blueprint(l));

    int result(1);
    int maxTime(32);
    for (auto& bp: std::vector<Blueprint>(blueprints.begin(), blueprints.begin() + 3))
    {
        std::map<std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int, int>, int>, int> memo = std::map<std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int, int>, int>, int>();
        int maxGeodes = maxGeode(
            std::make_tuple(0, 0, 0),
            std::make_tuple(1, 0, 0, 0),
            0,
            memo,
            bp,
            maxTime
        );
        result *= maxGeodes;
    }

    std::cout << "Part 2: Max Geodes " << result << std::endl;
    
}

int main() {
    StringVectorExercise exerciseRunner(19);
    std::cout << "Day 19 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
