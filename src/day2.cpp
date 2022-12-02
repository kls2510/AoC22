#include <iostream>
#include <map>
#include <set>
#include <exercise.hpp>

using namespace exercise;

const std::map<std::string, int> moveScores = {
    {"rock", 1}, {"paper", 2}, {"scissors", 3}
};
const int winScore = 6;
const int drawScore = 3;
const int loseScore = 0;
const std::set<std::tuple<std::string, std::string>> winningPairs = {
    std::make_tuple("paper", "rock"), 
    std::make_tuple("scissors", "paper"), 
    std::make_tuple("rock", "scissors")
};
const std::map<char, std::string> theirMoveMapping = {
    {'A', "rock"}, {'B', "paper"}, {'C', "scissors"}
};

template <>
void CharTupleExercise::runPart1()
{
    std::map<char, std::string> myMoveMapping = {
        {'X', "rock"}, {'Y', "paper"}, {'Z', "scissors"}
    };

    int currentTotal(0);
    for (auto n: this->data) {
        std::string theirMove = theirMoveMapping.at(std::get<0>(n));
        std::string myMove = myMoveMapping.at(std::get<1>(n));
        int resultScore;
        const bool theyWin = winningPairs.find(
            std::make_tuple(theirMove, myMove)
        ) != winningPairs.end();
        const bool meWin = winningPairs.find(
            std::make_tuple(myMove, theirMove)
        ) != winningPairs.end();
        int roundScore = theyWin ? loseScore : meWin ? winScore : drawScore;
        currentTotal += moveScores.at(myMove) + roundScore;
    }
    std::cout << "Part 1: total score " << std::to_string(currentTotal) << std::endl;
}

template <>
void CharTupleExercise::runPart2()
{
    std::map<std::string, std::string> myMoveWinMapping = std::map<std::string, std::string>();
    for (auto t : winningPairs) {
        myMoveWinMapping.insert(std::make_pair(std::get<1>(t),std::get<0>(t)));
    }
    std::map<std::string, std::string> myMoveLoseMapping = std::map<std::string, std::string>();
    for (auto t : winningPairs) {
        myMoveLoseMapping.insert(std::make_pair(std::get<0>(t),std::get<1>(t)));
    }

    int currentTotal(0);
    for (auto n: this->data) {
        std::string theirMove = theirMoveMapping.at(std::get<0>(n));
        char myInstruction = std::get<1>(n);
        std::string myMove;
        if (myInstruction == 'X') myMove = myMoveLoseMapping.at(theirMove);
        else if (myInstruction == 'Y') myMove = theirMove;
        else myMove = myMoveWinMapping.at(theirMove);

        int resultScore;
        const bool theyWin = myInstruction == 'X';
        const bool meWin = myInstruction == 'Z';
        int roundScore = theyWin ? loseScore : meWin ? winScore : drawScore;
        currentTotal += moveScores.at(myMove) + roundScore;
    }
    std::cout << "Part 2: total score " << std::to_string(currentTotal) << std::endl;
}

int main() {
    CharTupleExercise exerciseRunner(2);
    std::cout << "Day 2 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
