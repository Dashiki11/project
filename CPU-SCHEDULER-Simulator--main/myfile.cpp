#include <algorithm>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

struct linearqueue {

  int processus[6];
};

void insert(linearqueue *l, int mytimer, int marge) {
  for (int i = 0; i < marge; i++) {

    int *process = l[i].processus;
    if (process[5] == 0 && process[1] == mytimer) {
      process[5] = 1;
    }
  }
}

void executionProcess(linearqueue *eProcesses, int eProcessTE, int marge) {
  int counter = 0;

  for (int i = 0; i < marge; i++) {

    int *process = eProcesses[i].processus;
    if (i == eProcessTE && process[4] == 0) {
      process[0] = process[0] - 1;

      if (process[0] == 0) {
        process[4] = 1;
      }

    } else if (process[5] == 1 && process[4] == 0) {
      process[3] = process[3] + 1;
    }

    counter++;
  }
}

int schedulingProcessChoice(linearqueue *l, int type, int marge) {
  int processSelected = -1;
  int lowestProcess = 0;
  int counter = 0;
  bool boolProcessSelected = false;

  for (int i = 0; i < marge; i++) {

    int *process = l[i].processus;

    if ((boolProcessSelected == false && process[5] == 1 && process[4] == 0) ||
        (boolProcessSelected == true && process[5] == 1 && process[4] == 0 &&
         process[type] < lowestProcess)) {
      lowestProcess = process[type];
      processSelected = counter;
      boolProcessSelected = true;
    }
    counter++;
  }

  return processSelected;
}

void schedulingPreemptive(linearqueue *pProcesses, int type, int marge) {
  bool tester = true;
  int time = 0;
  cout << "schedulingPreemptive start\n";
  while (tester == true) {
    insert(pProcesses, time, marge);
    int processesToExecute = schedulingProcessChoice(pProcesses, type, marge);
    if (processesToExecute == -1) {
      tester = false;
    } else {
      executionProcess(pProcesses, processesToExecute, marge);
      time++;
    }
  }
  cout << "Scheduling Finish\n";
  cout << time << "time\n";
}

void schedulingNonPreemptive(linearqueue *pProcesses, int type, int marge) {
  bool tester = true;
  int time = 0;
  cout << "schedulingNonPreemptive start\n";
  insert(pProcesses, time, marge);
  while (tester == true) {
    int processesToExecute = schedulingProcessChoice(pProcesses, type, marge);
    if (processesToExecute == -1) {
      tester = false;
    } else {
      int *process = pProcesses[processesToExecute].processus;
      while (process[4] == 0) {
        executionProcess(pProcesses, processesToExecute, marge);
        time++;
        insert(pProcesses, time, marge);
      }
    }
  }
  cout << "Scheduling Finish\n";
  cout << time << "time\n";
}

void schedulingRoundRobin(linearqueue *pProcesses, int quantum, int marge) {
  bool tester = true;
  int time = 0;
  cout << "schedulingRoundRobin start\n";
  insert(pProcesses, time, marge);
  int processListSize = marge;
  int currentprocess = 0;
  while (tester == true) {
    for (int i = 0; i < quantum; i++) {
      int processesToExecute = schedulingProcessChoice(pProcesses, 1, marge);
      int *process = pProcesses[currentprocess].processus;
      if (processesToExecute == -1) {
        tester = false;
        break;
      } else if (process[4] == 1) {
        break;
      } else {
        executionProcess(pProcesses, currentprocess, marge);
        insert(pProcesses, time, marge);
        time++;
      }
    }
    currentprocess++;
    if (currentprocess == processListSize) {
      currentprocess = 0;
    }
  }
}

float getAverage(linearqueue *pProcesses, int marge) {
  int sum = 0;
  for (int i = 0; i < marge; i++) {
    int *process = pProcesses[i].processus;
    sum = sum + process[3];
  }
  return (sum / 5);
}
void showWaitingTime(linearqueue *pProcesses, int marge) {
  for (int i = 0; i < marge; i++) {
    int *process = pProcesses[i].processus;
    cout << "P" << i + 1 << ": " << process[3] << "ms \n";
  }
}

void writeFile(string output) {

  ofstream files;
  files.open(output);

  cout << "ENTER YOUR MESSAGE";

  files << "Average :"
        << "ms \n";

  files.close();
}

char *input = NULL;
char *output = NULL;

