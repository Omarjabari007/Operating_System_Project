#include <iostream>
#include <algorithm>
#include <limits>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;
using std::cout;
using std::endl;
// pcb data structure
struct PCB_PROCESS {
    bool is_finished;
    int id;
    int waitingTime;
    int turnaroundTime;
    int burstTime;
    int aRound_robin_RRivalTime;
    int remainingTime;
    int finishTime;
};
// Functions for each type ....
void FCFS(vector<PCB_PROCESS>& process_pcb , int Context_Switch_Time ) {
    int currentTime = 0;
    cout << "Gantt Chart Visualization :=> " <<  endl ; 
    for (auto& p :  process_pcb ) {
        if (currentTime!=0) {
            currentTime+=Context_Switch_Time; 
        }
        p.waitingTime = currentTime;
        currentTime += p.burstTime;
        p.finishTime =currentTime;
        p.turnaroundTime= p.finishTime;
        cout << " [ " << "|" << p.waitingTime << " - "  << p.finishTime << "|" << "P" << p.id<<"]";
    }
    cout << "|" << endl;
    cout << "PCB_PROCESS" << "\t" << "Finish_Time" << "\t" << "Waiting_Time" << "\t" << "Turnaround_Time" << "\t" << endl;
    for (const auto& p : process_pcb) {
        cout << "P" <<p.id << "\t"  << "\t" << p.finishTime << "\t"<<"\t" << p.waitingTime << "\t"<<"\t" << p.turnaroundTime << endl;
    }

    double Total_Burst_Time = 0 ;
    for ( const auto& p : process_pcb ) {
        Total_Burst_Time +=p.burstTime;
    }
    double CPU_UTILIZATION = (Total_Burst_Time/ (currentTime + Context_Switch_Time *(process_pcb.size() - 1 ))) *  100 ;
    cout << "  ---- >>" << "CPU_Utilization = " << CPU_UTILIZATION << "%" <<"<----" << endl;
    cout << "Made By Omar Jabari " << "  " << "id : 211144" << endl ;

    
}

void Shortest_remaining_Time_first( vector<PCB_PROCESS>& process_pcb,  int Context_Switch_Time ) {
    int currentTime = 0;
    int completed = 0;
    int Last_PCB_TRACKER = 0;
    int startPrintTime = 0;
     cout<<"\nGantt Chart:\n" << endl;
    while (completed < process_pcb.size()) {
        int idx = -1;
        int minTime = numeric_limits<int>::max();

        for (int i = 0; i < process_pcb.size(); i++ ) {
            if (process_pcb[i].aRound_robin_RRivalTime <= currentTime &&  !process_pcb[i].is_finished && 
                process_pcb[i].remainingTime <  minTime){
                minTime = process_pcb[i].remainingTime;
                idx = i;
            }
        }
        if (idx==  -1) {
            currentTime++;  
        }
        else {
            if (Last_PCB_TRACKER != process_pcb[idx].id &&Last_PCB_TRACKER != 0) {
                currentTime += Context_Switch_Time; 
            }
            if (Last_PCB_TRACKER != process_pcb[idx].id) {
                if (Last_PCB_TRACKER != 0) {
                    cout << " [ " << "|" << startPrintTime << " - " << currentTime << "|" << "P" << Last_PCB_TRACKER << "]";
                }
                startPrintTime = currentTime;
                Last_PCB_TRACKER =process_pcb[idx].id;
            }
            process_pcb[idx].remainingTime--;
            currentTime++;
            if (process_pcb[idx].remainingTime == 0) {
                process_pcb[idx].finishTime = currentTime;
                process_pcb[idx].turnaroundTime = currentTime - process_pcb[idx].aRound_robin_RRivalTime;
                process_pcb[idx].waitingTime = process_pcb[idx].turnaroundTime - process_pcb[idx].burstTime;
                process_pcb[idx].is_finished = true;
                completed++;
            }
        }
    }
    if (Last_PCB_TRACKER != 0) {
        cout << "|" << "P" << Last_PCB_TRACKER<< "(" <<  startPrintTime << "-" << currentTime << ")";
    }
       cout << "|" << endl;
    cout << "PCB_PROCESS" << "\t" << "Finish_Time" << "\t" << "Waiting_Time" << "\t" << "Turnaround_Time" << "\t" << endl;

    for (const auto& p : process_pcb) {
        cout << "P" << p.id << "\t\t" << p.finishTime << "\t\t" << p.waitingTime << "\t\t" << p.turnaroundTime << endl;
    }
    double Total_Burst_Time = 0;
    for (const auto& p :process_pcb) {
        Total_Burst_Time += p.burstTime;
    }
    double CPU_UTILIZATION = (Total_Burst_Time / (currentTime + Context_Switch_Time  *  (process_pcb.size() - 1)))  *  100 ;
    cout << "CPU Utilization = " << CPU_UTILIZATION << "%" << endl;
    cout << "Made By Omar Jabari " << "  " << "id : 211144" << endl;

}

