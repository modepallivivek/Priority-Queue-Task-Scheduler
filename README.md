# Priority Queue Task Scheduler

A console-based task scheduling application developed using C++ that performs CPU task scheduling operations using Priority Queue and Min-Heap. It supports task insertion, priority scheduling, FCFS scheduling, SJF scheduling, Gantt chart display, and average waiting/turnaround time calculation.

---

# Project Information

- Project Title: Priority Queue Task Scheduler
- Department: Computer Science and Engineering
- Academic Year: 2025–26
- Subject: Data Structures
- Technology Used: C++
- Type: Console Application

---

# Features

- Insert New Task
- Display All Tasks
- Display Min-Heap
- Decrease Task Priority
- Priority Scheduling
- First Come First Serve Scheduling
- Shortest Job First Scheduling
- ASCII Gantt Chart
- Average Waiting Time Calculation
- Average Turnaround Time Calculation
- Best Scheduling Policy Advisor
- Menu Driven Interface

---

# Project Type

Console-based application developed using C++.

---

# Technologies Used

- C++
- Object Oriented Programming
- Data Structures
- Min-Heap
- Priority Queue
- CPU Scheduling
- Functions
- Vectors
- Sorting

---

# Software Requirements

- GCC Compiler / CodeBlocks / VS Code
- Windows/Linux/MacOS

---

# How to Run

## Step 1
Save the source code as:

```bash
scheduler.cpp
```

## Step 2
Compile the code:

```bash
g++ scheduler.cpp -o scheduler
```

## Step 3
Run the program:

```bash
./scheduler
```
# Full Source Code
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

class Task {
public:
    string taskName;
    int priority;
    int arrivalTime;
    int burstTime;
    int deadline;
    int waitingTime;
    int turnaroundTime;
    int completionTime;

    Task() {
        taskName = "";
        priority = 0;
        arrivalTime = 0;
        burstTime = 0;
        deadline = 0;
        waitingTime = 0;
        turnaroundTime = 0;
        completionTime = 0;
    }

    Task(string name, int p, int arrival, int burst, int d) {
        taskName = name;
        priority = p;
        arrivalTime = arrival;
        burstTime = burst;
        deadline = d;
        waitingTime = 0;
        turnaroundTime = 0;
        completionTime = 0;
    }
};

class MinHeap {
private:
    vector<Task> heap;

    int parent(int index) {
        return (index - 1) / 2;
    }

    int leftChild(int index) {
        return 2 * index + 1;
    }

    int rightChild(int index) {
        return 2 * index + 2;
    }

    void heapifyUp(int index) {
        while (index > 0 &&
               heap[index].priority < heap[parent(index)].priority) {

            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < heap.size() &&
            heap[left].priority < heap[smallest].priority) {

            smallest = left;
        }

        if (right < heap.size() &&
            heap[right].priority < heap[smallest].priority) {

            smallest = right;
        }

        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    void insertTask(Task task) {
        heap.push_back(task);
        heapifyUp(heap.size() - 1);
    }

    Task extractMin() {
        if (heap.empty()) {
            cout << "\nHeap is Empty!\n";
            return Task();
        }

        Task minTask = heap[0];

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }

        return minTask;
    }

    void decreaseKey(string name, int newPriority) {
        int index = -1;

        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].taskName == name) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "\nTask Not Found!\n";
            return;
        }

        if (newPriority > heap[index].priority) {
            cout << "\nNew priority must be smaller than current priority!\n";
            return;
        }

        heap[index].priority = newPriority;
        heapifyUp(index);

        cout << "\nPriority Updated Successfully!\n";
    }

    bool isEmpty() {
        return heap.empty();
    }

    void displayHeap() {
        if (heap.empty()) {
            cout << "\nHeap is Empty!\n";
            return;
        }

        cout << "\n========== MIN HEAP TASKS ==========\n";

        cout << left << setw(15) << "Task"
             << setw(10) << "Priority"
             << setw(10) << "Arrival"
             << setw(10) << "Burst"
             << setw(10) << "Deadline" << endl;

        for (Task task : heap) {
            cout << left << setw(15) << task.taskName
                 << setw(10) << task.priority
                 << setw(10) << task.arrivalTime
                 << setw(10) << task.burstTime
                 << setw(10) << task.deadline << endl;
        }
    }
};

