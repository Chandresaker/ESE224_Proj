#ifndef DEPOT_H
#define DEPOT_H

#include <vector>
#include <string>
#include "Drone.h"

using namespace std;

// Depot class definition
class Depot {
private:
    // Private attribute for the fleet
    vector<Drone> drones;

    // --- Bonus Helper Methods ---
    // Calculates distance between two 2D points
    double getTaskDistance(int pos1[2], int pos2[2]);
    // Recursive helper for finding the optimal (brute-force) route
    void findOptimalPermutation(int droneIdx, int taskIndices[], int k, double& minDistance, int bestPath[]);

public:
    // --- Core Methods ---
    // Adds a drone to the vector
    void addDrone(Drone& d1);
    // Retrieves a reference to a drone at a specific index
    Drone& getDrone(int index);
    // Gets the total number of drones in the depot
    int getNumDrones() const;

    // --- Sorting Methods ---
    // Manually sorts the drone vector by name
    void sortByName();
    // Manually sorts the drone vector by ID
    void sortByID();
    // Manually sorts the drone vector by distance from origin
    void sortByPosition();

    // --- Searching Methods ---
    // Binary searches for a drone by name (must sort first)
    int searchDroneByName(string& name);
    // Binary searches for a drone by ID (must sort first)
    int searchDroneByID(int id);

    // --- Drone Interaction Methods ---
    // Swaps task data between two drones
    void swapDroneData(int index1, int index2);
    // Copies drone data from index1 to index2
    void copyDrone(int index1, int index2);
    // Inserts a new task into a specific drone's task list
    void insertDroneTask(int droneidx, int taskidx, string& task, int task_pos[2]);

    // --- Drone Data Sorting ---
    // Sorts a specific drone's task list ascending
    void sortDroneDataAscending(int idx);
    // Sorts a specific drone's task list descending
    void sortDroneDataDescending(int idx);
    
    // --- Other Methods ---
    // Manually shuffles the order of drones in the vector
    void randomizeOrder();
    // Prints the names of all drones
    void printAllNames();
    // Writes the entire depot state to "Depot.txt"
    void writeDepotToFile();

    // --- Bonus Route Optimization Methods ---
    // Computes and prints the greedy route for a drone
    void computeGreedyRoute(int droneIdx, ostream& out);
    // Computes and prints the global optimal route for a drone
    void computeOptimalRoute(int droneIdx, ostream& out);
};

#endif //DEPOT_H
