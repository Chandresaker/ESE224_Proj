/*
 * Project: ESE224_Proj — Drone Depot
 * Authors: Andy, Patrick, Kaicheng
 * File: AdvancedDrone.cpp
 * Purpose:
 *   Implements the AdvancedDrone class, which inherits from Drone and adds
 *   battery management, autopilot, and automatic task reordering based on
 *   battery level.
 */

#include "AdvancedDrone.h"
#include <utility> // for std::swap
#include <iostream>

using namespace std;

AdvancedDrone::AdvancedDrone() : Drone(), batteryLevel(100.0f), autopilotEnabled(false) {}

void AdvancedDrone::enableAutopilot() {
    autopilotEnabled = true;
}

void AdvancedDrone::updateBattery(float delta) {
    batteryLevel += delta;
    if (batteryLevel > 100.0f) batteryLevel = 100.0f;
    if (batteryLevel < 0.0f)  batteryLevel = 0.0f;
}

void AdvancedDrone::displayDrone() const {
    // Call base display for common info
    Drone::displayDrone();
    cout << "Battery: " << batteryLevel << "%";
    cout << ", Autopilot: " << (autopilotEnabled ? "ENABLED" : "DISABLED") << endl;
}

void AdvancedDrone::autoReorderTasks() {
    if (batteryLevel >= 30.0f) {
        return; // No change if battery is sufficient
    }

    // Reverse the order of tasks and their positions in-place
    int left = 0;
    int right = 4; // last index for 5 tasks
    while (left < right) {
        // Swap task names via getters/setters
        std::string leftTask  = getTask(left);
        std::string rightTask = getTask(right);
        setTask(left,  rightTask);
        setTask(right, leftTask);

        // Swap corresponding task positions (x and y)
        int leftX  = getTaskPosition(left,  0);
        int leftY  = getTaskPosition(left,  1);
        int rightX = getTaskPosition(right, 0);
        int rightY = getTaskPosition(right, 1);

        setTaskPosition(left,  0, rightX);
        setTaskPosition(left,  1, rightY);
        setTaskPosition(right, 0, leftX);
        setTaskPosition(right, 1, leftY);

        ++left;
        --right;
    }
}
