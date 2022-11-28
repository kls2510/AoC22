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
        v.push_back(std::stoi(line));
    }

    return v;
}
