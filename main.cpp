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

// Prints the interactive menu of available actions (options 1–25).
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
    cout << "19. Linked List register/remove\n";
    cout << "20. Dispatch Queue\n";
    cout << "21. Maintenance Stack\n";
    cout << "22. Spatial Tree build/search\n";
    cout << "23. DroneManager Access\n";
    cout << "24. AdvancedDrone polymorphism\n";
    cout << "25. Dynamic Task Insertion\n";
    cout << "==============================\n";
    cout << "Select an option: ";
}
// ============================================
// DIAGNOSTIC TEST SUITE  |  Author: Kaicheng
// ============================================
void runDiagnostics();

void runDynamicInsertionTest() {

    // Create temporary objects just for testing so we don't mess up the main depot
    Depot testDepot;
    SpatialTree testTree;
    DroneManager<Drone> testManager;

    // --- 1. Linked List Edge Cases ---
    cout << "[Test] Linked List Removal:\n";
    // Create dummy drones
    Drone d1; d1.setID(9991); d1.setName("Test1");
    Drone d2; d2.setID(9992); d2.setName("Test2");
    Drone d3; d3.setID(9993); d3.setName("Test3");

    // We pass pointers to the linked list
    testDepot.addDroneToLinkedList(&d1);
    testDepot.addDroneToLinkedList(&d2);
    testDepot.addDroneToLinkedList(&d3);
    
    // Case: Remove Middle
    testDepot.removeDroneFromLinkedList(9992); 
    // Case: Remove Head (Critical Check)
    testDepot.removeDroneFromLinkedList(9991);
    // Case: Remove Non-Existent
    testDepot.removeDroneFromLinkedList(8888); 
    
    cout << ">> Verify visual output (if implemented) or check logic flow.\n";
    cout << "PASS: Removal operations completed without crashing.\n";

    // --- 2. Queue/Stack Underflow ---
    cout << "\n[Test] Queue/Stack Empty Operations:\n";
    Drone* dEmpty = testDepot.dequeueDrone();
    if (dEmpty == nullptr) cout << "PASS: Dequeue from empty returned nullptr.\n";
    else cout << "FAIL: Dequeue from empty returned a pointer.\n";

    dEmpty = testDepot.popDrone();
    if (dEmpty == nullptr) cout << "PASS: Pop from empty returned nullptr.\n";
    else cout << "FAIL: Pop from empty returned a pointer.\n";

    // --- 3. Spatial Tree Limits ---
    cout << "\n[Test] Spatial Search Edge Cases:\n";
    testTree.insert(&d3); // Insert one drone
    Drone* found = testTree.search(-100, -100); // Search far outside bounds
    if (found == nullptr) 
        cout << "PASS: Search returned nullptr (or valid nearest neighbor).\n";
    else 
        cout << "Result: Found closest drone to (-100, -100): " << found->getName() << endl;

    // --- 4. Template Manager ---
    cout << "\n[Test] DroneManager Template:\n";
    testManager.addObject(&d3);
    Drone* searchRes = testManager.searchDroneByID(9993);
    if (searchRes != nullptr && searchRes->getID() == 9993) cout << "PASS: Template Search Found ID 9993.\n";
    else cout << "FAIL: Template Search failed.\n";

    cout << "=== DIAGNOSTICS COMPLETE ===\n\n";
}

