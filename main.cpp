#include <iostream>
#include <vector>
#include <cfloat>
#include "A Star/AStar.h"

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

    // ********* Astar Calcul *********

    int startNodeX = 9;
    int startNodeY = 7;

    int targetNodeX = 11; // 29
    int targetNodeY = 7; // 14

    int startNodeIndex = startNodeX + startNodeY * sizeX;
    int targetNodeIndex = targetNodeX + targetNodeY * sizeX;

    AStarResult result = AStar(graph, sizeX, sizeY, graph[startNodeIndex], graph[targetNodeIndex]);

    // Afficher la grille
    DisplayGrid(graph, sizeX, sizeY, result.closedList, result.path,
                graph[startNodeIndex], graph[targetNodeIndex]);

    // nettoyage (pour éviter les fuites mémoire)
    for (Node* n : graph) {
        delete n;
    }

    return 0;
}

