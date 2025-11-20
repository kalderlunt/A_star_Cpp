#include <iostream>
#include <vector>
#include <cfloat>
#include "A Star/AStar.h"
#include "TimeSystem/TimeSystem.h"

void InitGraph(std::vector<Node*>& graph, const int sizeX, const int sizeY) {
    graph.clear();

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            Node* node= new Node();
            node->x = x;
            node->y = y;
            node->GWeight = FLT_MAX;

            graph.push_back(node);
        }
    }
}

// Fonction de callback pour être appelée de manière temporisée
void TimedAStarCallback(std::vector<int>& dummy, int iteration, int maxIterations) {
    std::cout << "\n[Timed Callback] Iteration " << iteration << "/" << maxIterations << " executee!\n";
}

int main() {

    // ********* Tableau *********
    const int sizeX = 30;
    const int sizeY = 15;

    std::vector<Node*> graph;
    InitGraph(graph, sizeX, sizeY);

    // ********* Obstacles *********
    // Mur vertical
    for (int y = 3; y < 12; y++) {
        graph[10 + y * sizeX]->isObstacle = true;
    }
    // Mur horizontal
    for (int x = 15; x < 25; x++) {
        graph[x + 7 * sizeX]->isObstacle = true;
    }

    // ********* Astar Calcul avec Visualisation *********

    int startNodeX = 0;
    int startNodeY = 0;

    int targetNodeX = 29;
    int targetNodeY = 14;

    int startNodeIndex = startNodeX + startNodeY * sizeX;
    int targetNodeIndex = targetNodeX + targetNodeY * sizeX;

    AStarResult result = AStarWithVisualization(graph, sizeX, sizeY,
        graph[startNodeIndex], graph[targetNodeIndex], 0.3f);

    // nettoyage (pour éviter les fuites mémoire)
    for (Node* n : graph) {
        delete n;
    }

    return 0;
}

