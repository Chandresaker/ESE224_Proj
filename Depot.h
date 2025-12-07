/*
 * Project: ESE224_Proj — Drone Depot
 * File: Depot.h
 * Purpose:
 *   Declares the Depot class: manages a collection of Drone objects, including
 *   sorting/search, swapping task data between drones, task edits, shuffling,
 *   printing, persistence to Depot.txt, and route computations (greedy/global).
 */

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

    // --- Pointer-based linked list (registration by ID) ---
    // Head of linked list of drones ordered by ID
    Drone* head = nullptr;

    // --- Vector-based queue and stack (no std::queue/stack) ---
    // Dispatch queue (FIFO)
    vector<Drone*> dispatchQueue;
    // Maintenance stack (LIFO)
    vector<Drone*> maintenanceStack;

    // --- Bonus Helper Methods ---
    // Calculates distance between two 2D points
    double getTaskDistance(int pos1[2], int pos2[2]);
    // Recursive helper for finding the optimal (brute-force) route
    void findOptimalPermutation(int droneIdx, int taskIndices[], int k, double& minDistance, int bestPath[]);

    // --- Dynamic Insertion State ---
    struct DynamicTaskState {
        bool   hasTask = false;   // whether this drone currently has a pending inserted task
        string name;              // name of the inserted task
        int    pos[2] = {0, 0};   // (x,y) of inserted task
        double delta = 0.0;       // additional distance contributed by this task
    };

    // For each drone in 'drones', track its current dynamic task state
    vector<DynamicTaskState> dynamicStates;

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

    // --- Dynamic Task Insertion (Bonus) ---
    // Initializes internal dynamic insertion tracking (must be called after loading drones).
    void initializeDynamicInsertion();

    // Processes a newly arriving task (name, x, y). For each drone, evaluates inserting or
    // replacing a pending dynamic task, updates state and total distance, and prints decisions.
    // 'totalFleetDistance' should be maintained by the caller across calls.
    void processDynamicTask(const string& taskName, int x, int y,
                            double replacementThreshold,
                            double& totalFleetDistance);

    // --- Linked List / Queue / Stack Management ---
    // Adds a drone to the linked list ordered by ID
    void addDroneToLinkedList(Drone* drone);
    // Removes a drone by ID from the linked list
    void removeDroneFromLinkedList(int id);
    // Adds a drone to the dispatch queue (FIFO)
    void enqueueDrone(Drone* drone);
    // Removes and returns the front drone from the queue (or nullptr)
    Drone* dequeueDrone();
    // Pushes a drone onto the maintenance stack (LIFO)
    void pushDrone(Drone* drone);
    // Pops and returns the top drone from the stack (or nullptr)
    Drone* popDrone();

    // --- Diagnostics for List/Queue/Stack ---
    // Prints linked list IDs in order
    void printLinkedList() const;
    // Prints dispatch queue from front to back
    void printDispatchQueue() const;
    // Prints maintenance stack from top to bottom
    void printMaintenanceStack() const;
};

#endif //DEPOT_H
