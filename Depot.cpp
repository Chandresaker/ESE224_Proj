/*
 * Project: ESE224_Proj — Drone Depot
 * Authors: Andy, Patrick, Kaicheng
 * File: Depot.cpp
 * Purpose:
 *   Implements the Depot class: manages a collection of Drone objects
 *   (add/retrieve/count), sorting and searching, swapping task data,
 *   randomization, printing, file output, and route computations
 *   (greedy/local and global optimal).
 *
 * Notes:
 *   - swapDroneData(i, j) swaps ONLY task names and task positions between drones.
 *   - writeDepotToFile() outputs drone info, tasks, and both route variants to Depot.txt.
 */

#include "Depot.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <utility> // For std::swap
#include <random>    // For random engine
#include <chrono>    // For time-based seed
#include <limits>    // For numeric_limits

using namespace std;

// Adds a drone to the vector
void Depot::addDrone(Drone& d1) {
    drones.push_back(d1);
}

// Retrieves a reference to a drone at a specific index
Drone& Depot::getDrone(int index) {
    // This assumes index is valid; main program should check
    return drones[index];
}

// Gets the total number of drones in the depot
int Depot::getNumDrones() const {
    return drones.size();
}

// --- Sorting Implementations (Manual Bubble Sort) ---

// Manually sorts the drone vector by name (ascending)
void Depot::sortByName() {
    int n = drones.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            bool shouldSwap = false;
            // Compare names
            if (drones[j].getName() > drones[j + 1].getName()) {
                shouldSwap = true;
            } 
            // If names tie, break tie by ID
            else if (drones[j].getName() == drones[j + 1].getName()) {
                if (drones[j].getID() > drones[j + 1].getID()) {
                    shouldSwap = true;
                }
            }
            
            if (shouldSwap) {
                std::swap(drones[j], drones[j + 1]);
            }
        }
    }
}

// Manually sorts the drone vector by ID (ascending)
void Depot::sortByID() {
    int n = drones.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            // Compare IDs
            if (drones[j].getID() > drones[j + 1].getID()) {
                std::swap(drones[j], drones[j + 1]);
            }
        }
    }
}

// Manually sorts the drone vector by distance from origin (ascending)
void Depot::sortByPosition() {
    int n = drones.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            // Compare distances
            if (drones[j].getDistanceToOrigin() > drones[j + 1].getDistanceToOrigin()) {
                std::swap(drones[j], drones[j + 1]);
            }
        }
    }
}

// --- Searching Implementations (Binary Search) ---

// Binary searches for a drone by name
int Depot::searchDroneByName(string& name) {
    // Per instructions, sort first
    sortByName();

    int left = 0;
    int right = drones.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (drones[mid].getName() == name) {
            return mid; // Found
        } else if (drones[mid].getName() < name) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Not found
}

