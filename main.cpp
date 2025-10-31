/*
 * Project: ESE224_Proj — Drone Depot
 * Authors: Andy, Patrick, Kaicheng
 * File: main.cpp
 * Purpose:
 *   Console program to manage a small fleet of drones and their tasks:
 *   - Load drones from DroneInput.txt
 *   - Sort/search/view drones
 *   - Edit tasks (insert, copy, swap task data only)
 *   - Print Local (greedy) and Global (optimal) closed routes
 *
 * Notes:
 *   - Option 10 swaps ONLY task names and task positions between two drones.
 *   - Options 17 (Local/Greedy) and 18 (Global/Optimal) print a route for a chosen drone.
 *   - No console pauses; actions return directly to the menu.
 */

#include "Depot.h"
#include "Drone.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Loads up to 10 drones (each with 5 tasks) from the given text file.
// Expected format per drone:
//   name id initX initY
//   task1 x1 y1
//   task2 x2 y2
//   task3 x3 y3
//   task4 x4 y4
//   task5 x5 y5
void loadDronesFromFile(Depot& depot, const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }

    for (int i = 0; i < 10 && !infile.eof(); i++) {
        Drone d;
        string name;
        int id;
        int x, y;

    infile >> name;             // read drone name
    if (name.empty()) break;    // stop early if no more records
    infile >> id;               // read drone ID
    infile >> x >> y;           // read initial position (x,y)

    d.setName(name);            // set identity
    d.setID(id);
    d.setInitPosition(0, x);    // set initial X
    d.setInitPosition(1, y);    // set initial Y

        for (int j = 0; j < 5; j++) {
            string task;
            int tx, ty;
            infile >> task >> tx >> ty;   // read one task and its (x,y)
            d.setTask(j, task);
            d.setTaskPosition(j, 0, tx);
            d.setTaskPosition(j, 1, ty);
        }

    depot.addDrone(d);                 // append to depot
    }
    infile.close();
}

// Prints the interactive menu of available actions (options 1–18).
void displayMenu() {
    cout << "\n===== DRONE CONTROL MENU =====\n";
    cout << "1. Sort Drones By Name\n";
    cout << "2. Sort Drones By ID\n";
    cout << "3. Sort Drones By Position\n";
    cout << "4. Randomize Drone Order\n";
    cout << "5. Add a Drone\n";
    cout << "6. Retrieve a Drone\n";
    cout << "7. Search Drone By Name\n";
    cout << "8. Search Drone By ID\n";
    cout << "9. Write Depot to File\n";
    cout << "10. Swap Drone Data\n";
    cout << "11. Insert Drone Task\n";
    cout << "12. Copy-Paste Drone\n";
    cout << "13. Display All Drones names\n";
    cout << "14. SortDroneDataAscending\n";
    cout << "15. SortDroneDataDescending\n";
    cout << "16. Quit\n";
    cout << "17. Local Optimum Route (Greedy)\n";
    cout << "18. Global Optimum Route (Exact)\n";
    cout << "==============================\n";
    cout << "Select an option: ";
}

