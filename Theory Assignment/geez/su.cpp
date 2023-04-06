#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>

using namespace std;

struct MemoryBlock
{
    bool free;
    int size;
    char process;
    int actualSize;
};

void splitBlock(vector<MemoryBlock> &memory, int blockIndex, int requestedSize)
{
    while (memory[blockIndex].size / 2 >= requestedSize)
    {
        memory[blockIndex].free = false;
        memory.insert(memory.begin() + blockIndex + 1, {true, memory[blockIndex].size / 2});
        memory[blockIndex].size /= 2;
    }
    memory[blockIndex].actualSize = requestedSize;
}

void allocateMemory(vector<MemoryBlock> &memory, char process, int requestedSize)
{
    int blockIndex = -1;
    for (size_t i = 0; i < memory.size(); ++i)
    {
        if (memory[i].free && memory[i].size >= requestedSize)
        {
            if (blockIndex == -1 || memory[i].size < memory[blockIndex].size)
            {
                blockIndex = i;
            }
        }
    }

    if (blockIndex != -1)
    {
        memory[blockIndex].free = false;
        memory[blockIndex].process = process;
        splitBlock(memory, blockIndex, requestedSize);
    }
}

void releaseMemory(vector<MemoryBlock> &memory, char process)
{
    for (size_t i = 0; i < memory.size(); ++i)
    {
        if (memory[i].process == process)
        {
            memory[i].free = true;
            memory[i].process = 0;

            bool merged = true;

            while (merged)
            {
                merged = false;
                for (size_t j = 0; j < memory.size() - 1; ++j)
                {
                    if (memory[j].free && memory[j + 1].free && memory[j].size == memory[j + 1].size)
                    {
                        memory[j].size *= 2;
                        memory.erase(memory.begin() + j + 1);
                        merged = true;
                        break;
                    }
                }
            }

            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int testCases;
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <input_file>";
        exit(1);
    }
    ifstream input(argv[1]);
    input >> testCases;
    input.ignore();
    while (testCases--)
    {
        int U, L;
        input >> U >> L;
        string EmptyString;
        getline(input, EmptyString);
        getline(input, EmptyString);
        vector<MemoryBlock> memory;
        memory.push_back(MemoryBlock{true, (int)pow(2, U)});
        string input_string;
        while (getline(input, input_string) && !input_string.empty())
        {
            char process = input_string[0];
            int size = stoi(input_string.substr(2));

            if (size > 0)
            {
                allocateMemory(memory, process, size);
            }
            else
            {
                releaseMemory(memory, process);
            }
        }

        for (const auto &block : memory)
        {
            if (block.free)
            {
                cout << "Free Block: " << block.size << endl;
            }
            else
            {
                cout << block.process << ": " << block.actualSize << endl;
            }
        }

        if (testCases)
        {
            cout << endl;
        }
    }

    return 0;
}