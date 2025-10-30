#include "Depot.h"
#include "Drone.h"
#include "Depot.cpp"
#include "Drone.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

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

        infile >> name;
        if (name.empty()) break;
        infile >> id;
        infile >> x >> y;

        d.setName(name);
        d.setID(id);
        d.setInitPosition(0, x);
        d.setInitPosition(1, y);

        for (int j = 0; j < 5; j++) {
            string task;
            int tx, ty;
            infile >> task >> tx >> ty;
            d.setTask(j, task);
            d.setTaskPosition(j, 0, tx);
            d.setTaskPosition(j, 1, ty);
        }

        depot.addDrone(d);
    }
    infile.close();
}

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
    cout << "==============================\n";
    cout << "Select an option: ";
}
void pause_console() {

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    Depot depot;
    loadDronesFromFile(depot, "DroneInput.txt");
    cout << "Loaded " << depot.getNumDrones() << " drones from DroneInput.txt.\n";

    
    int choice;
    do {
        displayMenu();
        cin >> choice;
        cout << endl;

        
        switch (choice) {
        case 1:
            depot.sortByName();
            cout << "Sorted by name.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        case 2:
            depot.sortByID();
            cout << "Sorted by ID.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        case 3:
            depot.sortByPosition();
            cout << "Sorted by distance from origin.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        case 4:
            depot.randomizeOrder();
            cout << "Order randomized.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
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
            depot.addDrone(d);
            cout << "Drone added.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 6: {
            int idx;
            cout << "Enter drone index: ";
            cin >> idx;
            if (idx >= 0 && idx < depot.getNumDrones())
                depot.getDrone(idx).displayDrone();
            else
                cout << "Invalid index.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 7: {
            string name;
            cout << "Enter name to search: ";
            cin >> name;
            int idx = depot.searchDroneByName(name);
            if (idx != -1)
                depot.getDrone(idx).displayDrone();
            else
                cout << "Drone not found.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 8: {
            int id;
            cout << "Enter ID to search: ";
            cin >> id;
            int idx = depot.searchDroneByID(id);
            if (idx != -1)
                depot.getDrone(idx).displayDrone();
            else
                cout << "Drone not found.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 9:
            depot.writeDepotToFile();
            cout << "Depot written to file.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        case 10: {
            int a, b;
            cout << "Enter two indices to swap: ";
            cin >> a >> b;
            depot.swapDroneData(a, b);
            cout << "Drone data swapped.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
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
            int pos[2] = { tx, ty };
            depot.insertDroneTask(droneIdx, taskIdx, task, pos);
            cout << "Task inserted.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 12: {
            int src, dest;
            cout << "Enter source and destination indices: ";
            cin >> src >> dest;
            depot.copyDrone(src, dest);
            cout << "Drone copied.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 13:
            depot.printAllNames();
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        case 14: {
            int idx;
            cout << "Enter drone index: ";
            cin >> idx;
            depot.sortDroneDataAscending(idx);
            cout << "Drone tasks sorted ascending.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 15: {
            int idx;
            cout << "Enter drone index: ";
            cin >> idx;
            depot.sortDroneDataDescending(idx);
            cout << "Drone tasks sorted descending.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
        case 16:
            cout << "Exiting program.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        default:
            cout << "Invalid choice.\n";
            cout<<"click any key to continue with menu"<<endl; pause_console(); break;
        }
    } while (choice != 16);

    return 0;
}

