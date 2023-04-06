#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include<cstdio>
#include<stack>
using namespace std;

typedef enum {
    Occ,
    Free
} BlockType;

typedef struct {
    string name;
    int value;
    BlockType type;
} Block;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cout<<"Usage: "<< argv[0]<<" <filename>";
        exit(EXIT_FAILURE);
    }

    ifstream infile(argv[1]);
    int num_cases;
    infile >> num_cases;
    infile.ignore();

    for(int i = 0; i < num_cases;i++) {
        int U,L;
        infile >> U >> L;
        infile.ignore();
        string line;
        int upper = 2^U;
        int lower = 2^L;
        vector<Block> buddy;
        Block genesis;
        genesis.name = "EMP";
        genesis.value = upper;
        genesis.type = BlockType::Free;
        buddy.push_back(genesis);
        while(getline(infile, line)){
            char c;
            int n;
            if (sscanf(line.c_str(), "%c %d", &c, &n) == 2) {
                // line is in the form <character> <integer>
                // process it here
                std::istringstream is(line);
                if(!((is>>c>>n))){
                    cout<<"Error processing!"<<endl;
                    exit(EXIT_FAILURE);
                }
                if(n!=0){
                    

                }
            } else {
                // line is empty (just a newline)
                // do nothing
            }

        }
    }
    return 1;
}