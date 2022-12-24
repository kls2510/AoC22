#include <iostream>
#include <unordered_set>
#include <queue>
#include <list>
#include <unordered_map>
#include <exercise.hpp>

using namespace exercise;


namespace std {
    template <class T1, class T2> struct hash<std::pair<T1, T2>> {
        size_t operator()(const std::pair<T1, T2>& p) const
        {
            auto hash1(std::hash<T1>{}(p.first));
            auto hash2(std::hash<T2>{}(p.second));
            if (hash1 != hash2) return hash1 ^ hash2;
            return hash1;
        }
    };
};

std::unordered_map<std::pair<int, int>, std::vector<char>> parseWinds(std::vector<std::vector<char>> lines)
{
    std::unordered_map<std::pair<int, int>, std::vector<char>> winds = std::unordered_map<std::pair<int, int>, std::vector<char>>();
    int row(0), col;
    std::pair<int, int> current;
    for (auto l: lines)
    {
        col = 0;
        for (auto c: l)
        {
            current = std::make_pair(row, col);
            if (c == '>' || c == 'v' || c == '<' || c =='^') {
                if (!winds.contains(current)) winds.insert(std::make_pair(current, std::vector<char>()));
                winds.at(current).push_back(c);
            }
            col++;
        }
        row++;
    }
    return winds;
}

std::unordered_map<std::pair<int, int>, std::vector<char>> nextWindState(
    const std::unordered_map<std::pair<int, int>, std::vector<char>>& prev,
    int rowLimit,
    int colLimit
)
{
    std::unordered_map<std::pair<int, int>, std::vector<char>> winds = std::unordered_map<std::pair<int, int>, std::vector<char>>();
    std::pair<int, int> next;
    for (auto& [prevCoord, prevDirs]: prev)
    {
        for (auto& d: prevDirs)
        {
            if (d == '>') next = std::make_pair(
                prevCoord.first, prevCoord.second + 1 == colLimit ? 1: prevCoord.second + 1
            );
            else if (d == 'v') next = std::make_pair(
                prevCoord.first + 1 == rowLimit ? 1: prevCoord.first + 1, prevCoord.second
            );
            else if (d == '<') next = std::make_pair(
                prevCoord.first, prevCoord.second - 1 == 0 ? colLimit - 1: prevCoord.second - 1
            );
            else next = std::make_pair(
                prevCoord.first - 1 == 0 ? rowLimit - 1: prevCoord.first - 1, prevCoord.second
            );
            if (!winds.contains(next)) winds.insert(std::make_pair(next, std::vector<char>()));
            winds.at(next).push_back(d);
        }
    }
    return winds;
}

int prio(
    std::pair<int, int> target,
    std::pair<int, int> current
)
{
    return std::abs(target.first - current.first) + std::abs(target.second - current.second);
}