// Binary searches for a drone by ID
int Depot::searchDroneByID(int id) {
    // Per instructions, sort first
    sortByID();

    int left = 0;
    int right = drones.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (drones[mid].getID() == id) {
            return mid; // Found
        } else if (drones[mid].getID() < id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Not found
}

// --- Drone Interaction Method Implementations ---

// Swaps task data (tasks + task positions) between two drones
void Depot::swapDroneData(int index1, int index2) {
    int n = static_cast<int>(drones.size());
    if (index1 < 0 || index1 >= n || index2 < 0 || index2 >= n) {
        cout << "Error: Invalid indices for swap." << endl;
        return;
    }
    if (index1 == index2) {
        cout << "Swap skipped: indices are the same." << endl;
        return;
    }
    // Use the overloaded operator- to swap only tasks and their positions
    Drone t = drones[index1];
    drones[index1] - drones[index2];
    cout << "Swapped task data between drones " << index1 << " and " << index2 << "." << endl;
}

// Copies drone data from index1 to index2
void Depot::copyDrone(int index1, int index2) {
    // Use the overloaded operator<<
    drones[index2] << drones[index1];
}

// Inserts a new task into a specific drone's task list
void Depot::insertDroneTask(int droneidx, int taskidx, string& task, int task_pos[2]) {
    drones[droneidx].setTask(taskidx, task);
    drones[droneidx].setTaskPosition(taskidx, 0, task_pos[0]);
    drones[droneidx].setTaskPosition(taskidx, 1, task_pos[1]);
}

// Sorts a specific drone's task list ascending
void Depot::sortDroneDataAscending(int idx) {
    drones[idx].sortByAscending();
}

// Sorts a specific drone's task list descending
void Depot::sortDroneDataDescending(int idx) {
    drones[idx].sortByDescending();
}

// --- Other Method Implementations ---

// Manually shuffles the order of drones (Fisher-Yates shuffle)
void Depot::randomizeOrder() {
    // Get a time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    int n = drones.size();
    for (int i = n - 1; i > 0; --i) {
        // Generate a random index j such that 0 <= j <= i
        std::uniform_int_distribution<int> d(0, i);
        int j = d(e);
        // Swap drones[i] with drones[j]
        std::swap(drones[i], drones[j]);
    }
}

// Prints the names of all drones
void Depot::printAllNames() {
    cout << "--- Drone Roster ---" << endl;
    for (size_t i = 0; i < drones.size(); ++i) {
        cout << i << ": " << drones[i].getName() << endl;
    }
    cout << "--------------------" << endl;
}

// Writes the entire depot state to "Depot.txt"
void Depot::writeDepotToFile() {
    ofstream outFile("Depot.txt");
    if (!outFile.is_open()) {
        cout << "Error: Could not open Depot.txt for writing." << endl;
        return;
    }

    // Set fixed-point notation for distances
    outFile << fixed;
    outFile.precision(2);

    for (size_t i = 0; i < drones.size(); ++i) {
        // Write standard drone info
        outFile << "Drone: " << drones[i].getName() << " (ID=" << drones[i].getID() 
                << ", Position=(" << drones[i].getInitPosition(0) << ", " 
                << drones[i].getInitPosition(1) << "))" << endl;
        outFile << "Tasks:" << endl;
        for (int j = 0; j < 5; ++j) {
            outFile << j + 1 << ". " << drones[i].getTask(j) << " at (" 
                    << drones[i].getTaskPosition(j, 0) << "," 
                    << drones[i].getTaskPosition(j, 1) << ")" << endl;
        }

        // --- Bonus: Write Route Info ---
        computeGreedyRoute(i, outFile);
        computeOptimalRoute(i, outFile);

        // Add blank line for readability
        outFile << endl;
    }

    outFile.close();
}

// --- Bonus Method Implementations ---

// Calculates distance between two 2D points
double Depot::getTaskDistance(int pos1[2], int pos2[2]) {
    double x_diff = static_cast<double>(pos1[0] - pos2[0]);
    double y_diff = static_cast<double>(pos1[1] - pos2[1]);
    return sqrt(pow(x_diff, 2) + pow(y_diff, 2));
}

// Computes and prints the greedy route for a drone
void Depot::computeGreedyRoute(int droneIdx, ostream& out) {
    Drone& d = drones[droneIdx];
    int startPos[2] = {d.getInitPosition(0), d.getInitPosition(1)};
    int currentPos[2];
    currentPos[0] = startPos[0];
    currentPos[1] = startPos[1];

    bool visited[5] = {false, false, false, false, false};
    int route[5];
    double totalDistance = 0.0;

    for (int i = 0; i < 5; ++i) {
        double minDist = std::numeric_limits<double>::max();
        int bestTaskIdx = -1;

        // Find the closest unvisited task
        for (int j = 0; j < 5; ++j) {
            if (!visited[j]) {
                int taskPos[2] = {d.getTaskPosition(j, 0), d.getTaskPosition(j, 1)};
                double dist = getTaskDistance(currentPos, taskPos);
                if (dist < minDist) {
                    minDist = dist;
                    bestTaskIdx = j;
                }
            }
        }
        
        // Visit the closest task
        totalDistance += minDist;
        route[i] = bestTaskIdx;
        visited[bestTaskIdx] = true;
        currentPos[0] = d.getTaskPosition(bestTaskIdx, 0);
        currentPos[1] = d.getTaskPosition(bestTaskIdx, 1);
    }

    // Add distance to return to start
    totalDistance += getTaskDistance(currentPos, startPos);

    // Print Greedy Route
    out << "[Greedy Route - closed]" << endl;
    out << "Sequence (starting at (" << startPos[0] << "," << startPos[1] << ")):" << endl;
    out << "(" << startPos[0] << "," << startPos[1] << ")";
    for (int i = 0; i < 5; ++i) {
        int taskIdx = route[i];
        out << " -> (" << d.getTaskPosition(taskIdx, 0) << "," << d.getTaskPosition(taskIdx, 1) << ")";
    }
    out << " -> (" << startPos[0] << "," << startPos[1] << ")" << endl;
    out << "Total distance (greedy): " << totalDistance << endl;
}

// Recursive helper for finding the optimal (brute-force) route
void Depot::findOptimalPermutation(int droneIdx, int taskIndices[], int k, double& minDistance, int bestPath[]) {
    Drone& d = drones[droneIdx];
    int startPos[2] = {d.getInitPosition(0), d.getInitPosition(1)};

    // Base case: a full permutation is complete
    if (k == 5) {
        double currentDistance = 0.0;
        int lastPos[2];

        // Distance from start to first task
        int firstTaskPos[2] = {d.getTaskPosition(taskIndices[0], 0), d.getTaskPosition(taskIndices[0], 1)};
        currentDistance += getTaskDistance(startPos, firstTaskPos);
        lastPos[0] = firstTaskPos[0];
        lastPos[1] = firstTaskPos[1];

        // Sum distances between tasks
        for (int i = 1; i < 5; ++i) {
            int currentTaskPos[2] = {d.getTaskPosition(taskIndices[i], 0), d.getTaskPosition(taskIndices[i], 1)};
            currentDistance += getTaskDistance(lastPos, currentTaskPos);
            lastPos[0] = currentTaskPos[0];
            lastPos[1] = currentTaskPos[1];
        }

        // Add distance from last task back to start
        currentDistance += getTaskDistance(lastPos, startPos);

        // Check if this is the new best route
        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            for (int i = 0; i < 5; ++i) {
                bestPath[i] = taskIndices[i];
            }
        }
        return;
    }

    // Recursive step: generate permutations
    for (int i = k; i < 5; ++i) {
        std::swap(taskIndices[i], taskIndices[k]);
        findOptimalPermutation(droneIdx, taskIndices, k + 1, minDistance, bestPath);
        std::swap(taskIndices[i], taskIndices[k]); // Backtrack
    }
}


// Computes and prints the global optimal route for a drone
void Depot::computeOptimalRoute(int droneIdx, ostream& out) {
    Drone& d = drones[droneIdx];
    int startPos[2] = {d.getInitPosition(0), d.getInitPosition(1)};
    
    int taskIndices[5] = {0, 1, 2, 3, 4};
    int bestPath[5];
    double minDistance = std::numeric_limits<double>::max();

    // Find the best permutation
    findOptimalPermutation(droneIdx, taskIndices, 0, minDistance, bestPath);

    // Print Optimal Route
    out << "[Global Optimal Route - closed]" << endl;
    out << "Sequence (starting at (" << startPos[0] << "," << startPos[1] << ")):" << endl;
    out << "(" << startPos[0] << "," << startPos[1] << ")";
    for (int i = 0; i < 5; ++i) {
        int taskIdx = bestPath[i];
        out << " -> (" << d.getTaskPosition(taskIdx, 0) << "," << d.getTaskPosition(taskIdx, 1) << ")";
    }
    out << " -> (" << startPos[0] << "," << startPos[1] << ")" << endl;
    out << "Total distance (optimal): " << minDistance << endl;
}