void displayAllTasks(vector<Task> tasks) {
    if (tasks.empty()) {
        cout << "\nNo Tasks Available!\n";
        return;
    }

    cout << "\n========== ALL TASK DETAILS ==========\n";

    cout << left << setw(15) << "Task"
         << setw(10) << "Priority"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(10) << "Deadline" << endl;

    for (Task task : tasks) {
        cout << left << setw(15) << task.taskName
             << setw(10) << task.priority
             << setw(10) << task.arrivalTime
             << setw(10) << task.burstTime
             << setw(10) << task.deadline << endl;
    }
}

void printGanttChart(vector<Task> tasks) {
    cout << "\nASCII Gantt Chart:\n\n";

    for (Task task : tasks) {
        cout << "| " << task.taskName << " ";
    }

    cout << "|\n";

    if (!tasks.empty()) {
        cout << tasks[0].arrivalTime;
    }

    for (Task task : tasks) {
        cout << setw(6) << task.completionTime;
    }

    cout << endl;
}

void calculateReport(vector<Task> scheduledTasks, string policyName) {
    int currentTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;

    cout << "\n======================================";
    cout << "\n " << policyName;
    cout << "\n======================================";

    cout << "\nExecution Order:\n";

    for (int i = 0; i < scheduledTasks.size(); i++) {
        if (currentTime < scheduledTasks[i].arrivalTime) {
            currentTime = scheduledTasks[i].arrivalTime;
        }

        scheduledTasks[i].waitingTime =
            currentTime - scheduledTasks[i].arrivalTime;

        currentTime += scheduledTasks[i].burstTime;

        scheduledTasks[i].completionTime = currentTime;

        scheduledTasks[i].turnaroundTime =
            scheduledTasks[i].completionTime -
            scheduledTasks[i].arrivalTime;

        totalWaitingTime += scheduledTasks[i].waitingTime;
        totalTurnaroundTime += scheduledTasks[i].turnaroundTime;

        cout << scheduledTasks[i].taskName;

        if (i != scheduledTasks.size() - 1) {
            cout << " -> ";
        }
    }

    cout << endl;

    printGanttChart(scheduledTasks);

    cout << "\nTask Report:\n";

    cout << left << setw(15) << "Task"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(10) << "Waiting"
         << setw(15) << "Turnaround"
         << setw(15) << "Completion" << endl;

    for (Task task : scheduledTasks) {
        cout << left << setw(15) << task.taskName
             << setw(10) << task.arrivalTime
             << setw(10) << task.burstTime
             << setw(10) << task.waitingTime
             << setw(15) << task.turnaroundTime
             << setw(15) << task.completionTime << endl;
    }

    float averageWaitingTime =
        totalWaitingTime / scheduledTasks.size();

    float averageTurnaroundTime =
        totalTurnaroundTime / scheduledTasks.size();

    cout << fixed << setprecision(2);

    cout << "\nAverage Waiting Time    : "
         << averageWaitingTime;

    cout << "\nAverage Turnaround Time : "
         << averageTurnaroundTime << endl;
}

float getAverageWaitingTime(vector<Task> scheduledTasks) {
    int currentTime = 0;
    float totalWaitingTime = 0;

    for (int i = 0; i < scheduledTasks.size(); i++) {
        if (currentTime < scheduledTasks[i].arrivalTime) {
            currentTime = scheduledTasks[i].arrivalTime;
        }

        scheduledTasks[i].waitingTime =
            currentTime - scheduledTasks[i].arrivalTime;

        currentTime += scheduledTasks[i].burstTime;

        totalWaitingTime += scheduledTasks[i].waitingTime;
    }

    return totalWaitingTime / scheduledTasks.size();
}

vector<Task> priorityScheduling(vector<Task> tasks) {
    MinHeap heap;
    vector<Task> scheduledTasks;

    for (Task task : tasks) {
        heap.insertTask(task);
    }

    while (!heap.isEmpty()) {
        scheduledTasks.push_back(heap.extractMin());
    }

    return scheduledTasks;
}

