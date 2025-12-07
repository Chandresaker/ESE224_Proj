/*
 * Project: ESE224_Proj — Drone Depot
 * File: SpatialTree.h
 * Purpose:
 *   Declares SpatialNode and SpatialTree classes used to organize Drone
 *   objects spatially (by initial x/y position) in a binary search tree.
 *   Supports insertion, nearest-neighbor style search, and tree traversals
 *   for diagnostics.
 */

#ifndef SPATIALTREE_H
#define SPATIALTREE_H

#include "Drone.h"
#include <iostream>

// Node in the spatial binary search tree
class SpatialNode {
public:
    Drone* drone;       // Pointer to the Drone stored at this node
    SpatialNode* left;  // Left child
    SpatialNode* right; // Right child

    explicit SpatialNode(Drone* d) : drone(d), left(nullptr), right(nullptr) {}
};

// Binary search tree over drones, ordered by initial position
class SpatialTree {
private:
    SpatialNode* root;

    // Helper for recursive insertion
    SpatialNode* insertRecursive(SpatialNode* node, Drone* drone);

    // Helper for nearest-neighbor style search
    void searchNearestRecursive(SpatialNode* node, int x, int y,
                                Drone*& bestDrone, double& bestDistSq) const;

    // Traversal helpers
    void traverseInOrderRecursive(SpatialNode* node) const;
    void traversePreOrderRecursive(SpatialNode* node) const;
    void traversePostOrderRecursive(SpatialNode* node) const;

    // Helper to delete all nodes
    void clearRecursive(SpatialNode* node);

public:
    SpatialTree();
    ~SpatialTree();

    // Inserts a drone into the tree based on its initial x/y position.
    void insert(Drone* drone);

    // Searches for the drone whose initial position is closest
    // (Euclidean distance) to (x, y). Returns nullptr if tree is empty.
    Drone* search(int x, int y) const;

    // Traversal printouts (for diagnostics). Each prints drone ID and position.
    void traverseInOrder() const;
    void traversePreOrder() const;
    void traversePostOrder() const;
};

#endif // SPATIALTREE_H