int bfs(
    const std::unordered_map<int, std::unordered_map<std::pair<int, int>, std::vector<char>>>& timestepToWindState,
    std::pair<int, int> startPosition,
    std::pair<int, int> targetPosition,
    int startTime,
    int rowLimit,
    int colLimit
)
{
    struct {
        bool operator() (const std::tuple<int, int, std::pair<int, int>> l, const std::tuple<int, int, std::pair<int, int>> r) const { 
            int lDistance = std::get<0>(l) - std::get<1>(l);
            int rDistance = std::get<0>(r) - std::get<1>(r);
            return std::get<0>(l) > std::get<0>(r) || (std::get<0>(l) == std::get<0>(r) && lDistance < rDistance);
        }
    } customMinQOrder;
    std::list<std::tuple<int, int, std::pair<int, int>>> qdata = {};
    std::priority_queue q(qdata.begin(), qdata.end(), customMinQOrder);
    std::unordered_set<std::pair<int, std::pair<int, int>>> visited = std::unordered_set<std::pair<int, std::pair<int, int>>>();

    q.push(std::make_tuple(prio(targetPosition, startPosition), startTime, startPosition));
    visited.insert(std::make_pair(startTime, startPosition));

    int shortestTime(INT_MAX);
    std::vector<std::pair<int, int>> offsets = {
        std::make_pair(0, 0),
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0),
    };
    std::unordered_map<std::pair<int, int>, std::vector<char>> windState;
    while (!q.empty())
    {
        std::tuple<int, int, std::pair<int, int>> next = q.top();
        q.pop();
        int timestamp = std::get<1>(next);
        std::pair<int, int> nextCoord = std::get<2>(next);
        // std::cout << "(" << nextCoord.first << ", " << nextCoord.second << ") @ " << timestamp << std::endl;
        if (nextCoord.first == targetPosition.first && nextCoord.second == targetPosition.second)
        {
            shortestTime = timestamp;
            break;
        }
        int tsToCheck = timestamp + 1;
        windState = timestepToWindState.at(tsToCheck);
        for (auto& o: offsets)
        {
            std::pair<int, int> coordToCheck = std::make_pair(nextCoord.first + o.first, nextCoord.second + o.second);
            if (
                (
                    (coordToCheck.first == startPosition.first && coordToCheck.second == startPosition.second)
                    || (coordToCheck.first == targetPosition.first && coordToCheck.second == targetPosition.second)
                    || (coordToCheck.first > 0 && coordToCheck.first < rowLimit)
                )
                && coordToCheck.second > 0 
                && coordToCheck.second < colLimit 
                && !visited.contains(std::make_pair(tsToCheck, coordToCheck))
                && !windState.contains(coordToCheck)
            )
            {
                q.push(std::make_tuple(tsToCheck + prio(targetPosition, coordToCheck), tsToCheck, coordToCheck));
                visited.insert(std::make_tuple(tsToCheck, coordToCheck));
            }
        }
    }
    return shortestTime;
}

template <>
void CharGridExercise::runPart1()
{
    std::unordered_map<int, std::unordered_map<std::pair<int, int>, std::vector<char>>> timestepToWindState = std::unordered_map<int, std::unordered_map<std::pair<int, int>, std::vector<char>>>();
    int rowLimit(this -> data.size() - 1), colLimit(this -> data[0].size() - 1);
    std::pair<int, int> startPosition(
        std::make_pair(0, 1)
    ), targetPosition(
        std::make_pair(rowLimit, colLimit - 1)
    );

    timestepToWindState.insert(std::make_pair(
        0, parseWinds(this -> data)
    ));
    for (auto i=1; i < 1000; i++)
    {
        timestepToWindState.insert(
            std::make_pair(
                i, 
                nextWindState(timestepToWindState.at(i - 1), rowLimit, colLimit)
            )
        );
    }

    std::cout << "Part 1: Shortest time " << bfs(
        timestepToWindState, startPosition, targetPosition, 0, rowLimit, colLimit
    ) << std::endl;
}

template <>
void CharGridExercise::runPart2()
{
    std::unordered_map<int, std::unordered_map<std::pair<int, int>, std::vector<char>>> timestepToWindState = std::unordered_map<int, std::unordered_map<std::pair<int, int>, std::vector<char>>>();
    int rowLimit(this -> data.size() - 1), colLimit(this -> data[0].size() - 1);
    std::pair<int, int> startPosition(
        std::make_pair(0, 1)
    ), targetPosition(
        std::make_pair(rowLimit, colLimit - 1)
    );

    timestepToWindState.insert(std::make_pair(
        0, parseWinds(this -> data)
    ));
    for (auto i=1; i < 1000; i++)
    {
        timestepToWindState.insert(
            std::make_pair(
                i, 
                nextWindState(timestepToWindState.at(i - 1), rowLimit, colLimit)
            )
        );
    }

    int back = bfs(
        timestepToWindState, targetPosition, startPosition, 295, rowLimit, colLimit
    );
    int backAgain = bfs(
        timestepToWindState, startPosition, targetPosition, back, rowLimit, colLimit
    );

    std::cout << "Part 2: Shortest time " << backAgain << std::endl;
}


int main() {
    CharGridExercise exerciseRunner(24);
    std::cout << "Day 24 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}