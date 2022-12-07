#include <iostream>
#include <vector>
#include <map>
#include <exercise.hpp>

using namespace exercise;

class NodeFS
{
    private:
        bool isFile;
        std::string name;
        int size;
        std::shared_ptr<NodeFS> parent;
        std::map<std::string, std::shared_ptr<NodeFS>> children = std::map<std::string, std::shared_ptr<NodeFS>>();

        void bumpSize(int toAdd)
        {
            this->size += toAdd;
            if (this->parent) this->parent->bumpSize(toAdd);
        }

        void printAtLevel(int level)
        {
            for (auto i=0; i < level; i++) std::cout << '\t';
            std::string info = this -> isFile ? "file, size=": "folder, size=";
            info += std::to_string(this->size);
            std::cout << "- " << this->name << " (" << info << ")";
            std::cout << '\n';
            for (auto child: this -> children) child.second -> printAtLevel(level + 1);
        }

    public:
        NodeFS(
            bool isFile,
            std::string name,
            int size,
            std::shared_ptr<NodeFS> parent
        ) : isFile(isFile), name(name), size(size), parent(parent) {}

        void addChild(std::shared_ptr<NodeFS> child)
        {
            if (!this->children.contains(child->name))
            {
                this->children.insert(std::make_pair(child->name, child));
                this->bumpSize(child->size);
            }
        }

        const int getSize() {return this->size;}
        const std::shared_ptr<NodeFS> getParent() {return this->parent;}
        const std::shared_ptr<NodeFS> getChild(std::string name) 
        {
            return this->children.at(name); 
        }
        void print() { this -> printAtLevel(0); }
        int totalSizeDirsAtMost(int limit)
        {
            int totalSum(0);
            if (! this -> isFile) {
                if (this -> size <= limit) totalSum += this -> size;
                for (auto child: this -> children) totalSum += child.second -> totalSizeDirsAtMost(limit);
            }
            return totalSum;
        }
        int smallestDirSizeAtLeast(int minimum)
        {
            int smallest(INT_MAX);
            if (! this -> isFile) {
                if (this -> size >= minimum) smallest = std::min(smallest, this -> size);
                for (auto child: this -> children) smallest = std::min(smallest, child.second -> smallestDirSizeAtLeast(minimum));
            }
            return smallest;
        }
};

std::shared_ptr<NodeFS> makeFSTree(std::vector<std::string> terminalOutput)
{
    std::shared_ptr<NodeFS> root = std::shared_ptr<NodeFS>(
        new NodeFS(
            false, "/", 0, std::shared_ptr<NodeFS>(nullptr)
        )
    );
    std::shared_ptr<NodeFS> current;

    for (auto l: terminalOutput)
    {
        if (l[0] == '$')
        {
            // command
            if (l[2] == 'c')
            {
                // change directory
                if (l[5] == '.')
                {
                    // go up
                    current = current->getParent();
                }
                else if (l[5] == '/')
                {
                    // go root
                    current = root;
                }
                else
                {
                    // go down
                    current = current->getChild(l.substr(5, l.length()));
                }
            }
        }
        else {
            std::shared_ptr<NodeFS> newNode;
            // children
            if (l[0] == 'd')
            {
                // folder
                std::string name = l.substr(4, l.length());
                newNode = std::shared_ptr<NodeFS>(
                    new NodeFS(
                        false, name, 0, current
                    )
                );
            }
            else 
            {
                //file
                int splitIndex = l.find_first_of(' ');
                int size = std::stoi(l.substr(0, splitIndex));
                std::string name = l.substr(splitIndex + 1, l.length());
                newNode = std::shared_ptr<NodeFS>(
                    new NodeFS(
                        true, name, size, current
                    )
                );
            }
            current->addChild(newNode);
        }
    }

    return root;
}

template <>
void StringVectorExercise::runPart1()
{
    
    std::shared_ptr<NodeFS> root = makeFSTree(this -> data);
    std::cout << "Part 1: Total size " << std::to_string(root -> totalSizeDirsAtMost(100000)) << std::endl;
}

template <>
void StringVectorExercise::runPart2()
{
    std::shared_ptr<NodeFS> root = makeFSTree(this -> data);
    int disk(70000000), needed(30000000), used(root->getSize());
    int toDelete = needed - (disk - used);
    std::cout << "Part 2: Smallest size " << std::to_string(root -> smallestDirSizeAtLeast(toDelete)) << std::endl;
}

int main() {
    StringVectorExercise exerciseRunner(7);
    std::cout << "Day 7 exercise:" << std::endl;
    exerciseRunner.runPart1();
    exerciseRunner.runPart2();
    return 0;
}
