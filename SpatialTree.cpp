/*
 * Project: ESE224_Proj — Drone Depot
 * Authors: Andy, Kaicheng, Patrick
 * File: SpatialTree.cpp
 * Purpose:
 *   Implements the SpatialTree binary search tree over Drone objects,
 *   ordered by their initial (x, y) position. Provides insertion,
 *   nearest-neighbor-like search, and traversal printouts.
 */

#include "SpatialTree.h"
#include <cmath>   // For pow, sqrt (only pow used for squared distance)
#include <limits>  // For numeric_limits
#include <iostream>

using namespace std;

SpatialTree::SpatialTree() : root(nullptr) {}

SpatialTree::~SpatialTree() {
    clearRecursive(root);
}

void SpatialTree::clearRecursive(SpatialNode* node) {
    if (node == nullptr) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}

// Helper to compare two drones' positions with (x, y) and decide tree direction.
// Primary key: x-coordinate; secondary key: y-coordinate.
static bool isLessThan(Drone* a, Drone* b) {
    int ax = a->getInitPosition(0);
    int ay = a->getInitPosition(1);
    int bx = b->getInitPosition(0);
    int by = b->getInitPosition(1);

    if (ax < bx) return true;
    if (ax > bx) return false;
    // If x is equal, compare y
    return ay < by;
}

SpatialNode* SpatialTree::insertRecursive(SpatialNode* node, Drone* drone) {
    if (node == nullptr) {
        return new SpatialNode(drone);
    }

    if (isLessThan(drone, node->drone)) {
        node->left = insertRecursive(node->left, drone);
    } else {
        // Equal or greater goes to the right
        node->right = insertRecursive(node->right, drone);
    }
    return node;
}

void SpatialTree::insert(Drone* drone) {
    if (drone == nullptr) return;
    root = insertRecursive(root, drone);
}

// Squared Euclidean distance between drone's initial position and (x, y)
static double squaredDistanceTo(const Drone* d, int x, int y) {
    int dx = d->getInitPosition(0) - x;
    int dy = d->getInitPosition(1) - y;
    return static_cast<double>(dx * dx + dy * dy);
}

void SpatialTree::searchNearestRecursive(SpatialNode* node, int x, int y,
                                         Drone*& bestDrone, double& bestDistSq) const {
    if (node == nullptr) return;

    // Compute distance from this node's drone
    double distSq = squaredDistanceTo(node->drone, x, y);
    if (distSq < bestDistSq) {
        bestDistSq = distSq;
        bestDrone = node->drone;
    }

    // Decide which side to explore first based on x-coordinate
    int nodeX = node->drone->getInitPosition(0);

    if (x < nodeX) {
        // Target is to the left
        searchNearestRecursive(node->left, x, y, bestDrone, bestDistSq);
        // Optionally explore right if it might contain closer points.
        searchNearestRecursive(node->right, x, y, bestDrone, bestDistSq);
    } else {
        // Target is to the right or equal
        searchNearestRecursive(node->right, x, y, bestDrone, bestDistSq);
        // Optionally explore left as well.
        searchNearestRecursive(node->left, x, y, bestDrone, bestDistSq);
    }
}

Drone* SpatialTree::search(int x, int y) const {
    if (root == nullptr) return nullptr;

    Drone* bestDrone = nullptr;
    double bestDistSq = numeric_limits<double>::max();
    searchNearestRecursive(root, x, y, bestDrone, bestDistSq);
    return bestDrone;
}

// --- Traversal helpers ---

void SpatialTree::traverseInOrderRecursive(SpatialNode* node) const {
    if (node == nullptr) return;
    traverseInOrderRecursive(node->left);
    cout << "Drone ID=" << node->drone->getID()
         << " Pos=(" << node->drone->getInitPosition(0)
         << "," << node->drone->getInitPosition(1) << ")" << endl;
    traverseInOrderRecursive(node->right);
}

void SpatialTree::traversePreOrderRecursive(SpatialNode* node) const {
    if (node == nullptr) return;
    cout << "Drone ID=" << node->drone->getID()
         << " Pos=(" << node->drone->getInitPosition(0)
         << "," << node->drone->getInitPosition(1) << ")" << endl;
    traversePreOrderRecursive(node->left);
    traversePreOrderRecursive(node->right);
}

void SpatialTree::traversePostOrderRecursive(SpatialNode* node) const {
    if (node == nullptr) return;
    traversePostOrderRecursive(node->left);
    traversePostOrderRecursive(node->right);
    cout << "Drone ID=" << node->drone->getID()
         << " Pos=(" << node->drone->getInitPosition(0)
         << "," << node->drone->getInitPosition(1) << ")" << endl;
}

// --- Public traversal wrappers ---

void SpatialTree::traverseInOrder() const {
    traverseInOrderRecursive(root);
}

void SpatialTree::traversePreOrder() const {
    traversePreOrderRecursive(root);
}

void SpatialTree::traversePostOrder() const {
    traversePostOrderRecursive(root);
}
