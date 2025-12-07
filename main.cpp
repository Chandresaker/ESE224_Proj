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
#include "AdvancedDrone.h"
#include "SpatialTree.h"
#include "DroneManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
    cout << "19. Test Advanced/Tree/Manager\n";
    cout << "20. Dynamic Insertion Demo\n";
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
        * 19) Test AdvancedDrone, SpatialTree, DroneManager
        * 20) Dynamic Insertion Demo
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
        case 19: {
            cout << "\n[Demo] AdvancedDrone, SpatialTree, DroneManager test" << endl;

            AdvancedDrone adv;
            adv.setName("ADV_TEST");
            adv.setID(999);
            adv.setInitPosition(0, 5);
            adv.setInitPosition(1, 5);
            for (int i = 0; i < 5; ++i) {
                string tname = string("T") + char('A' + i);
                adv.setTask(i, tname);
                adv.setTaskPosition(i, 0, i);
                adv.setTaskPosition(i, 1, i);
            }

            cout << "Initial AdvancedDrone state:" << endl;
            adv.displayDrone();

            adv.updateBattery(-80.0f); // drop battery below 30%
            adv.autoReorderTasks();

            cout << "After low battery autoReorderTasks():" << endl;
            adv.displayDrone();

            SpatialTree tree;
            // Insert all existing depot drones plus the AdvancedDrone
            for (int i = 0; i < depot.getNumDrones(); ++i) {
                tree.insert(&depot.getDrone(i));
            }
            tree.insert(&adv);

            cout << "\nSpatialTree in-order traversal:" << endl;
            tree.traverseInOrder();

            Drone* nearest = tree.search(0, 0);
            if (nearest) {
                cout << "Nearest to (0,0): " << nearest->getName() << " (ID="
                     << nearest->getID() << ")" << endl;
            }

            DroneManager<Drone> manager;
            for (int i = 0; i < depot.getNumDrones(); ++i) {
                manager.addObject(&depot.getDrone(i));
            }
            manager.addObject(&adv);

            cout << "\nDroneManager roster before shuffle:" << endl;
            manager.printAllDroneNames();

            manager.randomizeDroneOrder();

            cout << "DroneManager roster after shuffle:" << endl;
            manager.printAllDroneNames();

            break;
        }
        case 20: {
            if (depot.getNumDrones() == 0) {
                cout << "No drones loaded.\n";
                break;
            }

            cout << "\n[Demo] Dynamic task insertion across fleet" << endl;

            // Initialize dynamic insertion state
            depot.initializeDynamicInsertion();

            // Compute a simple base fleet distance as the sum of each drone's
            // optimal closed route. We reuse computeOptimalRoute but capture
            // only the distance by writing to a temporary stringstream.
            double totalFleetDistance = 0.0;
            for (int i = 0; i < depot.getNumDrones(); ++i) {
                ostringstream oss;
                depot.computeOptimalRoute(i, oss);

                string line;
                double dist = 0.0;
                istringstream iss(oss.str());
                while (getline(iss, line)) {
                    const string tag = "Total distance (optimal): ";
                    size_t pos = line.find(tag);
                    if (pos != string::npos) {
                        dist = stod(line.substr(pos + tag.size()));
                        break;
                    }
                }
                totalFleetDistance += dist;
            }

            cout << "Initial fleet optimal total distance: "
                 << totalFleetDistance << endl;

            double threshold = 0.0; // replacementThreshold

            // Example dynamic tasks (could be replaced with user input)
            depot.processDynamicTask("MedicalKit_23", 52, 48, threshold, totalFleetDistance);
            depot.processDynamicTask("FoodPack_42", 30, 60, threshold, totalFleetDistance);

            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 16);

    return 0;
}

