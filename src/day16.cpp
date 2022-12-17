#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <queue>
#include <exercise.hpp>

using namespace exercise;

class Valve
{
    private:
        std::string name;
        int flowRate;
        std::vector<std::pair<int, std::shared_ptr<Valve>>> neighbours = std::vector<std::pair<int, std::shared_ptr<Valve>>>();

    public:
        Valve(std::string name, int rate): name(name), flowRate(rate) {}
        void setChildren(
            std::map<std::string, std::shared_ptr<Valve>>& allValves,
            std::vector<std::string> childList
        )
        {
            for (auto cname: childList) this -> neighbours.push_back(std::make_pair(1, allValves.at(cname)));
        }
        void setRelevantChildren(
            std::map<std::string, std::shared_ptr<Valve>>& allValves,
            std::map<std::string, int> distances
        )
        {
            this -> neighbours.clear();
            for (auto p: distances)
            {
                this -> neighbours.push_back(
                    std::make_pair(p.second, allValves.at(p.first))
                );
            }
        }
        int getFlowRate() const { return this -> flowRate; }
        std::string getName() const { return this -> name; }
        std::vector<std::pair<int, std::shared_ptr<Valve>>> getChildren() const
        {
            return this -> neighbours;
        }
};

std::map<std::string, int> findShortestPaths(std::shared_ptr<Valve> from, std::set<std::string> availableValves)
{
    availableValves.erase(from -> getName());
    std::queue<std::pair<int, std::shared_ptr<Valve>>> q = std::queue<std::pair<int, std::shared_ptr<Valve>>>();
    q.push(std::make_pair(0, from));
    std::map<std::string, int> shortestPathLengths = std::map<std::string, int>();
    std::pair<int, std::shared_ptr<Valve>> current;
    std::string currentName;
    std::set<std::string> visited = std::set<std::string>();
    visited.insert(from->getName());
    while (!q.empty() && !availableValves.empty()) {
        current = q.front();
        q.pop();
        currentName = current.second -> getName();
        if (availableValves.contains(currentName))
        {
            availableValves.erase(currentName);
            shortestPathLengths.insert(std::make_pair(currentName, current.first));
        }
        for (auto c: (current.second -> getChildren()))
        {
            currentName = c.second -> getName();
            if (!visited.contains(currentName))
            {
                visited.insert(currentName);
                q.push(std::make_pair(current.first + 1, c.second));
            }
        }
    }
    return shortestPathLengths;
}

std::map<std::string, std::shared_ptr<Valve>> parseInputLines(std::vector<std::string> inp)
{
    std::istringstream ss;
    std::string word, currentName;
    int wordIndex;
    std::map<std::string, std::shared_ptr<Valve>> valves;
    std::map<std::string, std::vector<std::string>> children;

    for (auto l: inp)
    {
        ss = std::istringstream(l);
        wordIndex = 0;
        while (ss >> word) {
            if (wordIndex == 1) currentName = word;
            else if (wordIndex == 4) valves.insert(std::make_pair(
                currentName, std::make_shared<Valve>(currentName, std::stoi(word.substr(word.find_first_of('=') + 1, word.length() - 1)))
            ));
            else if (wordIndex >= 9) {
                if (!children.contains(currentName)) children.insert(
                    std::make_pair(currentName, std::vector<std::string>())
                );
                children.at(currentName).push_back(
                    word.substr(0, word.length() + (word.find(',') == std::string::npos ? 0: -1))
                );
            }
            wordIndex++;
        }
    }
    // set initial children
    for (auto p: children) valves.at(p.first) -> setChildren(valves, p.second);
    // shorten routes between valves where pressure is >0 or start node
    std::shared_ptr<Valve> valveRoot = valves.at("AA");
    std::set<std::string> relevantNodes = std::set<std::string>();
    for (auto v: valves)
    {
        if (v.second -> getFlowRate() > 0) relevantNodes.insert(v.first);
    }
    std::map<std::string, std::map<std::string, int>> valveToAdjacentNodes;
    valveToAdjacentNodes.insert(std::make_pair(valveRoot->getName(), findShortestPaths(valveRoot, relevantNodes)));
    for (auto n: relevantNodes) valveToAdjacentNodes.insert(std::make_pair(n, findShortestPaths(valves.at(n), relevantNodes)));
    for (auto p: valveToAdjacentNodes) valves.at(p.first) -> setRelevantChildren(valves, p.second);
    return valves;
}