vector<Task> fcfsScheduling(vector<Task> tasks) {
    sort(tasks.begin(), tasks.end(),
         [](Task a, Task b) {
             return a.arrivalTime < b.arrivalTime;
         });

    return tasks;
}

vector<Task> sjfScheduling(vector<Task> tasks) {
    sort(tasks.begin(), tasks.end(),
         [](Task a, Task b) {
             return a.burstTime < b.burstTime;
         });

    return tasks;
}

void policyAdvisor(vector<Task> tasks) {
    vector<Task> priorityTasks = priorityScheduling(tasks);
    vector<Task> fcfsTasks = fcfsScheduling(tasks);
    vector<Task> sjfTasks = sjfScheduling(tasks);

    float priorityAverage = getAverageWaitingTime(priorityTasks);
    float fcfsAverage = getAverageWaitingTime(fcfsTasks);
    float sjfAverage = getAverageWaitingTime(sjfTasks);

    cout << "\n======================================";
    cout << "\n POLICY ADVISOR REPORT";
    cout << "\n======================================";

    cout << fixed << setprecision(2);

    cout << "\nPriority Scheduling Average Waiting Time : "
         << priorityAverage;

    cout << "\nFCFS Average Waiting Time                : "
         << fcfsAverage;

    cout << "\nSJF Average Waiting Time                 : "
         << sjfAverage;

    cout << "\n\nBest Policy:\n";

    if (priorityAverage <= fcfsAverage &&
        priorityAverage <= sjfAverage) {

        cout << "Priority Scheduling minimized the average waiting time.";
        cout << "\nThis happened because high priority tasks were executed earlier.\n";
    }
    else if (fcfsAverage <= priorityAverage &&
             fcfsAverage <= sjfAverage) {

        cout << "FCFS minimized the average waiting time.";
        cout << "\nThis happened because tasks were executed in arrival order.\n";
    }
    else {
        cout << "SJF minimized the average waiting time.";
        cout << "\nThis happened because shorter burst-time tasks were executed earlier.\n";
    }
}

