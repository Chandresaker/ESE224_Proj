/*
 * Project: ESE224_Proj — Drone Depot
 * File: AdvancedDrone.h
 * Purpose:
 *   Declares the AdvancedDrone class, which extends Drone with battery
 *   management and autopilot features, demonstrating polymorphism.
 */

#ifndef ADVANCEDDRONE_H
#define ADVANCEDDRONE_H

#include "Drone.h"

// AdvancedDrone extends Drone with battery and autopilot features
class AdvancedDrone : public Drone {
private:
    float batteryLevel;      // Battery percentage [0, 100]
    bool autopilotEnabled;   // Autopilot status

public:
    AdvancedDrone();

    void enableAutopilot();
    void updateBattery(float delta);

    // Override to include battery/autopilot info
    void displayDrone() const override;

    // If battery < 30%, reverse the order of tasks
    void autoReorderTasks();
};

#endif 
