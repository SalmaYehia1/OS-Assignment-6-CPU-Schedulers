#include "lab7.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <math.h>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

void Scheduler::execute()
{
    for (int i = 0; i < schedulongPoliceis.size(); i++)
    {
        clearTables();
        if (schedulongPoliceis[i].first == 1)
        {
            FCFS();
            if (type == "trace")
                trace(1, -1);
            else
                stats(1, -1);
        }
        else if (schedulongPoliceis[i].first == 2)
        {
            RR(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(2, schedulongPoliceis[i].second);
            else
                stats(2, schedulongPoliceis[i].second);
        }
        else if (schedulongPoliceis[i].first == 3)
        {
            SPN();
            if (type == "trace")
                trace(3, -1);
            else
                stats(3, -1);
        }
        else if (schedulongPoliceis[i].first == 4)
        {
            SRT();
            if (type == "trace")
                trace(4, -1);
            else
                stats(4, -1);
        }
        else if (schedulongPoliceis[i].first == 5)
        {
            HRRN();
            if (type == "trace")
                trace(5, -1);
            else
                stats(5, -1);
        }
        else if (schedulongPoliceis[i].first == 6)
        {
            FB1();
            if (type == "trace")
                trace(6, -1);
            else
                stats(6, -1);
        }
        else if (schedulongPoliceis[i].first == 7)
        {
            FB2i();
            if (type == "trace")
                trace(7, -1);
            else
                stats(7, -1);
        }
        else if (schedulongPoliceis[i].first == 8)
        {
            AGE(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(8, -1);
        }
    }
}
void Scheduler::trace(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 2)
    {
        if (argument > 10)
            cout << "RR-" << argument << " ";
        else
            cout << "RR-" << argument << "  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 3)
    {
        cout << "SPN   ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 4)
    {
        cout << "SRT   ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 5)
    {
        cout << "HRRN  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 6)
    {
        cout << "FB-1  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 7)
    {
        cout << "FB-2i ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 8)
    {
        cout << "Aging ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
}
void Scheduler::stats(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 2)
    {
        cout << "RR-" << argument << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 3)
    {
        cout << "SPN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 4)
    {
        cout << "SRT" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 5)
    {
        cout << "HRRN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 6)
    {
        cout << "FB-1" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 7)
    {
        cout << "FB-2i" << endl;
        printStats();
        cout << '\n';
    }
}
// notes:
/*
in preemptive ->while as we loop evry second
in non-preemptive -> for as we loop each process
*/
void Scheduler::FCFS()
{

    int time = 0;
    // assuming ordered processes by arrival time
    for (int i = 0; i < numberOfProcesses; i++)
    {
        Process &p = processes[i];

        // if finished executing at 3 and the next process came at 5 so jump to 5

        if (time < p.arrivalTime)
        {
            time = p.arrivalTime; // to update time with the arrival time
        }

        for (int m = 0; m < p.serveTime; m++) // executing process nd putting *
        {
            *(processesPrintingArray + i * maxSeconds + time) = '*';
            time++;
        }

        // for the waiting processes after the current the process
        for (int j = i + 1; j < numberOfProcesses; j++)

        {
            Process &p_next = processes[j];

            if (p_next.arrivalTime < time)
            // so yes waiting happened as the time i finished executing was after the next process arrived
            {
                int cpu_control = time - p.serveTime;
                int mark = max(p_next.arrivalTime, cpu_control);
                for (int t = mark; t < time; t++)
                {
                    if (*(processesPrintingArray + j * maxSeconds + t) == ' ')
                    {
                        *(processesPrintingArray + j * maxSeconds + t) = '.';
                    }
                }
            }
        }

        p.finishTime = time;
        p.turnAroundTime = time - p.arrivalTime;
        p.NormTurnTime = (float)p.turnAroundTime / p.serveTime;
    }
}

void Scheduler::RR(int quantum)
{
    int time = 0;
    int done = 0;

    queue<int> readyQueue;
    
    vector<int> finished(numberOfProcesses, 0);
    vector<int> arrived(numberOfProcesses, 0); 
    
    int index = -1;
    int q_spent = 0;    

    while (done < numberOfProcesses && time < maxSeconds)
    {

        for (int i = 0; i < numberOfProcesses; i++)
        {
            Process &p = processes[i];

            if (finished[i] == 0 && arrived[i] == 0 && p.arrivalTime <= time)
            {
                readyQueue.push(i);
                arrived[i] = 1;
            }
        }

        
        if (index != -1)
        {
            if (q_spent >= quantum)
            {
                readyQueue.push(index);
                index = -1; 
                q_spent = 0;
            }
        }

      
        if (index == -1 && !readyQueue.empty())
        {
            index = readyQueue.front();
            readyQueue.pop();
            q_spent = 0;
        }

        if (index != -1)
        {
            Process &p_executing = processes[index];
            *(processesPrintingArray +index * maxSeconds + time) = '*';
            p_executing.remainingTime--;
            q_spent++;

            // Check if Finished
            if (processes[index].remainingTime == 0)
            {
                finished[index] = 1;
                p_executing.finishTime = time + 1;
                p_executing.turnAroundTime = p_executing.finishTime - p_executing.arrivalTime;
                p_executing.NormTurnTime = (float)p_executing.turnAroundTime / p_executing.serveTime;
                done++;
                
                index = -1;
                q_spent = 0;
            }
        }

     
        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (arrived[j] == 1 && finished[j] == 0 && j != index)
            {
                char* slot = processesPrintingArray + j * maxSeconds + time;
                if (*slot != '*') *slot = '.';
            }
        }

        time++;
    }
}
void Scheduler::SPN()
{
    int time = 0;

    vector<int> finished(numberOfProcesses, 0);

    for (int c = 0; c < numberOfProcesses; c++)
    {
        
        int index = -1;
        int minDuration = INT_MAX;

        for (int i = 0; i < numberOfProcesses; i++)
        {
            // Check if not finished
            
            
            if (finished[i] == 0 && processes[i].arrivalTime <= time)
            {
                Process &p_searching = processes[i];

                if (p_searching.serveTime < minDuration)
                {
                    minDuration = p_searching.serveTime;
                    index = i;
                }
            }
        }

        if (index == -1) // as we should jump to the next as no process came in this time
        {
            time++;
            c--; // undo count
            continue;
        }

        Process &p_executing = processes[index];
        int start = time;

        for (int m = 0; m < p_executing.serveTime; m++)
        {
            *(processesPrintingArray + index * maxSeconds + time) = '*';
            time++;
        }

        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (index == j || finished[j] == 1)
                continue; // Skip if done is 1 or if it is the same process i am in

            Process &p_wait = processes[j];
            if (p_wait.arrivalTime < time)
            {
                int mark = max(p_wait.arrivalTime, start);
                for (int t = mark; t < time; t++)
                {
                    if (*(processesPrintingArray + j * maxSeconds + t) == ' ')
                        *(processesPrintingArray + j * maxSeconds + t) = '.';
                }
            }
        }

        // mark done = 1
        finished[index] = 1;

        p_executing.finishTime = time;
        p_executing.turnAroundTime = time - p_executing.arrivalTime;
        p_executing.NormTurnTime = (float)p_executing.turnAroundTime / p_executing.serveTime;
    }
}

void Scheduler::SRT()
{
    // preemptive version of SPN
    int time = 0;
    int done = 0;

    vector<int> finished(numberOfProcesses, 0);

    while (done < numberOfProcesses)
    {
        int index = -1;
        int min_remaining = INT_MAX;

        for (int i = 0; i < numberOfProcesses; i++)
        {
            // Check if not finished
            if (finished[i] == 0 && processes[i].arrivalTime <= time)
            {
                Process &p_searching = processes[i];

                if (p_searching.remainingTime < min_remaining)
                {
                    min_remaining = p_searching.remainingTime;
                    index = i;
                }
            }
        }

        if (index == -1) // as we should jump to the next as no process came in this time
        {
            time++;
            continue;
        }

        Process &p_executing = processes[index];

        *(processesPrintingArray + index * maxSeconds + time) = '*';
        p_executing.remainingTime = p_executing.remainingTime - 1;

        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (index == j || finished[j] == 1)
                continue; // Skip if done is 1 or if it is the same process i am in

            Process &p_wait = processes[j];
            if (p_wait.arrivalTime <= time)
            {
                if (*(processesPrintingArray + j * maxSeconds + time) == ' ')
                    *(processesPrintingArray + j * maxSeconds + time) = '.';
            }
        }

        time++;

        if (p_executing.remainingTime == 0)
        {
            done++;
            // mark done = 1
            finished[index] = 1;

            p_executing.finishTime = time;
            p_executing.turnAroundTime = time - p_executing.arrivalTime;
            p_executing.NormTurnTime = (float)p_executing.turnAroundTime / p_executing.serveTime;
        }
    }
}

void Scheduler::HRRN()
// same code as SPN bec, non-preemptive
// only the difference is in getting the ratio
{
    int time = 0;

    vector<int> finished(numberOfProcesses, 0);

    for (int c = 0; c < numberOfProcesses; c++)
    {
        int index = -1;
        int max_ratio = INT_MIN;

        for (int i = 0; i < numberOfProcesses; i++)
        {
            // Check if not finished
            if (finished[i] == 0 && processes[i].arrivalTime <= time)
            {

                Process &p_searching = processes[i];

                int wait_time = time - p_searching.arrivalTime;
                float ratio = (float)(wait_time + p_searching.serveTime) / p_searching.serveTime;
                if (ratio > max_ratio)
                {
                    max_ratio = ratio;
                    index = i;
                }
            }
        }

        if (index == -1) // as we should jump to the next as no process came in this time
        {
            time++;
            c--; // undo count
            continue;
        }

        Process &p_executing = processes[index];
        int start = time;

        for (int m = 0; m < p_executing.serveTime; m++)
        {
            *(processesPrintingArray + index * maxSeconds + time) = '*';
            time++;
        }

        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (index == j || finished[j] == 1)
                continue; // Skip if done is 1 or if it is the same process i am in

            Process &p_wait = processes[j];
            if (p_wait.arrivalTime < time)
            {
                int mark = max(p_wait.arrivalTime, start);
                for (int t = mark; t < time; t++)
                {
                    if (*(processesPrintingArray + j * maxSeconds + t) == ' ')
                        *(processesPrintingArray + j * maxSeconds + t) = '.';
                }
            }
        }

        // mark done = 1
        finished[index] = 1;

        p_executing.finishTime = time;
        p_executing.turnAroundTime = time - p_executing.arrivalTime;
        p_executing.NormTurnTime = (float)p_executing.turnAroundTime / p_executing.serveTime;
    }
}


void Scheduler::FB1()
{
    int time = 0;
    int done = 0;

    vector<queue<int>> priority_queues(numberOfProcesses); //3shan in worst case momkn ykoon number of quueus = number of processes
    vector<int> queue_level(numberOfProcesses, -1); //3shan a3rf kol process feen delwa2ty -1 3shan lesa not arrived

    vector<int> q_spent(numberOfProcesses, 0);     
    vector<int> finished(numberOfProcesses, 0);   // 0->not finished 1-> finished
    
    int index = -1; 

    while (done < numberOfProcesses && time < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            Process &p = processes[i];

            if (finished[i] == 0 && queue_level[i] == -1 && p.arrivalTime <= time)
            {
                priority_queues[0].push(i);
                queue_level[i] = 0; //moved to q(0)
                q_spent[i] = 0;
            }
        }

        if (index != -1)
        {
            int q = 1;

            if (q_spent[index] >= q)   //so finished executing q but we shouldnt move the process to another level
            //we should first make sure taht there are other processes waiting
            {
                
                bool waiting_process = false;
                for (int k = 0; k < numberOfProcesses; k++) {
                    if (!priority_queues[k].empty()) 
                    {
                         waiting_process = true; 
                         break; //ybaa keda found process waiting
                    }
                }

                if (waiting_process)
                {
                    int nextLevel = min(queue_level[index] + 1, numberOfProcesses - 1);
                    queue_level[index] = nextLevel;
                    priority_queues[nextLevel].push(index);
                }
                else
                {
                    int process_level=queue_level[index];
                   
                    priority_queues[process_level].push(index);  //so keep in the same level
                }

                q_spent[index] = 0; 
                index = -1; 
            }
        }

        if (index == -1)
        {
            for (int k = 0; k < numberOfProcesses; k++)
            {
                if (!priority_queues[k].empty())
                {
                    index = priority_queues[k].front();
                    priority_queues[k].pop();
                    break;
                }
            }
        }

        if (index != -1)
        {
            Process &p_executing = processes[index];
            *(processesPrintingArray + index * maxSeconds + time) = '*';
            p_executing .remainingTime--;
            q_spent[index]++;

            if (p_executing.remainingTime == 0)
            {
                finished[index] = 1;
                p_executing .finishTime = time + 1;
                p_executing .turnAroundTime = p_executing .finishTime - p_executing .arrivalTime;
                p_executing .NormTurnTime = (float)p_executing .turnAroundTime / p_executing .serveTime;
                done++;
                index = -1; 
            }
        }

        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (finished[j] == 0 && queue_level[j] != -1 && j != index)
            {
                char* slot = processesPrintingArray + j * maxSeconds + time;
                if (*slot != '*') *slot = '.';
            }
        }

        time++;
    }
}



void Scheduler::FB2i()
{

    //the diffence from fb1 and fb2i same exact code only q is diffent powers of 2 
    int time = 0;
    int done = 0;

    vector<queue<int>> priority_queues(numberOfProcesses); //3shan in worst case momkn ykoon number of quueus = number of processes
    vector<int> queue_level(numberOfProcesses, -1); //3shan a3rf kol process feen delwa2ty -1 3shan lesa not arrived

    vector<int> q_spent(numberOfProcesses, 0);     
    vector<int> finished(numberOfProcesses, 0);   // 0->not finished 1-> finished
    
    int index = -1; 

    while (done < numberOfProcesses && time < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            Process &p = processes[i];

            if (finished[i] == 0 && queue_level[i] == -1 && p.arrivalTime <= time)
            {
                priority_queues[0].push(i);
                queue_level[i] = 0; //moved to q(0)
                q_spent[i] = 0;
            }
        }

        if (index != -1)
        {
//delwa2ty fb2i fa q =2^level
            int current_level = queue_level[index];
            int q = (int)pow(2, current_level); 

            if (q_spent[index] >= q)   
            {
                bool waiting_process = false;
                for (int k = 0; k < numberOfProcesses; k++) {
                    if (!priority_queues[k].empty()) 
                    {
                         waiting_process = true; 
                         break; //ybaa keda found process waiting
                    }
                }

                if (waiting_process)
                {
                    int nextLevel = min(queue_level[index] + 1, numberOfProcesses - 1);  //checking as dont exceedthe max number of queues 
                    queue_level[index] = nextLevel;
                    priority_queues[nextLevel].push(index);
                }
                else
                {
                    int process_level = queue_level[index];
                    priority_queues[process_level].push(index);  //so keep in the same level
                }

                q_spent[index] = 0; 
                index = -1; 
            }
        }

        if (index == -1)
        {
            for (int k = 0; k < numberOfProcesses; k++)
            {
                if (!priority_queues[k].empty())
                {
                    index = priority_queues[k].front();
                    priority_queues[k].pop();
                    break;
                }
            }
        }

        if (index != -1)

        {

            Process &p_executing = processes[index];

            *(processesPrintingArray + index * maxSeconds + time) = '*';
            p_executing.remainingTime--;
            q_spent[index]++;

            if (p_executing.remainingTime == 0)
            {
                finished[index] = 1;
                p_executing.finishTime = time + 1;
                p_executing.turnAroundTime = p_executing.finishTime - p_executing.arrivalTime;
                p_executing.NormTurnTime = (float)p_executing.turnAroundTime / p_executing.serveTime;
                done++;
                index = -1; 
            }
        }

        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (finished[j] == 0 && queue_level[j] != -1 && j != index)
            {
                char* slot = processesPrintingArray + j * maxSeconds + time;
                if (*slot != '*') *slot = '.';
            }
        }

        time++;
    }
}

void Scheduler::AGE(int quantum)
{
  

    int time = 0;
    int done = 0;

    if (quantum < 1) 
    {
        quantum = 1;
    }

    vector<int> current_priority(numberOfProcesses);//3shan elpriority btzeed 1 lma btb2a waiting
    vector<int> finished(numberOfProcesses, 0); //zy elba2yy 3shan a3rf done wala la
    
    vector<int> waiting_time(numberOfProcesses, 0); //lw 7sl tie zy fe 11a meen kan mstny elawel howa leeeh higher priority 

    for(int i = 0; i < numberOfProcesses; i++) {
        Process &p_current = processes[i];

        current_priority[i] = p_current.priority;
    }

    int currentIdx = -1; 
    int q_spent = 0; 

    while (done < numberOfProcesses && time < maxSeconds)
    {
        bool need_reschedule = false;
        if (currentIdx == -1 || finished[currentIdx] == 1 || q_spent >= quantum)
        {
            need_reschedule = true;
        }

        if (need_reschedule)
        {
            if (currentIdx != -1 && finished[currentIdx] == 0)
            {
                current_priority[currentIdx] = processes[currentIdx].priority;
            }

            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (processes[i].arrivalTime <= time && finished[i] == 0 && i != currentIdx)
                {
                    current_priority[i]++; 
                }
            }

            int max_p = -1;
            int nextIdx = -1;
            bool found_process = false;

            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (finished[i] == 0 && processes[i].arrivalTime <= time)
                {
                    found_process = true;
                    
                    if (current_priority[i] > max_p)
                    {
                        max_p = current_priority[i];
                        nextIdx = i;
                    }
                    //if tie break it check waiting time
                    else if (current_priority[i] == max_p)
                    {
                        if (waiting_time[i] > waiting_time[nextIdx])
                        {
                            nextIdx = i;
                        }
                    }
                }
            }

            // context switching 
            if (found_process)
            {
                currentIdx = nextIdx;
            } 
            else currentIdx = -1;

            q_spent = 0;
        }

        if (currentIdx != -1)
        {
            if (time < maxSeconds) {
                *(processesPrintingArray + currentIdx * maxSeconds + time) = '*';
            }
            
            waiting_time[currentIdx] = 0;

            Process &p_executing = processes[currentIdx];


            p_executing.remainingTime--;
            q_spent++;

            if (p_executing.remainingTime == 0)
            {
                finished[currentIdx] = 1;
                p_executing.finishTime = time + 1;
                p_executing.turnAroundTime =p_executing.finishTime - p_executing.arrivalTime;
                p_executing.NormTurnTime = (float)p_executing.turnAroundTime /p_executing.serveTime;
                done++;
            }
        }

        for (int j = 0; j < numberOfProcesses; j++)
        {
            if (finished[j] == 0 && processes[j].arrivalTime <= time && j != currentIdx)
            {
                if (time < maxSeconds) {
                    char* slot = processesPrintingArray + j * maxSeconds + time;
                    if (*slot != '*') *slot = '.';
                }
                
                waiting_time[j]++;
            }
        }

        time++;
    }
}




