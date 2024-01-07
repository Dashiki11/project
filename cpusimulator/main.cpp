#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Structure to represent a process/job
struct Process {
    int burst_time;
    int arrival_time;
    int priority;
    int waiting_time;
};

// Function to read input from file and store processes
vector<Process> readInputFromFile(const string& filename) {
    vector<Process> processes;
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Unable to open the file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;
        Process process;

        getline(ss, token, ':');
        process.burst_time = stoi(token);

        getline(ss, token, ':');
        process.arrival_time = stoi(token);

        getline(ss, token, ':');
        process.priority = stoi(token);

        process.waiting_time = 0;

        processes.push_back(process);
    }

    inputFile.close();
    return processes;
}

// Function to perform First Come First Served (FCFS) Scheduling
void FCFS(vector<Process>& processes) {
    int n = processes.size();

    // Calculating completion times and waiting times
    int currentTime = 0;
    for (int i = 0; i < n; ++i) {
        if (currentTime < processes[i].arrival_time) {
            currentTime = processes[i].arrival_time;
        }

        processes[i].waiting_time = currentTime - processes[i].arrival_time;

        currentTime += processes[i].burst_time;
    }
}

// Function to perform Shortest-Job-First (SJF) Scheduling(non preemptive)
void SJF(vector<Process>& processes) {
    // Calculate waiting times for each process
    int n = processes.size();

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.burst_time < b.burst_time;
        });

    int currentTime = 0;
    for (int i = 0; i < n; ++i) {
        if (currentTime < processes[i].arrival_time) {
            currentTime = processes[i].arrival_time;
        }

        processes[i].waiting_time = currentTime - processes[i].arrival_time;
        currentTime += processes[i].burst_time;
    }
}

// Function to perform Shortest-Job-First (SJF) Scheduling(preemptive)
void preemptiveSJF(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    vector<int> remaining_time(n);

    for (int i = 0; i < n; ++i) {
        remaining_time[i] = processes[i].burst_time;
    }

    while (completed != n) {
        int shortest = INT_MAX, shortestIndex = -1;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= currentTime && remaining_time[i] < shortest && remaining_time[i] > 0) {
                shortest = remaining_time[i];
                shortestIndex = i;
            }
        }

        if (shortestIndex == -1) {
            currentTime++;
            continue;
        }

        remaining_time[shortestIndex]--;

        if (remaining_time[shortestIndex] == 0) {
            completed++;
            int finishTime = currentTime + 1;
            processes[shortestIndex].waiting_time = finishTime - processes[shortestIndex].burst_time - processes[shortestIndex].arrival_time;
        }

        currentTime++;
    }
}


// Function to perform Priority Scheduling(non preemptive)
void priorityScheduling(vector<Process>& processes) {
    int n = processes.size();

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.priority < b.priority;
        });

    int currentTime = 0;
    for (int i = 0; i < n; ++i) {
        if (currentTime < processes[i].arrival_time) {
            currentTime = processes[i].arrival_time;
        }

        processes[i].waiting_time = currentTime - processes[i].arrival_time;
        currentTime += processes[i].burst_time;
    }
}

// Function to perform Priority Scheduling (preemptive)
void preemptivePriority(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    vector<int> remaining_time(n);

    for (int i = 0; i < n; ++i) {
        remaining_time[i] = processes[i].burst_time;
    }

    while (completed != n) {
        int highestPriority = INT_MAX, highestPriorityIndex = -1;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= currentTime && processes[i].priority < highestPriority && remaining_time[i] > 0) {
                highestPriority = processes[i].priority;
                highestPriorityIndex = i;
            }
        }

        if (highestPriorityIndex == -1) {
            currentTime++;
            continue;
        }

        remaining_time[highestPriorityIndex]--;

        if (remaining_time[highestPriorityIndex] == 0) {
            completed++;
            int finishTime = currentTime + 1;
            processes[highestPriorityIndex].waiting_time = finishTime - processes[highestPriorityIndex].burst_time - processes[highestPriorityIndex].arrival_time;
        }

        currentTime++;
    }
}

// Function to perform Round Robin Scheduling
void roundRobin(vector<Process>& processes, int time_quantum) {
    int n = processes.size();
    vector<int> remaining_time(n);

    for (int i = 0; i < n; ++i) {
        remaining_time[i] = processes[i].burst_time;
    }

    int currentTime = 0;
    while (true) {
        bool done = true;
        for (int i = 0; i < n; ++i) {
            if (remaining_time[i] > 0) {
                done = false;
                if (remaining_time[i] > time_quantum) {
                    currentTime += time_quantum;
                    remaining_time[i] -= time_quantum;
                }
                else {
                    currentTime += remaining_time[i];
                    processes[i].waiting_time = currentTime - processes[i].burst_time - processes[i].arrival_time;
                    remaining_time[i] = 0;
                }
            }
        }
        if (done) {
            break;
        }
    }
}

// Function to display results on screen
void displayResults(const vector<Process>& processes, const string& method) {
    // Display results for each process and average waiting time
    cout << "Scheduling Method: " << method << endl;
    cout << "Process Waiting Times:" << endl;

    for (size_t i = 0; i < processes.size(); ++i) {
        cout << "P" << i + 1 << ": " << processes[i].waiting_time << " ms" << endl;
    }

    // Calculate average waiting time
    double totalWaitingTime = 0;
    for (const auto& process : processes) {
        totalWaitingTime += process.waiting_time;
    }

    double averageWaitingTime = totalWaitingTime / processes.size();

    cout << "Average Waiting Time: " << averageWaitingTime << " ms" << endl;
}