int main(int argc, char *argv[]) {
  int c;

  if (argc < 2) {
    cout << "You should introduce 2 arguments";
    exit(0);
  }

  while ((c = getopt(argc, argv, "f:o:")) != -1) {
    switch (c) {
    case 'f':
      input = optarg;
      break;
    case 'o':
      output = optarg;
      break;
    default:
      break;
    }
  }

  if (input == NULL || output == NULL) {
    cout << "use the parameter u for input and o for output";
    exit(0);
  }
  int nbreofline = 0;
  vector<linearqueue> tempArray{};

  std::ifstream file(input);
  std::string str;

  while (std::getline(file, str)) {
    vector<string> words{};
    string delimiter = ":";
    
    int pos;
    string text = str;

    while ((pos = text.find(delimiter)) != string::npos) {
      words.push_back(text.substr(0, pos));
      text.erase(0, pos + delimiter.length());
    }
    int p[6];
    int count = 0;
    for (const auto &str : words) {
      p[count] = stoi(str);
      count++;
    }
    p[2] = stoi(text);
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    linearqueue S1 = {{p[0], p[1], p[2], p[3], p[4], p[5]}};
    tempArray.push_back(S1);
    nbreofline++;
  }

  linearqueue processes[nbreofline];
  int j = 0;
  for (const auto &ar : tempArray) {
    processes[j] = ar;
    j++;
  }

  int userAction;
  int i = 0;
  bool finish = false;
  while (finish == false) {
    cout << "Choose Action : \n";
    cout << "1. Scheduling Preemptinve\n";
    cout << "2. Scheduling Non-Preemptinve\n";
    cout << "3. Show Result\n";
    cout << "4. End Program\n";
    cin >> userAction;

    switch (userAction) {
    case 1: {
      int preemptiveSchedulingType;
      cout << "Choose Between the five Scheduling Preemptinve Type \n";
      cout << "1. None\n";
      cout << "2. First Come, First Served Scheduling\n";
      cout << "3. Shortest-Job-First Scheduling\n";
      cout << "4. Priority Scheduling\n";
      cout << "5. Round-Robin Scheduling\n";
      cin >> preemptiveSchedulingType;
      switch (preemptiveSchedulingType) {
      case 1:
        cout << "None\n";
        break;
      case 2:
        cout << "First Come, First Served Scheduling\n";
        schedulingPreemptive(processes, 1, nbreofline);
        break;
      case 3:
        cout << "Shortest-Job-First Scheduling\n";
        schedulingPreemptive(processes, 0, nbreofline);
        break;
      case 4:
        cout << "Priority Scheduling\n";
        schedulingPreemptive(processes, 2, nbreofline);
        break;
      case 5:
        cout << "Round-Robin Scheduling\n";
        cout << "Insert Quantum\n";
        int quantum;
        cin >> quantum;
        schedulingRoundRobin(processes, quantum, nbreofline);
        break;
      default:
        cout << "you have to choose between 1 - 5";
        break;
      }

    } break;
    case 2: {
      int nonPreemptiveSchedulingType;
      cout << "Choose Between the five Scheduling Non Preemptinve Type \n";
      cout << "1. None\n";
      cout << "2. First Come, First Served Scheduling\n";
      cout << "3. Shortest-Job-First Scheduling\n";
      cout << "4. Priority Scheduling\n";
      cin >> nonPreemptiveSchedulingType;
      switch (nonPreemptiveSchedulingType) {
      case 1:
        cout << "None Scheduling method choosen try again \n";
        break;
      case 2:
        cout << "First Come, First Served Scheduling\n";
        schedulingNonPreemptive(processes, 1, nbreofline);

        break;
      case 3:
        cout << "Shortest-Job-First Scheduling\n";
        schedulingNonPreemptive(processes, 0, nbreofline);
        break;
      case 4:
        cout << "Priority Scheduling\n";
        schedulingNonPreemptive(processes, 2, nbreofline);
        break;
      default:
        cout << "you have to choose between 1 - 4";
        break;
      }

    } break;
    case 3: {
      cout << "The Result is:\n";
      cout << "Processes waiting Time \n";
      float average;
      average = getAverage(processes, nbreofline);
      showWaitingTime(processes, nbreofline);
      cout << "Average :" << average << "ms \n";
    }

    break;
    case 4: {
      cout << "Exit Program, Thank you  \n";
      float averages;
      averages = getAverage(processes, nbreofline);

      ofstream file;
      file.open(output);

      file << "Processes waiting Time \n";
      for (int i = 0; i < nbreofline; i++) {
        int *process = processes[i].processus;
        file << "P" << i + 1 << ": " << process[3] << "ms \n";
      }

      file << "Average :" << averages << "ms \n";
      file.close();
      finish = true;
    }

    break;
    default:
      cout << "you have to choose between 1 - 4";
      break;
    }
   
  }

  return 0;
}