void Scheduler::printTracing()
{
    for (int process = 0; process < numberOfProcesses; process++)
    {
        cout << processes[process].processName << "     |";
        for (int time = 0; time < maxSeconds; time++)
            cout << *(processesPrintingArray + process * maxSeconds + time) << '|';
        cout << " \n";
    }
}




void Scheduler::printStats()
{
    float sum, mean, sum2;
    cout << "Process    |";
    for (int i = 0; i < numberOfProcesses; i++)
        cout << "  " << processes[i].processName << "  |";
    cout << endl;
    cout << "Arrival    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].arrivalTime << "  |";
        else
            cout << " " << processes[i].arrivalTime << "  |";
    }
    cout << endl;
    cout << "Service    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].serveTime << "  |";
        else
            cout << " " << processes[i].serveTime << "  |";
    }
    cout << " Mean|" << endl;
    cout << "Finish     |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].finishTime >= 10)
            cout << " " << processes[i].finishTime << "  |";
        else
            cout << "  " << processes[i].finishTime << "  |";
    }
    cout << "-----|" << endl;
    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcesses; i++)
    {

        if (processes[i].turnAroundTime >= 10)
            cout << " " << (int)processes[i].turnAroundTime << "  |";
        else
            cout << "  " << (int)processes[i].turnAroundTime << "  |";
        sum += processes[i].turnAroundTime;
    }
    cout << fixed;
    cout << setprecision(2);
    mean = (sum * 1.0) / numberOfProcesses;
    if (mean >= 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
    cout << "NormTurn   |";
    sum2 = 0;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].NormTurnTime > 10)
            cout << processes[i].NormTurnTime << "|";
        else
            cout << " " << processes[i].NormTurnTime << "|";
        sum2 += (processes[i].NormTurnTime * 1.0);
    }

    mean = (sum2 * 1.0) / numberOfProcesses;
    if (mean > 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
}
void Scheduler::clearTables()
{

    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < maxSeconds; j++)
            *(processesPrintingArray + i * maxSeconds + j) = ' ';
    }
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processes[i].finishTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].NormTurnTime = 0;
        processes[i].processState = ' ';
        processes[i].remainingTime = processes[i].serveTime;
    }
    processorBusy = false;
    while (!readyQueue.empty())
        readyQueue.pop();
    while (!readyPriorityQueue.empty())
        readyPriorityQueue.pop();
}
void Scheduler::splitPolicyAndParameter(string str)
{
    string w = "";
    pair<int, int> policies;
    bool parameterExists = false;
    policies.second = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            parameterExists = true;
            policies.first = stoi(w);
            w = "";
        }
        else if (str[i] == ',')
        {
            if (parameterExists)
                policies.second = stoi(w);
            else
            {
                policies.first = stoi(w);
                policies.second = -1;
            }
            w = "";
            schedulongPoliceis.push_back(policies);
            parameterExists = false;
        }
        else
            w = w + str[i];
    }
    if (parameterExists)
        policies.second = stoi(w);
    else
        policies.first = stoi(w);
    schedulongPoliceis.push_back(policies);
}
void Scheduler::splitPrcoessAndTimes(string str, int id)
{
    Process process;
    string w = "";
    process.processName = str[0];
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i] == ',')
        {
            process.arrivalTime = stoi(w);
            w = "";
        }
        else
            w = w + str[i];
    }
    processorBusy = false;
    process.processState = ' ';
    if (schedulongPoliceis[0].first == 8)
    {
        process.priority = stoi(w);
        process.currentPriority = stoi(w);
    }
    else
        process.serveTime = stoi(w);
    process.remainingTime = process.serveTime;
    process.waitingTime = 0;
    process.id = id;
    processes.push_back(process);
}
void Scheduler::readFile()
{
    processorBusy = false;
    string temp1, temp2;
    cin >> type;
    cin >> temp1;
    splitPolicyAndParameter(temp1);
    cin >> maxSeconds;
    cin >> numberOfProcesses;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cin >> temp2;
        splitPrcoessAndTimes(temp2, i);
    }
    processesPrintingArray = new char[numberOfProcesses * maxSeconds];
    currentProcess.processName = 0;
    currentProcess.q = 0;
    clearTables();
}
void Scheduler::printHeader()
{
    for (int i = 0; i < maxSeconds + 1; i++)
        cout << i % 10 << ' ';
    cout << "\n";
    printDashes();
}
void Scheduler::printDashes()
{
    cout << "------------------------------------------------";
}
int main(void)
{
    Scheduler scheduler;
    scheduler.readFile();
    scheduler.execute();
    return 0;
}