// Function to write results to the output file
void writeResultsToFile(const vector<Process>& processes, const string& method, const string& filename) {
    // Write results to the output file
    ofstream outputFile(filename, ios::app);

    if (!outputFile) {
        cerr << "Unable to open the output file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    outputFile << "Scheduling Method: " << method << endl;
    outputFile << "Process Waiting Times:" << endl;

    for (size_t i = 0; i < processes.size(); ++i) {
        outputFile << "P" << i + 1 << ": " << processes[i].waiting_time << " ms" << endl;
    }

    double totalWaitingTime = 0;
    for (const auto& process : processes) {
        totalWaitingTime += process.waiting_time;
    }

    double averageWaitingTime = totalWaitingTime / processes.size();

    outputFile << "Average Waiting Time: " << averageWaitingTime << " ms" << endl;
    outputFile.close();
}

int main(int argc, char* argv[]) {
    string inputFile = "input.txt";
    string outputFile = "output.txt";
    string choiceOfSchedulingMethod;
    int timeQuantum;
    bool exitProgram = false;
    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        }
    }

    if (inputFile.empty() || outputFile.empty()) {
        std::cerr << "Usage: " << argv[0] << " -f <input_file> -o <output_file>\n";
        return 1;
    }

    vector<Process> processes = readInputFromFile(inputFile);

    // Menu-driven simulation
    int option;
    do {
        cout << "CPU Scheduler Simulator\n";
        cout << "1) Scheduling Method (None)\n";
        cout << "2) Show Result\n";
        cout << "3) End Program\n";
        cout << "Option > ";
        cin >> option;

        switch (option) {
        case 1:
            // Ask user for scheduling method
            cout << "Choose Scheduling Method:\n";
            cout << "1) First Come First Served (FCFS)\n";
            cout << "2) Shortest Job First (SJF)\n";
            cout << "3) Preemptive Shortest Job First (PSJF)\n";
            cout << "4) Priority Scheduling\n";
            cout << "5) Preemptive Priority Scheduling\n";
            cout << "6) Round Robin Scheduling\n";
            int methodChoice;
            cin >> methodChoice;

            switch (methodChoice) {
            case 1:
                choiceOfSchedulingMethod = "FCFS";
                break;
            case 2:
                choiceOfSchedulingMethod = "SJF";
                break;
            case 3:
                choiceOfSchedulingMethod = "PSJF";
                break;
            case 4:
                choiceOfSchedulingMethod = "Priority";
                break;
            case 5:
                choiceOfSchedulingMethod = "P Priority";
                break;
            case 6:
                choiceOfSchedulingMethod = "RoundRobin";
                // Prompt and get the time quantum for Round Robin scheduling
                cout << "Enter time quantum for Round Robin Scheduling: ";
                cin >> timeQuantum;
                break;

            default:
                cout << "Invalid choice. Please choose a valid scheduling method." << endl;
                break;

            }
        case 2:
            // Display results on screen
            // Add results to output file

            if (choiceOfSchedulingMethod == "FCFS") {
                FCFS(processes);
                displayResults(processes, "First Come First Served (FCFS)");
                writeResultsToFile(processes, "First Come First Served (FCFS)", outputFile);
            }
            else if (choiceOfSchedulingMethod == "SJF") {
                SJF(processes);
                displayResults(processes, "Shortest Job First (SJF)");
                writeResultsToFile(processes, "Shortest Job First (SJF)", outputFile);
            }
            else if (choiceOfSchedulingMethod == "Priority") {
                priorityScheduling(processes);
                displayResults(processes, "Priority Scheduling");
                writeResultsToFile(processes, "Priority Scheduling", outputFile);
            }
            else if (choiceOfSchedulingMethod == "RoundRobin") {
                // Assuming timeQuantum holds the value for Round Robin
                roundRobin(processes, timeQuantum);
                displayResults(processes, "Round Robin Scheduling (Time Quantum: " + to_string(timeQuantum) + ")");
                writeResultsToFile(processes, "Round Robin Scheduling (Time Quantum: " + to_string(timeQuantum) + ")", outputFile);
            }
            else if (choiceOfSchedulingMethod == "PSJF") {
                preemptiveSJF(processes);
                displayResults(processes, "Preemptive Shortest Job First (PSJF)");
                writeResultsToFile(processes, "Preemptive Shortest Job First (PSJF)", outputFile);
            }
            else if (choiceOfSchedulingMethod == "P Priority") {
                preemptivePriority(processes);
                displayResults(processes, "Preemptive Priority Scheduling");
                writeResultsToFile(processes, "Preemptive Priority Scheduling", outputFile);
            }
            else {
                cout << "Please choose a scheduling method first!" << endl;
            }
            break;
        case 3:  // Write results to the output file and end the program
            if (choiceOfSchedulingMethod.empty()) {
                cout << "Please choose a scheduling method first!" << endl;
                break;
            }
            exitProgram = true;

        default:
            cout << "Invalid option. Please choose a valid option.\n";
        }
    } while (!exitProgram);

   return 0;
}

