#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<math.h>
#include<stdbool.h>
#include<chrono>
#include<thread>
#include<atomic>
#include<semaphore.h>
using namespace std;
using namespace std::chrono;

sem_t s;
void child () {
    sem_post(&s);
    cout<<"Child ";
}

int main () {
    sem_init(&s,0,-3);
    thread T1(child);
    thread T2(child);
    thread T3(child);
    sem_wait(&s);
    cout<<"out of waiting";
    return 1;
}