int maxPressure(
    std::shared_ptr<Valve>& current, 
    std::set<std::string>& remaining,
    int remainingTime
)
{
    if (remainingTime <= 1 || remaining.size() == 0) return 0;
    remaining.erase(current -> getName());
    int newRemainingTime, releaseFromThisValve;
    if (current -> getFlowRate() == 0)
    {
        newRemainingTime = remainingTime;
        releaseFromThisValve = 0;
    }
    else
    {
        newRemainingTime = remainingTime - 1;
        releaseFromThisValve = newRemainingTime * current -> getFlowRate();
    }
    int maxPressureReleaseFromTurningOn(releaseFromThisValve);
    for (auto c: current -> getChildren())
    {
        if (remaining.contains(c.second -> getName()) && newRemainingTime - c.first > 0)
        {
            maxPressureReleaseFromTurningOn = std::max(
                maxPressureReleaseFromTurningOn,
                releaseFromThisValve + maxPressure(
                    c.second, remaining, newRemainingTime - c.first
                )
            );
        }
    }
    remaining.insert(current -> getName());

    return maxPressureReleaseFromTurningOn;
}

template <>
void StringVectorExercise::runPart1()
{
    std::map<std::string, std::shared_ptr<Valve>> valves = parseInputLines(this -> data);
    std::shared_ptr<Valve> valveRoot = valves.at("AA");
    std::set<std::string> remaining = std::set<std::string>(); 
    for (auto v: valves)
    {
        if (v.second -> getFlowRate() > 0) remaining.insert(v.first);
    }   
    int result = maxPressure(valveRoot, remaining, 30);
    std::cout << "Part 1: Max pressure released " << result << std::endl;
}

void combsHelper(int offset, int k, int n, std::set<int> current, std::vector<std::set<int>>& indexes) {
  if (k == 0) {
    indexes.push_back(current);
    return;
  }
  for (int i = offset; i <= n - k; ++i) {
    current.insert(i);
    combsHelper(i+1, k-1, n, current, indexes);
    current.erase(i);
  }
}

std::vector<std::set<int>> combs(int k, int n)
{
    std::vector<std::set<int>> result = std::vector<std::set<int>>();
    std::set<int> c = std::set<int>();
    combsHelper(0, k, n, c, result);
    return result;
}

template <>
void StringVectorExercise::runPart2()
{
    std::map<std::string, std::shared_ptr<Valve>> valves = parseInputLines(this -> data);
    std::shared_ptr<Valve> valveRoot = valves.at("AA");
    std::vector<std::string> allNonZeroValves = std::vector<std::string>();
    for (auto v: valves)
    {
        if (v.second -> getFlowRate() > 0) allNonZeroValves.push_back(v.first);
    } 
    int currentMax(0);
    std::set<std::string> toVisit1 = std::set<std::string>(), toVisit2 = std::set<std::string>();
    for (auto split = 0; split <= allNonZeroValves.size() / 2; split++)
    {
        std::cout << "Split by " << split << std::endl;
        std::vector<std::set<int>> allCombs = combs(split, allNonZeroValves.size());
        std::cout << "Num to check " << allCombs.size() / 2 << std::endl;
        for (auto n=0; n < allCombs.size() / 2; n++)
        {
            std::set<int> c = allCombs[n];
            toVisit1.clear();
            toVisit2.clear();
            for (auto j=0; j<allNonZeroValves.size(); j++) {
                if (c.contains(j)) toVisit1.insert(allNonZeroValves[j]);
                else toVisit2.insert(allNonZeroValves[j]);
            }
            currentMax = std::max(
                currentMax,
                maxPressure(valveRoot, toVisit1, 26)
                + maxPressure(valveRoot, toVisit2, 26)
            );
        }
    }
    std::cout << "Part 2: Max pressure released " << currentMax << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(16);
    std::cout << "Day 16 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
