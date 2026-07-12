#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
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

        if (left < (int)heap.size() &&
            heap[left].priority < heap[smallest].priority)
            smallest = left;

        if (right < (int)heap.size() &&
            heap[right].priority < heap[smallest].priority)
            smallest = right;

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
        if (heap.empty())
            return Task();

        Task minTask = heap[0];

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty())
            heapifyDown(0);

        return minTask;
    }

    bool decreaseKey(string name, int newPriority) {
        int index = -1;

        for (int i = 0; i < (int)heap.size(); i++) {
            if (heap[i].taskName == name) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "\nTask Not Found!\n";
            return false;
        }

        if (newPriority > heap[index].priority) {
            cout << "\nNew priority must be smaller than current priority!\n";
            return false;
        }

        heap[index].priority = newPriority;
        heapifyUp(index);

        cout << "\nPriority Updated Successfully!\n";
        return true;
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

vector<Task> priorityScheduling(vector<Task> tasks) {
    vector<Task> scheduledTasks;
    vector<bool> completed(tasks.size(), false);

    int currentTime = 0;
    int completedCount = 0;

    while (completedCount < (int)tasks.size()) {
        int selected = -1;
        int bestPriority = INT_MAX;

        for (int i = 0; i < (int)tasks.size(); i++) {
            if (!completed[i] &&
                tasks[i].arrivalTime <= currentTime &&
                tasks[i].priority < bestPriority) {

                bestPriority = tasks[i].priority;
                selected = i;
            }
        }

        if (selected == -1) {
            currentTime++;
            continue;
        }

        scheduledTasks.push_back(tasks[selected]);
        currentTime += tasks[selected].burstTime;
        completed[selected] = true;
        completedCount++;
    }

    return scheduledTasks;
}

vector<Task> fcfsScheduling(vector<Task> tasks) {
    stable_sort(tasks.begin(), tasks.end(),
        [](Task a, Task b) {
            return a.arrivalTime < b.arrivalTime;
        });

    return tasks;
}

vector<Task> sjfScheduling(vector<Task> tasks) {
    vector<Task> scheduledTasks;
    vector<bool> completed(tasks.size(), false);

    int currentTime = 0;
    int completedCount = 0;

    while (completedCount < (int)tasks.size()) {
        int selected = -1;
        int shortestBurst = INT_MAX;

        for (int i = 0; i < (int)tasks.size(); i++) {
            if (!completed[i] &&
                tasks[i].arrivalTime <= currentTime &&
                tasks[i].burstTime < shortestBurst) {

                shortestBurst = tasks[i].burstTime;
                selected = i;
            }
        }

        if (selected == -1) {
            currentTime++;
            continue;
        }

        scheduledTasks.push_back(tasks[selected]);
        currentTime += tasks[selected].burstTime;
        completed[selected] = true;
        completedCount++;
    }

    return scheduledTasks;
}

void printGanttChart(vector<Task> tasks) {
    cout << "\nASCII Gantt Chart:\n\n";

    for (Task task : tasks)
        cout << "| " << task.taskName << " ";

    cout << "|\n";

    if (!tasks.empty())
        cout << tasks[0].arrivalTime;

    for (Task task : tasks)
        cout << setw(6) << task.completionTime;

    cout << endl;
}

void calculateReport(vector<Task> scheduledTasks, string policyName) {
    if (scheduledTasks.empty()) {
        cout << "\nNo Tasks Available!\n";
        return;
    }

    int currentTime = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;

    cout << "\n======================================";
    cout << "\n " << policyName;
    cout << "\n======================================";

    cout << "\nExecution Order:\n";

    for (int i = 0; i < (int)scheduledTasks.size(); i++) {
        if (currentTime < scheduledTasks[i].arrivalTime)
            currentTime = scheduledTasks[i].arrivalTime;

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

        if (i != (int)scheduledTasks.size() - 1)
            cout << " -> ";
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

    cout << fixed << setprecision(2);

    cout << "\nAverage Waiting Time    : "
         << totalWaitingTime / scheduledTasks.size();

    cout << "\nAverage Turnaround Time : "
         << totalTurnaroundTime / scheduledTasks.size()
         << endl;
}

float getAverageWaitingTime(vector<Task> tasks) {
    if (tasks.empty())
        return 0;

    int currentTime = 0;
    float totalWaitingTime = 0;

    for (Task task : tasks) {
        if (currentTime < task.arrivalTime)
            currentTime = task.arrivalTime;

        totalWaitingTime +=
            currentTime - task.arrivalTime;

        currentTime += task.burstTime;
    }

    return totalWaitingTime / tasks.size();
}

void policyAdvisor(vector<Task> tasks) {
    vector<Task> priorityTasks = priorityScheduling(tasks);
    vector<Task> fcfsTasks = fcfsScheduling(tasks);
    vector<Task> sjfTasks = sjfScheduling(tasks);

    float priorityAverage =
        getAverageWaitingTime(priorityTasks);

    float fcfsAverage =
        getAverageWaitingTime(fcfsTasks);

    float sjfAverage =
        getAverageWaitingTime(sjfTasks);

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

        cout << "Priority Scheduling minimized the average waiting time.\n";
    }
    else if (fcfsAverage <= priorityAverage &&
             fcfsAverage <= sjfAverage) {

        cout << "FCFS minimized the average waiting time.\n";
    }
    else {
        cout << "SJF minimized the average waiting time.\n";
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
            int priority, arrival, burst, deadline;

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

            Task task(name, priority, arrival,
                      burst, deadline);

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

            if (heap.decreaseKey(name, newPriority)) {
                for (Task &task : tasks) {
                    if (task.taskName == name) {
                        task.priority = newPriority;
                        break;
                    }
                }
            }

            break;
        }

        case 5:
            calculateReport(
                priorityScheduling(tasks),
                "PRIORITY SCHEDULING");
            break;

        case 6:
            calculateReport(
                fcfsScheduling(tasks),
                "FCFS SCHEDULING");
            break;

        case 7:
            calculateReport(
                sjfScheduling(tasks),
                "SJF SCHEDULING");
            break;

        case 8:
            calculateReport(
                priorityScheduling(tasks),
                "PRIORITY SCHEDULING");

            calculateReport(
                fcfsScheduling(tasks),
                "FCFS SCHEDULING");

            calculateReport(
                sjfScheduling(tasks),
                "SJF SCHEDULING");
            break;

        case 9:
            if (tasks.empty())
                cout << "\nNo Tasks Available!\n";
            else
                policyAdvisor(tasks);
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
