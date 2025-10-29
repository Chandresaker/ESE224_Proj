#ifndef DRONE_H
#define DRONE_H

#include <string>
#include <iostream>

using namespace std;

// Drone class definition
class Drone {
private:
    // Private attributes for the drone
    string name;
    int ID;
    string tasks[5];
    int init_position[2];
    int task_positions[5][2];

public:
    // Default constructor
    Drone();

    // --- Getters ---
    string getName() const;
    int getID() const;
    string getTask(int index) const;
    int getInitPosition(int index) const;
    int getTaskPosition(int taskIndex, int posIndex) const;
    
    // --- Setters ---
    void setName(string name);
    void setID(int ID);
    void setTask(int index, string task);
    void setInitPosition(int index, int pos);
    void setTaskPosition(int taskIndex, int posIndex, int pos);

    // --- Sorting Methods ---
    // Manually sorts tasks in ascending order
    void sortByAscending();
    // Manually sorts tasks in descending order
    void sortByDescending();

    // --- Display Method ---
    // Displays all attributes of the drone
    void displayDrone() const;

    // --- Operator Overloads ---
    // Copies all attributes from d2 into this object
    void operator<<(const Drone& d2);
    // Swaps tasks and task positions with d2
    void operator-(Drone& d2);

    // --- Helper Method ---
    // Calculates distance from origin for sorting in Depot
    double getDistanceToOrigin() const;
};

#endif //DRONE_H
