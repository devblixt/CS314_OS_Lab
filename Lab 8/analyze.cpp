#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <set>

using namespace std;

int vpages; // virtually addressable pages
int mpages; // number of page frames storeable in main memory
int dpages; // number of disk blocks available

bool contains(std::vector<int> &vec, int elementToFind)
{
    return std::find(vec.begin(), vec.end(), elementToFind) != vec.end();
}

void fifo(std::vector<int> seq)
{
    // cout<<vpages<<" "<<mpages<<" "<<dpages<<" "<<endl;
    vector<int> mmpages; // pages currently in main memory
    int hit = 0;
    int miss = 0;
    for (auto &itr : seq)
    {
        if (contains(mmpages, itr))
        {
            // std::cout<<"Pages contain "<<itr<<endl;
            ++hit;
        }
        else
        {
            // cout<<"Pages do not contain "<<itr<<", ";
            ++miss;
            if (mmpages.size() < mpages)
            {
                // cout<<"pushing!"<<endl;
                mmpages.push_back(itr);
            }
            else
            {
                // cout<<"getting rid of "<<*mmpages.begin()<<", pushing!"<<endl;
                mmpages.erase(mmpages.begin());
                mmpages.push_back(itr);
            }
        }
    }
    float percentage = (float) miss / ((float) hit + (float) miss);
    // for(auto &itr: mmpages){
    //     cout<<"itr: "<<itr<<std::endl;
    // }
    cout<<percentage*100<<" ";
}

void lru(std::vector<int> seq)
{
    vector<int> mmpages; // pages currently in main memory
    int hit = 0;
    int miss = 0;
    for (auto &itr : seq)
    {
        if (contains(mmpages, itr))
        {
            ++hit;
            auto it = std::find(mmpages.begin(), mmpages.end(), itr);
            std::rotate(mmpages.begin(), it, it + 1);
        }
        else
        {
            ++miss;
            if (mmpages.size() < mpages)
            {
                mmpages.insert(mmpages.begin(), itr);
            }
            else
            {
                mmpages.pop_back();
                mmpages.insert(mmpages.begin(), itr);
            }
        }
    }
    // cout << "LRU: " << endl;
    // cout << "hits: " << hit << " miss: " << miss << endl;
    //     for(auto &itr: mmpages){
    //     cout<<"itr: "<<itr<<std::endl;
    // }
    float percentage = (float) miss / ((float) hit + (float) miss);
    cout<<percentage*100 <<" ";
}

void random(std::vector<int> seq)
{
    vector<int> mmpages;
    int hit = 0;
    int miss = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    for (auto &itr : seq)
    {
        if (contains(mmpages, itr))
        {
            ++hit;
        }
        else
        {
            ++miss;
            std::uniform_int_distribution<int> dist(0, mmpages.size() - 1);

            int randomIndex = 0;
            if (!mmpages.empty())
            {
                randomIndex = dist(gen);
            }
            if (mmpages.size() < mpages)
            {
                mmpages.insert(mmpages.begin() + randomIndex, itr);
            }
            else
            {
                mmpages[randomIndex] = itr;
            }
        }
    }
    // cout << "RANDOM: " << endl;
    // cout << "hits: " << hit << " miss: " << miss << endl;
    float percentage = (float) miss / ( hit +  miss);
    cout<<percentage*100;
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout << "Usage : " << argv[0] << " <virtual pages> <main memory accomodatable pages> <disk blocks available> <input file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // bullshit argument
    //  if(atoi(argv[1])>atoi(argv[3])+atoi(argv[2])){
    //      std::cout<<"Can't have more virtually addressable pages than accomodatable pages!";
    //      exit(EXIT_FAILURE);
    //  }

    vpages = atoi(argv[1]);
    mpages = atoi(argv[2]);
    dpages = atoi(argv[3]);

    vector<int> seq;
    fstream input;
    input.open(argv[4], ios::in);
    string inputString;
    if (!getline(input, inputString))
    {
        std::cout << "empty file!" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::istringstream input_stream(inputString);
    int number;
    set<int> pages;
    while (input_stream >> number)
    {
        // cout<<number<<" ";
        seq.push_back(number);
        pages.insert(number);
    }
    auto set_size = pages.size();
    // cout<<"set size "<<set_size<<endl;
    if(set_size > (mpages + dpages)){
        cout<<"Can't accomodate pages!";
        exit(EXIT_FAILURE);
    }
    if(set_size > vpages) {
        cout<<"Too many pages, can't address with current virtual address space!"<<endl;
        exit(EXIT_FAILURE);
    }
    fifo(seq);
    lru(seq);
    random(seq);

    return 0;
}