// Automated dynamic insertion test: demonstrates one insertion and one replacement
// without user input. Uses a local Depot so main program state is unchanged.
void runDynamicInsertionTest2() {
    cout << "\n=== Dynamic Insertion Test (auto) ===\n";

    Depot testDepot;

    // Build two simple drones with linear tasks for deterministic distances
    Drone d1;
    d1.setName("Alpha");
    d1.setID(1);
    d1.setInitPosition(0, 0);
    d1.setInitPosition(1, 0);
    for (int i = 0; i < 5; ++i) {
        d1.setTask(i, "A" + to_string(i));
        d1.setTaskPosition(i, 0, (i + 1) * 10);
        d1.setTaskPosition(i, 1, 0);
    }

    Drone d2;
    d2.setName("Bravo");
    d2.setID(2);
    d2.setInitPosition(0, 0);
    d2.setInitPosition(1, 0);
    for (int i = 0; i < 5; ++i) {
        d2.setTask(i, "B" + to_string(i));
        d2.setTaskPosition(i, 0, (i + 1) * 8);
        d2.setTaskPosition(i, 1, 2);
    }

    testDepot.addDrone(d1);
    testDepot.addDrone(d2);
    testDepot.initializeDynamicInsertion();

    // Compute initial fleet optimal total distance
    double totalFleetDistance = 0.0;
    for (int i = 0; i < testDepot.getNumDrones(); ++i) {
        ostringstream oss;
        testDepot.computeOptimalRoute(i, oss);

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

    cout << "Initial fleet optimal total distance: " << totalFleetDistance << "\n";

    double threshold = 0.0; // any improvement triggers replacement

    // First dynamic task: far away, will be a costly insertion
    testDepot.processDynamicTask("UrgentFar", 80, 80, threshold, totalFleetDistance);

    // Second dynamic task: much closer, should trigger a replacement on the same drone
    testDepot.processDynamicTask("CloserBetter", 5, 0, threshold, totalFleetDistance);

    // Third dynamic task: modest cost, may go to the other drone for insertion
    testDepot.processDynamicTask("MidRange", 25, 5, threshold, totalFleetDistance);

    cout << "Final fleet distance after auto dynamic insertions: "
         << totalFleetDistance << "\n";
    cout << "=== End Dynamic Insertion Test ===\n\n";
}
// ==========================================
// END DIAGNOSTIC TEST SUITE
// ==========================================
int main() {
    // 1. RUN TESTS FIRST
    //runDiagnostics(); commented out to avoid interrupting normal flow / debugging done

    // 2. Proceed with normal program execution
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
        * 19) Linked List register/remove   20) Dispatch Queue
        * 21) Maintenance Stack             22) Spatial Tree build/search
        * 23) DroneManager                 24) AdvancedDrone polymorphism
        * 25) Dynamic Task Insertion
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
            cout << "[Linked List Operations - modifies depot]" << endl;
            cout << "Current linked list state:" << endl;
            depot.printLinkedList();

            cout << "\n1. Add NEW drone to depot and linked list" << endl;
            cout << "2. Register existing drones to linked list" << endl;
            cout << "3. Remove drone from linked list by ID" << endl;
            cout << "Select operation (0 to skip): ";
            int listOp;
            cin >> listOp;

            if (listOp == 1) {
                // Create and add a new drone to depot
                Drone newDrone;
                string name;
                int id, x, y;
                cout << "Enter drone name: ";
                cin >> name;
                cout << "Enter drone ID: ";
                cin >> id;
                cout << "Enter initial position (x y): ";
                cin >> x >> y;
                newDrone.setName(name);
                newDrone.setID(id);
                newDrone.setInitPosition(0, x);
                newDrone.setInitPosition(1, y);
                // Add to depot
                depot.addDrone(newDrone);
                cout << "Drone added to depot. Total drones: " << depot.getNumDrones() << endl;
                // Also add to linked list
                depot.addDroneToLinkedList(&depot.getDrone(depot.getNumDrones() - 1));
                cout << "Drone registered in linked list." << endl;
            } else if (listOp == 2) {
                cout << "Adding all depot drones to linked list by ID..." << endl;
                for (int i = 0; i < depot.getNumDrones(); ++i) {
                    depot.addDroneToLinkedList(&depot.getDrone(i));
                }
            } else if (listOp == 3) {
                cout << "Enter ID to remove from linked list: ";
                int removeId;
                cin >> removeId;
                depot.removeDroneFromLinkedList(removeId);
                cout << "Removed from linked list (drone still in depot)." << endl;
            }

            cout << "\nFinal linked list state:" << endl;
            depot.printLinkedList();
            break;
        }
        case 20: {
            cout << "[Dispatch Queue Operations - modifies depot]" << endl;
            cout << "Current queue state:" << endl;
            depot.printDispatchQueue();

            cout << "\n1. Add NEW drone to depot and enqueue" << endl;
            cout << "2. Enqueue existing drone by index" << endl;
            cout << "3. Dequeue front drone" << endl;
            cout << "Select operation (0 to skip): ";
            int queueOp;
            cin >> queueOp;

            if (queueOp == 1) {
                // Create and add a new drone to depot
                Drone newDrone;
                string name;
                int id, x, y;
                cout << "Enter drone name: ";
                cin >> name;
                cout << "Enter drone ID: ";
                cin >> id;
                cout << "Enter initial position (x y): ";
                cin >> x >> y;
                newDrone.setName(name);
                newDrone.setID(id);
                newDrone.setInitPosition(0, x);
                newDrone.setInitPosition(1, y);
                // Add to depot
                depot.addDrone(newDrone);
                cout << "Drone added to depot. Total drones: " << depot.getNumDrones() << endl;
                // Also enqueue
                depot.enqueueDrone(&depot.getDrone(depot.getNumDrones() - 1));
                cout << "Drone enqueued for dispatch." << endl;
            } else if (queueOp == 2) {
                if (depot.getNumDrones() == 0) {
                    cout << "No drones in depot to enqueue." << endl;
                } else {
                    cout << "Enter drone index to enqueue (0-" << depot.getNumDrones()-1 << "): ";
                    int enqIdx;
                    cin >> enqIdx;
                    if (enqIdx >= 0 && enqIdx < depot.getNumDrones()) {
                        depot.enqueueDrone(&depot.getDrone(enqIdx));
                        cout << "Enqueued: " << depot.getDrone(enqIdx).getName() << endl;
                    } else {
                        cout << "Invalid index." << endl;
                    }
                }
            } else if (queueOp == 3) {
                Drone* dequeued = depot.dequeueDrone();
                if (dequeued) {
                    cout << "Dequeued: " << dequeued->getName() << " (ID=" << dequeued->getID() << ")" << endl;
                } else {
                    cout << "Queue was empty." << endl;
                }
            }

            cout << "\nFinal queue state:" << endl;
            depot.printDispatchQueue();
            break;
        }
        case 21: {
            cout << "[Maintenance Stack Operations - modifies depot]" << endl;
            cout << "Current stack state:" << endl;
            depot.printMaintenanceStack();

            cout << "\n1. Add NEW drone to depot and push to stack" << endl;
            cout << "2. Push existing drone by index" << endl;
            cout << "3. Pop top drone from stack" << endl;
            cout << "Select operation (0 to skip): ";
            int stackOp;
            cin >> stackOp;

            if (stackOp == 1) {
                // Create and add a new drone to depot
                Drone newDrone;
                string name;
                int id, x, y;
                cout << "Enter drone name: ";
                cin >> name;
                cout << "Enter drone ID: ";
                cin >> id;
                cout << "Enter initial position (x y): ";
                cin >> x >> y;
                newDrone.setName(name);
                newDrone.setID(id);
                newDrone.setInitPosition(0, x);
                newDrone.setInitPosition(1, y);
                // Add to depot
                depot.addDrone(newDrone);
                cout << "Drone added to depot. Total drones: " << depot.getNumDrones() << endl;
                // Also push to stack
                depot.pushDrone(&depot.getDrone(depot.getNumDrones() - 1));
                cout << "Drone pushed to maintenance stack." << endl;
            } else if (stackOp == 2) {
                if (depot.getNumDrones() == 0) {
                    cout << "No drones in depot to push." << endl;
                } else {
                    cout << "Enter drone index to push (0-" << depot.getNumDrones()-1 << "): ";
                    int pushIdx;
                    cin >> pushIdx;
                    if (pushIdx >= 0 && pushIdx < depot.getNumDrones()) {
                        depot.pushDrone(&depot.getDrone(pushIdx));
                        cout << "Pushed: " << depot.getDrone(pushIdx).getName() << endl;
                    } else {
                        cout << "Invalid index." << endl;
                    }
                }
            } else if (stackOp == 3) {
                Drone* popped = depot.popDrone();
                if (popped) {
                    cout << "Popped: " << popped->getName() << " (ID=" << popped->getID() << ")" << endl;
                } else {
                    cout << "Stack was empty." << endl;
                }
            }

            cout << "\nFinal stack state:" << endl;
            depot.printMaintenanceStack();
            break;
        }
        case 22: {
            if (depot.getNumDrones() == 0) { cout << "No drones loaded.\n"; break; }

            cout << "[Spatial Tree - all traversals and search]" << endl;
            SpatialTree tree;
            for (int i = 0; i < depot.getNumDrones(); ++i) {
                tree.insert(&depot.getDrone(i));
            }
            cout << "Inserted " << depot.getNumDrones() << " drones into spatial tree." << endl;

            // Allow adding new drones by coordinate (ordered by x then y via tree comparator)
            char addChoice;
            cout << "Add a NEW drone by coordinates into the spatial tree? (y/n): ";
            cin >> addChoice;
            while (addChoice == 'y' || addChoice == 'Y') {
                Drone newDrone;
                string name;
                int id, x, y;
                cout << "Enter drone name: ";
                cin >> name;
                cout << "Enter drone ID: ";
                cin >> id;
                cout << "Enter initial position (x y): ";
                cin >> x >> y;
                newDrone.setName(name);
                newDrone.setID(id);
                newDrone.setInitPosition(0, x);
                newDrone.setInitPosition(1, y);
                depot.addDrone(newDrone); // persist in depot for lifetime stability
                tree.insert(&depot.getDrone(depot.getNumDrones() - 1));
                cout << "Inserted new drone into depot and spatial tree at (" << x << "," << y << ")" << endl;
                cout << "Add another? (y/n): ";
                cin >> addChoice;
            }

            cout << "\n--- In-Order Traversal ---" << endl;
            tree.traverseInOrder();

            cout << "\n--- Pre-Order Traversal ---" << endl;
            tree.traversePreOrder();

            cout << "\n--- Post-Order Traversal ---" << endl;
            tree.traversePostOrder();

            int sx, sy;
            cout << "\nEnter (x y) to search nearest: ";
            cin >> sx >> sy;
            Drone* nearest = tree.search(sx, sy);
            if (nearest) {
                cout << "Nearest to (" << sx << "," << sy << "): "
                     << nearest->getName() << " (ID=" << nearest->getID() 
                     << ") at (" << nearest->getInitPosition(0) << "," 
                     << nearest->getInitPosition(1) << ")" << endl;
            } else {
                cout << "Tree was empty." << endl;
            }
            break;
        }
        case 23: {
            cout << "[DroneManager Template Operations]" << endl;
            
            // Build manager from current depot drones
            DroneManager<Drone> manager;
            for (int i = 0; i < depot.getNumDrones(); ++i) {
                manager.addObject(&depot.getDrone(i));
            }
            cout << "Manager loaded with " << manager.getSize() << " drones from depot." << endl;

            cout << "\n1. Add drone to manager (addObject)" << endl;
            cout << "2. Sort by Name (sortDronesByName)" << endl;
            cout << "3. Sort by ID (sortDronesByID)" << endl;
            cout << "4. Sort by Position (sortDronesByPosition)" << endl;
            cout << "5. Randomize order (randomizeDroneOrder)" << endl;
            cout << "6. Get drone by index (getObject)" << endl;
            cout << "7. Search by ID (searchDroneByID)" << endl;
            cout << "8. Search by Name (searchDroneByName)" << endl;
            cout << "9. Print roster (printAllDroneNames)" << endl;
            cout << "10. Get size (getSize)" << endl;
            cout << "Select operation (0 to skip): ";

            cout << "\n\nInitial manager roster:" << endl;
            manager.printAllDroneNames();

            int mgrOp;
            cin >> mgrOp;

            if (mgrOp == 1) {
                // Create a new drone, add to depot (so it persists), then register depot copy in manager
                Drone newDrone;
                string name;
                int id, x, y;
                cout << "Enter drone name: ";
                cin >> name;
                cout << "Enter drone ID: ";
                cin >> id;
                cout << "Enter initial position (x y): ";
                cin >> x >> y;
                newDrone.setName(name);
                newDrone.setID(id);
                newDrone.setInitPosition(0, x);
                newDrone.setInitPosition(1, y);
                depot.addDrone(newDrone); // persist
                manager.addObject(&depot.getDrone(depot.getNumDrones() - 1));
                cout << "Drone added to depot and manager. Manager size: " << manager.getSize() << endl;
                manager.printAllDroneNames();
            } else if (mgrOp == 2) {
                manager.sortDronesByName();
                cout << "Manager sorted by name:" << endl;
                manager.printAllDroneNames();
            } else if (mgrOp == 3) {
                manager.sortDronesByID();
                cout << "Manager sorted by ID:" << endl;
                manager.printAllDroneNames();
            } else if (mgrOp == 4) {
                manager.sortDronesByPosition();
                cout << "Manager sorted by position (distance from origin):" << endl;
                manager.printAllDroneNames();
            } else if (mgrOp == 5) {
                manager.randomizeDroneOrder();
                cout << "Manager order randomized:" << endl;
                manager.printAllDroneNames();
            } else if (mgrOp == 6) {
                if (manager.getSize() == 0) {
                    cout << "Manager is empty." << endl;
                } else {
                    cout << "Enter manager index (0-" << manager.getSize()-1 << "): ";
                    int idx;
                    cin >> idx;
                    if (idx >= 0 && idx < manager.getSize()) {
                        Drone* d = manager.getObject(idx);
                        if (d) {
                            cout << "Drone at manager index " << idx << ":" << endl;
                            d->displayDrone();
                        }
                    } else {
                        cout << "Invalid index." << endl;
                    }
                }
            } else if (mgrOp == 7) {
                cout << "Enter ID to search: ";
                int searchId;
                cin >> searchId;
                Drone* found = manager.searchDroneByID(searchId);
                if (found) {
                    cout << "Found by ID:" << endl;
                    found->displayDrone();
                } else {
                    cout << "Not found by ID." << endl;
                }
            } else if (mgrOp == 8) {
                cout << "Enter name to search: ";
                string searchName;
                cin >> searchName;
                Drone* found = manager.searchDroneByName(searchName);
                if (found) {
                    cout << "Found by name:" << endl;
                    found->displayDrone();
                } else {
                    cout << "Not found by name." << endl;
                }
            } else if (mgrOp == 9) {
                manager.printAllDroneNames();
            } else if (mgrOp == 10) {
                cout << "Manager size: " << manager.getSize() << " drones" << endl;
            }

            // Persist manager order/content back into depot
            Depot rebuilt;
            for (int i = 0; i < manager.getSize(); ++i) {
                Drone* ptr = manager.getObject(i);
                if (ptr) {
                    rebuilt.addDrone(*ptr); // copy pointed drone into rebuilt depot
                }
            }
            depot = rebuilt;
            cout << "Depot reordered to match manager (size: " << depot.getNumDrones() << ")." << endl;

            break;
        }
        case 24: {
            cout << "[AdvancedDrone Polymorphism Demo]" << endl;

            // Create AdvancedDrone with user input
            AdvancedDrone adv;
            string advName;
            int advId, advX, advY;
            cout << "Enter AdvancedDrone name: ";
            cin >> advName;
            cout << "Enter AdvancedDrone ID: ";
            cin >> advId;
            cout << "Enter initial position (x y): ";
            cin >> advX >> advY;

            adv.setName(advName);
            adv.setID(advId);
            adv.setInitPosition(0, advX);
            adv.setInitPosition(1, advY);

            // Set up tasks
            for (int i = 0; i < 5; ++i) {
                string tname = string("Task") + to_string(i + 1);
                adv.setTask(i, tname);
                adv.setTaskPosition(i, 0, advX + i * 2);
                adv.setTaskPosition(i, 1, advY + i * 2);
            }

            cout << "\n--- Initial AdvancedDrone state ---" << endl;
            adv.displayDrone();

            cout << "\nEnable autopilot? (1=yes, 0=no): ";
            int enableAuto;
            cin >> enableAuto;
            if (enableAuto == 1) {
                adv.enableAutopilot();
                cout << "Autopilot enabled." << endl;
            }

            cout << "\nEnter battery change (e.g., -80 to drain): ";
            float batteryDelta;
            cin >> batteryDelta;
            adv.updateBattery(batteryDelta);

            cout << "\n--- After battery update ---" << endl;
            adv.displayDrone();

            cout << "\nTrigger autoReorderTasks (reverses if battery < 30%)..." << endl;
            adv.autoReorderTasks();
            cout << "--- After autoReorderTasks ---" << endl;
            adv.displayDrone();

            // Demonstrate polymorphism via base pointer
            Drone* basePtr = &adv;
            cout << "\n--- Via base Drone pointer (virtual dispatch) ---" << endl;
            basePtr->displayDrone();

            // Add to depot so it persists
            cout << "\nAdd this AdvancedDrone to depot? (1=yes, 0=no): ";
            int addToDepot;
            cin >> addToDepot;
            if (addToDepot == 1) {
                depot.addDrone(adv);  // slicing copies base Drone attributes
                cout << "AdvancedDrone added to depot (as Drone). Total drones: " 
                     << depot.getNumDrones() << endl;
            }
            break;
        }
        case 25: {
            if (depot.getNumDrones() == 0) { cout << "No drones loaded.\n"; break; }

            cout << "[Dynamic Task Insertion - modifies depot state]" << endl;

            depot.initializeDynamicInsertion();

            // Compute a base fleet distance by summing optimal routes
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

            cout << "\nEnter replacement threshold (0 for any improvement): ";
            double threshold;
            cin >> threshold;

            char addMore = 'y';
            while (addMore == 'y' || addMore == 'Y') {
                string taskName;
                int taskX, taskY;
                cout << "\nEnter new task name: ";
                cin >> taskName;
                cout << "Enter task position (x y): ";
                cin >> taskX >> taskY;

                depot.processDynamicTask(taskName, taskX, taskY, threshold, totalFleetDistance);

                cout << "\nAdd another dynamic task? (y/n): ";
                cin >> addMore;
            }

            cout << "\nFinal fleet distance after dynamic insertions: " 
                 << totalFleetDistance << endl;
            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 16);

    return 0;
}

