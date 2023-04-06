//Normal implementation, no classes

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

typedef enum {
    Free,
    Not
} BlockType;

typedef enum {
    Merge,
    Unmerge
} MergeType;

typedef struct {
    BlockType type;
    int size;
    char process;
    int actualSize;
} MemoryBlock;

void splitBlock(vector<MemoryBlock> &memory, int blockIndex, int requestedSize)
{
    while (memory[blockIndex].size / 2 >= requestedSize)
    {
        memory[blockIndex].type = BlockType::Not;
        memory.insert(memory.begin() + blockIndex + 1, {BlockType::Free, memory[blockIndex].size / 2});
        memory[blockIndex].size /= 2;
    }
    memory[blockIndex].actualSize = requestedSize;
}

void allocateMemory(vector<MemoryBlock> &memory, char process, int requestedSize)
{
    int blockIndex = -1;
    for (size_t i = 0; i < memory.size(); ++i)
    {
        if ((memory[i].type == BlockType::Free) && memory[i].size >= requestedSize)
        {
            if (blockIndex == -1 || memory[i].size < memory[blockIndex].size)
            {
                blockIndex = i;
            }
        }
    }

    if (blockIndex != -1)
    {
        memory[blockIndex].type = BlockType::Not;
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
            memory[i].type = BlockType::Free;
            memory[i].process = 0;

            MergeType Flag = MergeType::Merge;

            while (Flag == MergeType::Merge)
            {
                Flag = MergeType::Unmerge;
                for (size_t j = 0; j < memory.size() - 1; ++j)
                {
                    if ((memory[j].type == BlockType::Free) && (memory[j + 1].type == BlockType::Free) && memory[j].size == memory[j + 1].size)
                    {
                        memory[j].size *= 2;
                        memory.erase(memory.begin() + j + 1);
                        Flag = MergeType::Merge;
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
        exit(EXIT_FAILURE);
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
        memory.push_back(MemoryBlock{BlockType::Free, (int)pow(2, U)});
        string input_string;
        while (getline(input, input_string) && !input_string.empty())
        {
            char process;
            int size;
            istringstream ss(input_string);
            ss>> process>> size;

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
            if (block.type == BlockType::Free)
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