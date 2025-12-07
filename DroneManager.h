/*
 * Project: ESE224_Proj — Drone Depot
 * File: DroneManager.h
 * Purpose:
 *   Declares and implements a templated DroneManager class that manages
 *   collections of Drone-like objects using std::vector. It provides
 *   generic add/get plus drone-specific operations (sort/search/print)
 *   assuming the managed type T exposes the Drone interface:
 *     - getName() -> std::string
 *     - getID() -> int
 *     - getDistanceToOrigin() -> double
 */

#ifndef DRONEMANAGER_H
#define DRONEMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <utility>   // for std::swap
#include <random>
#include <chrono>
#include <ctime>

// T is expected to be a Drone-like type with:
//   std::string getName() const;
//   int getID() const;
//   double getDistanceToOrigin() const;

template <typename T>
class DroneManager {
private:
    std::vector<T*> objects;

public:
    // Adds an object pointer to the collection (no ownership implied).
    void addObject(T* obj) {
        if (obj != nullptr) {
            objects.push_back(obj);
        }
    }

    // Returns object pointer at index (no bounds check).
    T* getObject(int index) {
        return objects[index];
    }

    int getSize() const {
        return static_cast<int>(objects.size());
    }

    // Sorts drones by name (ascending); ties broken by ID.
    void sortDronesByName() {
        int n = static_cast<int>(objects.size());
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - 1 - i; ++j) {
                bool shouldSwap = false;
                if (objects[j]->getName() > objects[j + 1]->getName()) {
                    shouldSwap = true;
                } else if (objects[j]->getName() == objects[j + 1]->getName() &&
                           objects[j]->getID() > objects[j + 1]->getID()) {
                    shouldSwap = true;
                }
                if (shouldSwap) {
                    std::swap(objects[j], objects[j + 1]);
                }
            }
        }
    }

    // Sorts drones by ID (ascending).
    void sortDronesByID() {
        int n = static_cast<int>(objects.size());
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - 1 - i; ++j) {
                if (objects[j]->getID() > objects[j + 1]->getID()) {
                    std::swap(objects[j], objects[j + 1]);
                }
            }
        }
    }

    // Sorts drones by distance from origin (ascending).
    void sortDronesByPosition() {
        int n = static_cast<int>(objects.size());
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - 1 - i; ++j) {
                if (objects[j]->getDistanceToOrigin() > objects[j + 1]->getDistanceToOrigin()) {
                    std::swap(objects[j], objects[j + 1]);
                }
            }
        }
    }

    // Randomizes the order of drones (shuffle).
    void randomizeDroneOrder() {
        unsigned seed = static_cast<unsigned>(std::time(nullptr));
        std::mt19937 engine(seed);  // Mersenne Twister

        int n = static_cast<int>(objects.size());
        for (int i = n - 1; i > 0; --i) {
            std::uniform_int_distribution<int> dist(0, i);
            int j = dist(engine);
            std::swap(objects[i], objects[j]);
        }
    }

    // Binary search by name. Returns pointer or nullptr if not found.
    // Note: This sorts by name first to satisfy binary search precondition.
    T* searchDroneByName(const std::string& name) {
        if (objects.empty()) return nullptr;

        sortDronesByName();

        int left = 0;
        int right = static_cast<int>(objects.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            const std::string& midName = objects[mid]->getName();
            if (midName == name) {
                return objects[mid];
            } else if (midName < name) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return nullptr;
    }

    // Binary search by ID. Returns pointer or nullptr if not found.
    // Note: This sorts by ID first to satisfy binary search precondition.
    T* searchDroneByID(int id) {
        if (objects.empty()) return nullptr;

        sortDronesByID();

        int left = 0;
        int right = static_cast<int>(objects.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            int midID = objects[mid]->getID();
            if (midID == id) {
                return objects[mid];
            } else if (midID < id) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return nullptr;
    }

    // Prints all drone names to std::cout.
    void printAllDroneNames() {
        std::cout << "--- DroneManager Roster ---" << std::endl;
        for (int i = 0; i < static_cast<int>(objects.size()); ++i) {
            std::cout << i << ": " << objects[i]->getName() << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
    }
};

#endif
