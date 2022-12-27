#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <exercise.hpp>

using namespace exercise;


int getDecimal(char c)
{
    if (c == '=') return -2;
    else if (c == '-') return -1;
    else if (c == '0') return 0;
    else if (c == '1') return 1;
    else return 2;
}

std::pair<char, char> getSumAndCarryFromDecimal(int d)
{
    if (d == -6) return std::make_pair('-', '-');
    else if (d == -5) return std::make_pair('0', '-');
    else if (d == -4) return std::make_pair('1', '-');
    else if (d == -3) return std::make_pair('2', '-');
    else if (d == -2) return std::make_pair('=', '0');
    else if (d == -1) return std::make_pair('-', '0');
    else if (d == 0) return std::make_pair('0', '0');
    else if (d == 1) return std::make_pair('1', '0');
    else if (d == 2) return std::make_pair('2', '0');
    else if (d == 3) return std::make_pair('=', '1');
    else if (d == 4) return std::make_pair('-', '1');
    else if (d == 5) return std::make_pair('0', '1');
    else return std::make_pair('1', '1');
}

std::string add(std::string snafu1, std::string snafu2)
{
    // std::cout << "ADDING " << snafu1 << " AND " << snafu2 << std::endl; 
    std::string sum("");
    int snafu1idx, snafu2idx;
    char snafu1digit, snafu2digit, carry('0');
    int v1, v2, v3, decimalSum;
    std::pair<char, char> sumAndCarry;
    int i = 0;
    while (i < snafu1.length() && i < snafu2.length())
    {
        snafu1idx = snafu1.length() - 1 - i;
        snafu1digit = snafu1[snafu1idx];
        v1 = getDecimal(snafu1digit);
        snafu2idx = snafu2.length() - 1 - i;
        snafu2digit = snafu2[snafu2idx];
        v2 = getDecimal(snafu2digit);
        v3 = getDecimal(carry);
        decimalSum = v1 + v2 + v3;

        sumAndCarry = getSumAndCarryFromDecimal(decimalSum);
        // std::cout << snafu1digit << " -> " << v1 << " + "  << snafu2digit << " -> " << v2 << " + " << carry << " -> " << v3 << " = " << sumAndCarry.first << " carry " << sumAndCarry.second << std::endl;
        sum += sumAndCarry.first;
        carry = sumAndCarry.second;
        i++;
    }
    std::string remaining("");
    if (snafu1.length() > i) remaining = snafu1.substr(0, snafu1.length() - i);
    else if (snafu2.length() > i) remaining = snafu2.substr(0, snafu2.length() - i);

    std::reverse(sum.begin(), sum.end());
    if (remaining.length() == 0) return (carry != '0' ? carry + sum: sum);
    if (carry == '0') return remaining + sum;
    return add(remaining, std::string(1, carry)) + sum;
}

template <>
void StringVectorExercise::runPart1()
{
    std::vector<std::string> numbers = this -> data;
    std::string sum("");
    for (auto n: numbers) sum = add(sum, n);
    std::cout << "Part 1: Sum = " << sum << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(25);
    std::cout << "Day 25 exercise:" << std::endl;
    exerciseRunner.runPart1();
    return 0;
}
