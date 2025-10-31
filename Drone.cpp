#include "Drone.h"
#include <iostream>
#include <string>
#include <cmath> // For sqrt and pow
#include <utility> // For std::swap

using namespace std;

// Default constructor implementation
Drone::Drone() {
    ID = 0;
    name = "";
    init_position[0] = 0;
    init_position[1] = 0;
    for (int i = 0; i < 5; ++i) {
        tasks[i] = "";
        task_positions[i][0] = 0;
        task_positions[i][1] = 0;
    }
}

// --- Getter Implementations ---

string Drone::getName() const {
    return name;
}

int Drone::getID() const {
    return ID;
}

string Drone::getTask(int index) const {
    // Check for valid index
    if (index >= 0 && index < 5) {
        return tasks[index];
    }
    return ""; // Return empty string on bad index
}

int Drone::getInitPosition(int index) const {
    // Check for valid index
    if (index == 0 || index == 1) {
        return init_position[index];
    }
    return 0; // Return 0 on bad index
}

int Drone::getTaskPosition(int taskIndex, int posIndex) const {
    // Check for valid indices
    if (taskIndex >= 0 && taskIndex < 5 && (posIndex == 0 || posIndex == 1)) {
        return task_positions[taskIndex][posIndex];
    }
    return 0; // Return 0 on bad index
}

// --- Setter Implementations ---

void Drone::setName(string name) {
    this->name = name;
}

void Drone::setID(int ID) {
    this->ID = ID;
}

void Drone::setTask(int index, string task) {
    // Check for valid index
    if (index >= 0 && index < 5) {
        this->tasks[index] = task;
    }
}

void Drone::setInitPosition(int index, int pos) {
    // Check for valid index
    if (index == 0 || index == 1) {
        this->init_position[index] = pos;
    }
}

void Drone::setTaskPosition(int taskIndex, int posIndex, int pos) {
    // Check for valid indices
    if (taskIndex >= 0 && taskIndex < 5 && (posIndex == 0 || posIndex == 1)) {
        this->task_positions[taskIndex][posIndex] = pos;
    }
}

// --- Sorting Implementations ---

// Manually sorts tasks in ascending order using Bubble Sort
void Drone::sortByAscending() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4 - i; ++j) {
            // Compare adjacent task names
            if (tasks[j] > tasks[j + 1]) {
                // Swap task names
                std::swap(tasks[j], tasks[j + 1]);

                // Swap corresponding task positions
                std::swap(task_positions[j][0], task_positions[j + 1][0]);
                std::swap(task_positions[j][1], task_positions[j + 1][1]);
            }
        }
    }
}

// Manually sorts tasks in descending order using Bubble Sort
void Drone::sortByDescending() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4 - i; ++j) {
            // Compare adjacent task names
            if (tasks[j] < tasks[j + 1]) {
                // Swap task names
                std::swap(tasks[j], tasks[j + 1]);

                // Swap corresponding task positions
                std::swap(task_positions[j][0], task_positions[j + 1][0]);
                std::swap(task_positions[j][1], task_positions[j + 1][1]);
            }
        }
    }
}

// --- Display Implementation ---

// Displays all attributes of the drone
void Drone::displayDrone() const {
    cout << "Drone: " << name << " (ID=" << ID << ", Position=(" << init_position[0] << ", " << init_position[1] << "))" << endl;
    cout << "Tasks:" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << i + 1 << ". " << tasks[i] << " at (" << task_positions[i][0] << "," << task_positions[i][1] << ")" << endl;
    }
}

// --- Operator Overload Implementations ---

// Copies all attributes from d2 into this object
void Drone::operator<<(const Drone& d2) {
    this->name = d2.name;
    this->ID = d2.ID;
    this->init_position[0] = d2.init_position[0];
    this->init_position[1] = d2.init_position[1];

    for (int i = 0; i < 5; ++i) {
        this->tasks[i] = d2.tasks[i];
        this->task_positions[i][0] = d2.task_positions[i][0];
        this->task_positions[i][1] = d2.task_positions[i][1];
    }
}

// Swaps tasks and task positions with d2
void Drone::operator-(Drone& d2) {
    for (int i = 0; i < 5; ++i) {
        std::swap(this->tasks[i], d2.tasks[i]);
        std::swap(this->task_positions[i], d2.task_positions[i]);
    }
}

// --- Helper Implementation ---

// Calculates Euclidean distance from origin (0,0)
double Drone::getDistanceToOrigin() const {
    double x = static_cast<double>(init_position[0]);
    double y = static_cast<double>(init_position[1]);
    return sqrt(pow(x, 2) + pow(y, 2));
}
