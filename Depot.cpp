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

    // Ensure dynamicStates stays in sync size-wise (default state for new drone)
    if (dynamicStates.size() < drones.size()) {
        dynamicStates.push_back(DynamicTaskState());
    }
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

// --- Dynamic Insertion Initialization ---

// Call once after loading initial drones to reset dynamic task state
void Depot::initializeDynamicInsertion() {
    dynamicStates.clear();
    dynamicStates.resize(drones.size());
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

// --- Dynamic Task Insertion (Bonus) ---

// Helper: compute additional distance if we insert a new task into drone d
// We approximate by choosing the best insertion position into the existing 5-task route.
// Returns the minimal extra distance (Delta) and does NOT modify the drone's base tasks.
static double computeInsertionDeltaForDrone(Drone& d, int newPos[2]) {
    // Build base route order [0..4] using current task order
    int order[5] = {0, 1, 2, 3, 4};

    int startPos[2] = {d.getInitPosition(0), d.getInitPosition(1)};

    // Compute original closed route length
    auto distanceBetween = [&](int p1[2], int p2[2]) {
        double x_diff = static_cast<double>(p1[0] - p2[0]);
        double y_diff = static_cast<double>(p1[1] - p2[1]);
        return sqrt(pow(x_diff, 2) + pow(y_diff, 2));
    };

    int curr[2] = {startPos[0], startPos[1]};
    double baseDist = 0.0;
    for (int i = 0; i < 5; ++i) {
        int nextPos[2] = {d.getTaskPosition(order[i], 0), d.getTaskPosition(order[i], 1)};
        baseDist += distanceBetween(curr, nextPos);
        curr[0] = nextPos[0];
        curr[1] = nextPos[1];
    }
    baseDist += distanceBetween(curr, startPos);

    // Try inserting new task at all positions along the closed tour
    double bestDelta = std::numeric_limits<double>::max();

    // We model the 6-segment closed tour (start -> 5 tasks -> start) and
    // insert the new point by splitting one segment.
    for (int i = 0; i < 6; ++i) {
        int A[2];
        int B[2];
        if (i == 0) {
            // segment: start -> first task
            A[0] = startPos[0];
            A[1] = startPos[1];
            B[0] = d.getTaskPosition(order[0], 0);
            B[1] = d.getTaskPosition(order[0], 1);
        } else if (i == 5) {
            // segment: last task -> start
            A[0] = d.getTaskPosition(order[4], 0);
            A[1] = d.getTaskPosition(order[4], 1);
            B[0] = startPos[0];
            B[1] = startPos[1];
        } else {
            // segment: task[i-1] -> task[i]
            A[0] = d.getTaskPosition(order[i - 1], 0);
            A[1] = d.getTaskPosition(order[i - 1], 1);
            B[0] = d.getTaskPosition(order[i], 0);
            B[1] = d.getTaskPosition(order[i], 1);
        }

        double oldSeg = distanceBetween(A, B);
        double newSeg = distanceBetween(A, newPos) + distanceBetween(newPos, B);
        double candidateDist = baseDist - oldSeg + newSeg;
        double delta = candidateDist - baseDist;

        if (delta < bestDelta) {
            bestDelta = delta;
        }
    }

    return bestDelta;
}

// Processes a newly arriving task across all drones.
// replacementThreshold: minimum improvement required to replace an existing dynamic task.
void Depot::processDynamicTask(const string& taskName, int x, int y,
                              double replacementThreshold,
                              double& totalFleetDistance) {
    if (drones.empty()) return;

    if (dynamicStates.size() != drones.size()) {
        dynamicStates.clear();
        dynamicStates.resize(drones.size());
    }

    int newPos[2] = {x, y};

    cout << "New task: " << taskName << " (x=" << x << ", y=" << y << ")" << endl;
    // Track fleet distance before processing this task to report net change
    double beforeTotal = totalFleetDistance;

    for (size_t i = 0; i < drones.size(); ++i) {
        Drone& d = drones[i];
        DynamicTaskState& state = dynamicStates[i];

        double deltaNew = computeInsertionDeltaForDrone(d, newPos);

        if (!state.hasTask) {
            // Simple insertion: this drone had no dynamic task yet.
            state.hasTask = true;
            state.name = taskName;
            state.pos[0] = x;
            state.pos[1] = y;
            state.delta = deltaNew;

            totalFleetDistance += deltaNew;

            cout << "Drone#" << i << ": Insert =" << deltaNew << endl;
        } else {
            // Decide whether to replace existing dynamic task
            double deltaOld = state.delta;
            if (deltaNew < deltaOld - replacementThreshold) {
                cout << "Drone#" << i << ": Replace old task " << state.name
                     << " (old=" << deltaOld
                     << " new=" << deltaNew << ")" << endl;

                // Update total fleet distance: remove old delta, add new
                totalFleetDistance -= deltaOld;
                totalFleetDistance += deltaNew;

                // Update to the newly chosen task
                state.name = taskName;
                state.pos[0] = x;
                state.pos[1] = y;
                state.delta = deltaNew;

                cout << "=> Drone#" << i << " now tracks task " << taskName << endl;
            } else {
                cout << "Drone#" << i << ": Keep existing task " << state.name
                     << " (old=" << deltaOld
                     << ", new=" << deltaNew << " not better)" << endl;
            }
        }
    }

    double deltaFleet = totalFleetDistance - beforeTotal;
    cout << "Total fleet distance: " << totalFleetDistance
            << " (" << (deltaFleet >= 0 ? "+" : "") << deltaFleet
            << " change)" << endl;

}

// --- Linked List / Queue / Stack Implementations ---

// Inserts drone into linked list ordered by ID (ascending)
void Depot::addDroneToLinkedList(Drone* drone) {
    if (drone == nullptr) return;

    // If list is empty or new drone has smallest ID, insert at head
    if (head == nullptr || drone->getID() < head->getID()) {
        drone->setNextDrone(head);
        head = drone;
        return;
    }

    // Find insertion point
    Drone* current = head;
    while (current->getNextDrone() != nullptr &&
           current->getNextDrone()->getID() < drone->getID()) {
        current = current->getNextDrone();
    }

    drone->setNextDrone(current->getNextDrone());
    current->setNextDrone(drone);
}

// Removes a drone with the given ID from the linked list
void Depot::removeDroneFromLinkedList(int id) {
    if (head == nullptr) return;

    // If head is the node to remove
    if (head->getID() == id) {
        Drone* temp = head;
        head = head->getNextDrone();
        temp->setNextDrone(nullptr);
        return;
    }

    Drone* current = head;
    while (current->getNextDrone() != nullptr &&
           current->getNextDrone()->getID() != id) {
        current = current->getNextDrone();
    }

    if (current->getNextDrone() != nullptr) {
        Drone* toRemove = current->getNextDrone();
        current->setNextDrone(toRemove->getNextDrone());
        toRemove->setNextDrone(nullptr);
    }
}

// Adds a drone to the end of the queue (FIFO) using vector
void Depot::enqueueDrone(Drone* drone) {
    if (drone == nullptr) return;
    dispatchQueue.push_back(drone);
}

// Removes and returns the front drone from the queue
Drone* Depot::dequeueDrone() {
    if (dispatchQueue.empty()) {
        return nullptr;
    }
    Drone* front = dispatchQueue.front();
    dispatchQueue.erase(dispatchQueue.begin());
    return front;
}

// Pushes a drone onto the top of the stack
void Depot::pushDrone(Drone* drone) {
    if (drone == nullptr) return;
    maintenanceStack.push_back(drone);
}

// Pops and returns the top drone from the stack
Drone* Depot::popDrone() {
    if (maintenanceStack.empty()) {
        return nullptr;
    }
    Drone* top = maintenanceStack.back();
    maintenanceStack.pop_back();
    return top;
}