int main() {
    Depot depot;
    loadDronesFromFile(depot, "DroneInput.txt");
    cout << "Loaded " << depot.getNumDrones() << " drones from DroneInput.txt.\n";

    
    int choice;
    do {
    displayMenu();          // show menu
    cin >> choice;          // read user selection
        cout << endl;

        /*
        * Menu quick reference:
        *  1) Sort by Name   2) Sort by ID   3) Sort by Position   4) Randomize
        *  5) Add Drone      6) Show Drone   7) Search by Name     8) Search by ID
        *  9) Write File    10) Swap Tasks  11) Insert Task       12) Copy Drone
        * 13) List Names    14) Sort Tasks ↑ 15) Sort Tasks ↓     16) Quit
        * 17) Local Route (Greedy)          18) Global Route (Optimal)
        */
        
        switch (choice) {
        case 1:
            depot.sortByName();                 // sort fleet by name (asc)
            cout << "Sorted by name.\n";
            break;
        case 2:
            depot.sortByID();                   // sort fleet by ID (asc)
            cout << "Sorted by ID.\n";
            break;
        case 3:
            depot.sortByPosition();             // sort by distance from origin (asc)
            cout << "Sorted by distance from origin.\n";
            break;
        case 4:
            depot.randomizeOrder();             // shuffle fleet order
            cout << "Order randomized.\n";
            break;
        case 5: {
            Drone d;
            string name;
            int id, x, y;
            cout << "Enter drone name: ";
            cin >> name;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter initial position (x y): ";
            cin >> x >> y;
            d.setName(name);
            d.setID(id);
            d.setInitPosition(0, x);
            d.setInitPosition(1, y);
            depot.addDrone(d);                  // add new drone to fleet
            cout << "Drone added.\n";
            break;
        }
        case 6: {
            int idx;
            cout << "Enter drone index: ";
            cin >> idx;
            if (idx >= 0 && idx < depot.getNumDrones())
                depot.getDrone(idx).displayDrone();   // show all attributes
            else
                cout << "Invalid index.\n";
            break;
        }
        case 7: {
            string name;
            cout << "Enter name to search: ";
            cin >> name;
            int idx = depot.searchDroneByName(name);   // binary search by name
            if (idx != -1)
                depot.getDrone(idx).displayDrone();
            else
                cout << "Drone not found.\n";
            break;
        }
        case 8: {
            int id;
            cout << "Enter ID to search: ";
            cin >> id;
            int idx = depot.searchDroneByID(id);       // binary search by ID
            if (idx != -1)
                depot.getDrone(idx).displayDrone();
            else
                cout << "Drone not found.\n";
            break;
        }
        case 9:
            depot.writeDepotToFile();           // save snapshot to Depot.txt
            cout << "Depot written to file.\n";
            break;
        case 10: {
            int a, b;
            cout << "Enter two indices to swap tasks between: ";
            cin >> a >> b;
            if (a < 0 || b < 0 || a >= depot.getNumDrones() || b >= depot.getNumDrones()) { // bounds check
                cout << "Invalid indices.\n";
            } else if (a == b) {                // no-op if same index
                cout << "Indices are the same; nothing to swap.\n";
            } else {
                depot.swapDroneData(a, b);      // swap ONLY task names/positions
            }
            break;
        }
        case 11: {
            int droneIdx, taskIdx, tx, ty;
            string task;
            cout << "Enter drone index: ";
            cin >> droneIdx;
            cout << "Enter task index (0-4): ";
            cin >> taskIdx;
            cout << "Enter task name and position (x y): ";
            cin >> task >> tx >> ty;
            int pos[2] = { tx, ty };            // pack (x,y) into array
            depot.insertDroneTask(droneIdx, taskIdx, task, pos); // insert/overwrite task
            cout << "Task inserted.\n";
            break;
        }
        case 12: {
            int src, dest;
            cout << "Enter source and destination indices: ";
            cin >> src >> dest;
            depot.copyDrone(src, dest);         // deep copy src -> dest
            cout << "Drone copied.\n";
            break;
        }
        case 13:
            depot.printAllNames();              // list indices and names
            break;
        case 14: {
            int idx;
            cout << "Enter drone index: ";
            cin >> idx;
            depot.sortDroneDataAscending(idx);  // sort one drone's tasks A->Z
            cout << "Drone tasks sorted ascending.\n";
            break;
        }
        case 15: {
            int idx;
            cout << "Enter drone index: ";
            cin >> idx;
            depot.sortDroneDataDescending(idx); // sort one drone's tasks Z->A
            cout << "Drone tasks sorted descending.\n";
            break;
        }
        case 16:
            cout << "Exiting program.\n";
            break;
        case 17: {
            if (depot.getNumDrones() == 0) { cout << "No drones loaded.\n"; break; }
            int idx;
            cout << "Enter drone index for Local (Greedy) route: ";
            cin >> idx;
            if (idx >= 0 && idx < depot.getNumDrones()) {
                depot.computeGreedyRoute(idx, cout);  // nearest-next closed tour
            } else {
                cout << "Invalid index.\n";
            }
            break;
        }
        case 18: {
            if (depot.getNumDrones() == 0) { cout << "No drones loaded.\n"; break; }
            int idx;
            cout << "Enter drone index for Global Optimal route: ";
            cin >> idx;
            if (idx >= 0 && idx < depot.getNumDrones()) {
                depot.computeOptimalRoute(idx, cout); // brute-force closed tour
            } else {
                cout << "Invalid index.\n";
            }
            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 16);

    return 0;
}

