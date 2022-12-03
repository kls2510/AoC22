#ifndef EXERCISE_H_
#define EXERCISE_H_
    #include <iostream>
    #include <vector>
    #include <fstream>

    namespace exercise {

        template <typename T>
        class Exercise{
            private:
                int day;
                T data;
                T formatExerciseData(std::fstream& contents);

            public:
                Exercise(int day)
                {
                    this->day = day;
                    std::fstream fileData("resources/day" + std::to_string(day) + ".txt");
                    this->data = this->formatExerciseData(fileData);
                    fileData.close();
                }
                void runPart1()
                {
                    std::cout << "Part 1 still to implement" << std::endl;
                }
                void runPart2()
                {
                    std::cout << "Part 2 still to implement" << std::endl;
                }
        };

        typedef Exercise<std::vector<int>> IntVectorExercise;
        typedef Exercise<std::vector<std::tuple<char, char>>> CharTupleExercise;
        typedef Exercise<std::vector<std::string>> StringVectorExercise;

    }

#endif  // EXERCISE_H_