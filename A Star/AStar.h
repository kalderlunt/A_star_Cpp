#pragma once
#include <vector>

struct Node {
    int x;
    int y;
    float GWeight;
    bool isObstacle = false;
};

struct AStarResult {
    std::vector<Node*> path;
    std::vector<Node*> closedList;
};

// Calcule la distance euclidienne entre deux nœuds
float Distance(const Node& A, const Node& B);

// Vérifie si un nœud est dans une liste
bool Contains(const std::vector<Node*>& list, Node* value);

// Ajoute un nœud à une liste s'il n'y est pas déjà
void AddUnique(std::vector<Node*>& list, Node* value);

// Reconstruit le chemin du nœud de départ au nœud cible
std::vector<Node*> ReconstructPath(const std::vector<Node*>& from, Node* startNode, Node* targetNode, int sizeX);

// Reconstruit le chemin avec visualisation étape par étape
std::vector<Node*> ReconstructPathWithVisualization(const std::vector<Node*>& from, Node* startNode, Node* targetNode, int sizeX,
                                                     const std::vector<Node*>& graph, int sizeY,
                                                     const std::vector<Node*>& closedList, float delayBetweenSteps);

// Algorithme A* pour trouver le plus court chemin
AStarResult AStar(std::vector<Node*>& graph, int sizeX, int sizeY, Node* startNode, Node* targetNode);

// Algorithme A* avec visualisation étape par étape
AStarResult AStarWithVisualization(std::vector<Node*>& graph, int sizeX, int sizeY, Node* startNode, Node* targetNode, float delayBetweenSteps);

// Affiche la grille avec les résultats de A*
void DisplayGrid(const std::vector<Node*>& graph, int sizeX, int sizeY,
                 const std::vector<Node*>& closedList, const std::vector<Node*>& path,
                 Node* startNode, Node* targetNode);