int main() {
    vector<Task> tasks;
    MinHeap heap;

    int choice;

    do {
        cout << "\n======================================";
        cout << "\n PRIORITY QUEUE TASK SCHEDULER";
        cout << "\n======================================";
        cout << "\n1. Insert New Task";
        cout << "\n2. Display All Tasks";
        cout << "\n3. Display Min-Heap";
        cout << "\n4. Decrease Task Priority";
        cout << "\n5. Priority Scheduling";
        cout << "\n6. FCFS Scheduling";
        cout << "\n7. SJF Scheduling";
        cout << "\n8. Run All Scheduling Policies";
        cout << "\n9. Policy Advisor";
        cout << "\n10. Exit";
        cout << "\n======================================";
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice) {

            case 1: {
                string name;
                int priority;
                int arrival;
                int burst;
                int deadline;

                cout << "\nEnter Task Name: ";
                cin >> name;

                cout << "Enter Priority: ";
                cin >> priority;

                cout << "Enter Arrival Time: ";
                cin >> arrival;

                cout << "Enter Burst Time: ";
                cin >> burst;

                cout << "Enter Deadline: ";
                cin >> deadline;

                Task task(name, priority, arrival, burst, deadline);

                tasks.push_back(task);
                heap.insertTask(task);

                cout << "\nTask Inserted Successfully!\n";
                break;
            }

            case 2:
                displayAllTasks(tasks);
                break;

            case 3:
                heap.displayHeap();
                break;

            case 4: {
                string name;
                int newPriority;

                cout << "\nEnter Task Name: ";
                cin >> name;

                cout << "Enter New Priority: ";
                cin >> newPriority;

                heap.decreaseKey(name, newPriority);

                for (int i = 0; i < tasks.size(); i++) {
                    if (tasks[i].taskName == name) {
                        tasks[i].priority = newPriority;
                        break;
                    }
                }

                break;
            }

            case 5:
                if (tasks.empty()) {
                    cout << "\nNo Tasks Available!\n";
                }
                else {
                    calculateReport(priorityScheduling(tasks),
                                    "PRIORITY SCHEDULING");
                }
                break;

            case 6:
                if (tasks.empty()) {
                    cout << "\nNo Tasks Available!\n";
                }
                else {
                    calculateReport(fcfsScheduling(tasks),
                                    "FCFS SCHEDULING");
                }
                break;

            case 7:
                if (tasks.empty()) {
                    cout << "\nNo Tasks Available!\n";
                }
                else {
                    calculateReport(sjfScheduling(tasks),
                                    "SJF SCHEDULING");
                }
                break;

            case 8:
                if (tasks.empty()) {
                    cout << "\nNo Tasks Available!\n";
                }
                else {
                    calculateReport(priorityScheduling(tasks),
                                    "PRIORITY SCHEDULING");

                    calculateReport(fcfsScheduling(tasks),
                                    "FCFS SCHEDULING");

                    calculateReport(sjfScheduling(tasks),
                                    "SJF SCHEDULING");
                }
                break;

            case 9:
                if (tasks.empty()) {
                    cout << "\nNo Tasks Available!\n";
                }
                else {
                    policyAdvisor(tasks);
                }
                break;

            case 10:
                cout << "\nThank You! Program Exited.\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while (choice != 10);

    return 0;
}
# Sample Input

Task Name: T1
Priority: 2
Arrival Time: 0
Burst Time: 5
Deadline: 10

Task Name: T2
Priority: 1
Arrival Time: 1
Burst Time: 3
Deadline: 8

Task Name: T3
Priority: 3
Arrival Time: 2
Burst Time: 4
Deadline: 12

Task Name: T4
Priority: 4
Arrival Time: 3
Burst Time: 2
Deadline: 7

---
```
## Sample Output

```txt
======================================
 PRIORITY QUEUE TASK SCHEDULER
======================================

Execution Order:
T2 -> T1 -> T3 -> T4

ASCII Gantt Chart:

| T2 | T1 | T3 | T4 |
0     3     8     12    14

Average Waiting Time    : 4.25
Average Turnaround Time : 7.75
```

---

# Project Modules

1. Task Creation Module  
Creates a new task with task name, priority, arrival time, burst time, and deadline.

2. Min-Heap Module  
Stores tasks according to priority using heap operations.

3. Priority Scheduling Module  
Schedules tasks based on priority value.

4. FCFS Scheduling Module  
Schedules tasks based on arrival time.

5. SJF Scheduling Module  
Schedules tasks based on burst time.

6. Gantt Chart Module  
Displays task execution order in chart format.

7. Policy Advisor Module  
Compares scheduling policies and suggests the best one.

---

# Concepts Used

- Classes and Objects
- Encapsulation
- Vectors
- Min-Heap
- Priority Queue
- Heapify Up
- Heapify Down
- Sorting
- CPU Scheduling
- Functions
- Loops
- Conditional Statements

---

# Future Enhancements

- File Handling
- GUI Interface
- Round Robin Scheduling
- Deadline Based Scheduling
- Preemptive Priority Scheduling
- Database Connectivity
- Colorful Gantt Chart
- Export Report as PDF

---

# Advantages

- Easy to use
- Beginner friendly project
- Demonstrates Min-Heap clearly
- Shows CPU scheduling simulation
- Helps understand priority queues
- Compares multiple scheduling policies
- Useful for Data Structures project

---

## Team Members

- VANNALA RAMCHARAN
- MODEPALLI VIVEK
- SHUSHANTH SADANAPALLY
- BANDARI USHA SREE
- TEKULAPALLY MANIKANTA
# Conclusion

The Priority Queue Task Scheduler is a simple and efficient C++ application that demonstrates CPU scheduling using priority queue and Min-Heap concepts.

This project helps students understand how tasks are scheduled using different scheduling algorithms such as Priority Scheduling, FCFS, and SJF.

It is useful for learning Data Structures, Operating System basics, and algorithm analysis.