void Round_robin_RR(vector<PCB_PROCESS>& process_pcb ,  int quantum , int Context_Switch_Time ) {
    queue<int> q ;  
    int currentTime =0;
    int Last_PCB_TRACKER = 0;
    int startPrintTime =0;
    cout << "\nGantt Chart:\n";

    for (int i = 0 ;i < process_pcb.size();i++ ) {
        if (process_pcb[i].aRound_robin_RRivalTime<= currentTime) {
            q.push(i) ;
        }
    }
    while ( !q.empty() ) {
        int idx = q.front();
        q.pop();

        if (Last_PCB_TRACKER != 0&& Last_PCB_TRACKER!= process_pcb[idx].id) {
            currentTime += Context_Switch_Time;  
        }

        int Run_TIME = min(quantum , process_pcb[idx].remainingTime);
        cout << " [ " << "|" << currentTime<< " - " << currentTime + Run_TIME << "|" << "P" << process_pcb[idx].id << "]";

        process_pcb[idx].remainingTime -= Run_TIME;
        currentTime += Run_TIME ;

        
        vector<int> tempQueue ;   
        while (!q.empty()) {
            tempQueue.push_back(q.front() );
            q.pop();
        }

        
        for (int i = 0; i < process_pcb.size(); i++ ) {
            if (!process_pcb[i].is_finished && process_pcb[i].aRound_robin_RRivalTime > process_pcb[idx].aRound_robin_RRivalTime && process_pcb[i].aRound_robin_RRivalTime <= currentTime && find(tempQueue.begin(), tempQueue.end(), i) == tempQueue.end()) {
                tempQueue.push_back(i);
            }
        }
        if (process_pcb[idx].remainingTime > 0 ) {
            tempQueue.push_back(idx); 
        }
        else {
            process_pcb[idx].is_finished = true;
            process_pcb[idx].finishTime = currentTime;
            process_pcb[idx].turnaroundTime = currentTime - process_pcb[idx].aRound_robin_RRivalTime;
            process_pcb[idx].waitingTime = process_pcb[idx].turnaroundTime - process_pcb[idx].burstTime;
        }
        for (int id : tempQueue ) {
            q.push(id);
        }
    }

    cout << "|" << endl; 
    cout << "PCB_PROCESS" << "\t" << "Finish_Time" << "\t" << "Waiting_Time" << "\t" << "Turnaround_Time" << "\t" << endl;
    for (const auto& p : process_pcb) {
        cout << "P" << p.id << "\t\t" << p.finishTime << "\t\t" << p.waitingTime << "\t\t" << p.turnaroundTime << endl;
    }
    double Total_Burst_Time = 0;
    for (const auto& p : process_pcb) {
        Total_Burst_Time += p.burstTime;
    }
    double CPU_UTILIZATION  = (Total_Burst_Time /  (currentTime + Context_Switch_Time  * (process_pcb.size() - 1))) * 100;
    cout << "CPU Utilization = " << CPU_UTILIZATION << "%" << endl;
    
    cout << "Made By Omar Jabari " << "  " << "id : 211144" << endl;
}
double calculateCPU_UTILIZATION(const vector<PCB_PROCESS>&  process_pcb , int Context_Switch_Time , int currentTime ) {
    double Total_Burst_Time = 0;
    for (const auto& p :  process_pcb ) {
        Total_Burst_Time  +=  p.burstTime ;
        
    }
    // for cpu uti : 
    double CPU_UTILIZATION =(Total_Burst_Time/( currentTime +Context_Switch_Time *  (process_pcb.size() - 1 ))) * 100 ;
    return CPU_UTILIZATION  ;
}

// omar jabari-------------------
int main() {
    int no_of_process, Context_Switch_Time ,  quantum ;
    cout << "--Scheduling Algorithms--" << endl; 
    cout << "1 - First Come First Served .." << endl;
    cout << "2 - Shortest Remaining Time First .. " << endl;
    cout << "3- Round Robin " << endl;
    cout << "Enter Your Option ->   1 - 3 ..." << endl;
    int option; cin >> option;
    string filePath = "C://Users//omarj//source//repos//Operatin_System_Project//Operatin_System_Project//os_read.txt";
    ifstream inFile(filePath);
    if (!inFile) {
        cout<<"Can't open the file ...... "<< filePath<<endl;
        return 1;
    }

    inFile >> no_of_process; 
    vector<PCB_PROCESS> process_pcb(no_of_process);
    for (int i = 0; i < no_of_process; i++) {
        inFile >> process_pcb[i].aRound_robin_RRivalTime >> process_pcb[i].burstTime;
        process_pcb[i].id =i + 1 ;  
        process_pcb[i].remainingTime = process_pcb[i].burstTime;
        process_pcb[i].is_finished = false;
    }
    inFile >> Context_Switch_Time;
    if (option == 3) {
        inFile >> quantum;
    }
    int currentTime = 0; 
    
    if (option == 1) {
        FCFS(process_pcb, Context_Switch_Time);
    }
    else if (option == 2) {
        Shortest_remaining_Time_first(process_pcb, Context_Switch_Time);
    }
    else if (option == 3) {
        Round_robin_RR(process_pcb, quantum, Context_Switch_Time);
    }
    else {
        cout << " Inavlid Option .. Try again . " << endl;
    }
    return 0;
}
