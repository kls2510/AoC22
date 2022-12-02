#include <fstream>
#include <iostream>
#include <vector>
#include <exercise.hpp>

using namespace exercise;


template <>
std::vector<int> IntVectorExercise::formatExerciseData(std::fstream& contents)
{
    std::vector<int> v = std::vector<int>();
    std::string line;

    while (getline (contents, line)) {
        line != "" ? v.push_back(std::stoi(line)) : v.push_back(-1);
    }

    return v;
}

template <>
std::vector<std::tuple<char, char>> CharTupleExercise::formatExerciseData(std::fstream& contents)
{
    std::vector<std::tuple<char, char>> v = std::vector<std::tuple<char, char>>();
    std::string line;

    while (getline (contents, line)) {
        std::string s1 = line.substr(0, 1);
        std::string s2 = line.substr(2, 3);
        v.push_back(std::tuple(s1[0], s2[0]));
    }

    return v;
}
