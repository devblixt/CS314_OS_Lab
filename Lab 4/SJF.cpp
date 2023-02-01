#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include <queue>
using namespace std;

enum Type
{
    CP,
    IO
};
enum Status
{
    taken,
    available
};

class Process
{
public:
    int arrival_time;
    int waiting_time = 0;
    vector<std::tuple<int, Type>> bursts;
    int completion_time = 0;
    int turnaround_time = 0;
    int total_burst_time = 0;
};

class Cpu
{
public:
    Status status;
    int process_id;
    int completion_time;
};

class IO_Device
{
public:
    Status status;
    int process_id;
    int completion_time;
};

struct PriorityElement
{
    int value1;
    int value2;
};

struct Compare
{
    bool operator()(const PriorityElement &lhs, const PriorityElement &rhs)
    {
        return lhs.value1 > rhs.value1;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " file" << endl;
        exit(1);
    }
    Cpu cpu;
    cpu.status = Status::available;
    IO_Device io;
    io.status = Status::available;
    vector<Process> processes;
    fstream input;
    input.open(argv[1], ios::in);
    string inputString;
    while (getline(input, inputString))
    {
        // arrival time
        int arrivalFlag = 0;
        bool cpu = true;
        Process newprocess;
        std::istringstream input_stream(inputString);
        int number;
        while (input_stream >> number)
        {
            if (number != -1)
            {
                if (arrivalFlag == 0)
                {
                    newprocess.arrival_time = number;
                    ++arrivalFlag;
                }
                else
                {
                    if (cpu)
                    {
                        newprocess.bursts.push_back(std::make_tuple(number, Type::CP));
                        newprocess.total_burst_time += number;
                        cpu = false;
                    }
                    else
                    {
                        newprocess.bursts.push_back(std::make_tuple(number, Type::IO));
                        newprocess.total_burst_time += number;
                        cpu = true;
                    }
                }
            }
            else
                break;
        }
        processes.push_back(newprocess);
    }
    int todo = processes.size();
    // start scheduler
    int system_time = 0;
    priority_queue<PriorityElement, std::vector<PriorityElement>, Compare> cpu_queue;
    queue<PriorityElement> io_queue;
    vector<Process> workingProcesses;
    // onboard all processes

    int done = 0;
    while (!(done == todo))
    {
        while (1)
        {
            if (!processes.empty())
            {
                if (system_time == processes.front().arrival_time)
                {
                    Process newProcess = processes.front();
                    cpu_queue.push({get<0>(newProcess.bursts.front()), workingProcesses.size()});
                    newProcess.bursts.erase(newProcess.bursts.begin());
                    workingProcesses.push_back(newProcess);
                    processes.erase(processes.begin());
                }
                else
                    break;
            }
            else
            {
                break;
            }
        }
        if (cpu.status == Status::available)
        {
            if (!cpu_queue.empty())
            {
                cpu.status = Status::taken;
                PriorityElement processDetail = cpu_queue.top();
                cpu.process_id = processDetail.value2;
                cpu.completion_time = processDetail.value1 + system_time;
                cpu_queue.pop();
            }
        }
        else
        {
            if (system_time == cpu.completion_time)
            {
                // completed with cpu

                cpu.status = Status::available;
                int process_id = cpu.process_id;
                if (!workingProcesses.at(process_id).bursts.empty())
                {
                    io_queue.push({get<0>(workingProcesses.at(process_id).bursts.front()), process_id});
                    workingProcesses.at(process_id).bursts.erase(workingProcesses.at(process_id).bursts.begin());
                }
                else
                {
                    // cout << "Process with process id " << process_id << " has completed at system time " << system_time << endl;
                    workingProcesses.at(process_id).turnaround_time = system_time - workingProcesses.at(process_id).arrival_time;
                    workingProcesses.at(process_id).waiting_time = workingProcesses.at(process_id).turnaround_time - workingProcesses.at(process_id).total_burst_time;
                    ++done;
                }
                if (!cpu_queue.empty())
                {
                    cpu.status = Status::taken;
                    PriorityElement processDetail = cpu_queue.top();
                    cpu.process_id = processDetail.value2;
                    cpu.completion_time = processDetail.value1 + system_time;
                    cpu_queue.pop();
                }
            }
        }
        if (io.status == Status::available)
        {
            if (!io_queue.empty())
            {
                io.status = Status::taken;
                PriorityElement queueDetail = io_queue.front();
                io.process_id = queueDetail.value2;
                io.completion_time = queueDetail.value1 + system_time;
                io_queue.pop();
            }
        }
        else
        {
            if (system_time == io.completion_time)
            {
                // completed an io operation
                io.status = Status::available;
                int process_id = io.process_id;
                if (!workingProcesses.at(process_id).bursts.empty())
                {
                    cpu_queue.push({get<0>(workingProcesses.at(process_id).bursts.front()), process_id});
                    workingProcesses.at(process_id).bursts.erase(workingProcesses.at(process_id).bursts.begin());
                }
                else
                {
                    // cout<<"Wrong completion for "<<process_id<<endl;
                    // cout << "Process with arrival time " << workingProcesses.at(process_id).arrival_time << " has completed"<< endl;
                    workingProcesses.at(process_id).turnaround_time = system_time - workingProcesses.at(process_id).arrival_time;
                    ++done;
                }
                if (!io_queue.empty())
                {
                    io.status = Status::taken;
                    PriorityElement processDetail = io_queue.front();
                    io.completion_time = processDetail.value1 + system_time;
                    io.process_id = processDetail.value2;
                    io_queue.pop();
                }
            }
        }
        ++system_time;
        // routine health check
        //  if(system_time%1==0){
        //  cout<<cpu_queue.top().value1<<" "<<cpu_queue.top().value2<<endl;
        //  cout<<"io queue "<<io_queue.front().value1<<" "<<io_queue.front().value2<<endl;
        //  }
        //  cout << "System time right now: " << system_time << endl;
    }
    float avg_time_for_process = system_time / done;
    float avg_turnaround_time = 0;
    float avg_wait_time = 0;
    float avg_penalty_ratio = 0;
    int size_p = workingProcesses.size();
    for (int i = 0; i < size_p; i++)
    {
        cout << "For Process ID " << i << " :" << endl;
        cout << "**********************************" << endl;
        cout << "Turnaround time is : " << workingProcesses.at(i).turnaround_time << endl;
        cout << "Total Waiting Time is : " << workingProcesses.at(i).waiting_time << endl;
        float penalty_ratio = (float)workingProcesses.at(i).turnaround_time / workingProcesses.at(i).total_burst_time;
        cout << "Penalty ratio is : " << penalty_ratio << endl
             << endl
             << endl;
        avg_turnaround_time += (float)workingProcesses.at(i).turnaround_time / size_p;
        avg_wait_time += (float)workingProcesses.at(i).waiting_time / size_p;
        avg_penalty_ratio += (float)penalty_ratio / size_p;
    }
    cout << "Average turnaround time is : " << avg_turnaround_time << endl;
    cout << "Average Waiting Time is : " << avg_wait_time << endl;
    cout << "Average penalty ratio is: " << avg_penalty_ratio << endl;
    cout << "Average time for process: " << avg_time_for_process << endl;
    return 0;
}