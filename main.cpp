#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
#include <cfloat>

struct Node {
    int x;
    int y;
    float GWeight;
};

float Distance(const Node& A, const Node& B){
    return std::sqrt(std::pow(A.x - B.x, 2) + std::pow(A.y - B.y, 2));
}

bool Contains(const std::vector<Node*>& list, Node* value) {
    return std::find(list.begin(), list.end(), value) != list.end();
}

void AddUnique(std::vector<Node*>& list, Node* value) {
    if (!Contains(list, value)) {
        list.push_back(value);
    }
}

void AStar (std::vector<Node*>& graph, int sizeX, int sizeY, Node* startNode, Node* targetNode) {
    std::vector<Node*> closedList;
    std::vector<Node*> openList;

    bool targetIsFound = false;

    startNode->GWeight = 0;
    openList.push_back(startNode);

    while (!openList.empty() && !targetIsFound) {
        // Sortir l'élément le moins coûteux de l’OpenList
        Node* lessCostNode = nullptr;
        float lessCostNodeWeight = FLT_MAX;

        for (Node* node : openList) {
            float coordWeight = node->GWeight + Distance(*node, *targetNode);

            if (coordWeight < lessCostNodeWeight) {
                lessCostNode = node;
                lessCostNodeWeight = coordWeight;
            }
        }

        openList.erase(std::remove(openList.begin(), openList.end(), lessCostNode), openList.end());


        // Récupérer les voisins de l'élément le moins coûteux
        std::vector<Node*> neighbors;
        int widthToCheckNeighbor = 1;

        for (int x = -widthToCheckNeighbor; x <= widthToCheckNeighbor; x++) {
            for (int y = -widthToCheckNeighbor; y <= widthToCheckNeighbor; y++) {

                if (x == 0 && y == 0)
                    continue;

                int neighborIndex = lessCostNode->x + x * sizeY + lessCostNode->x + sizeY; // calcul pour trouver l'index dans le tableau 2dimensions avoir un seul tableau.

                /*// hors de la grille
                if (x < 0 || y < 0 || x >= sizeX || y >= sizeY)
                    continue;*/

                if (neighborIndex >= 0 && neighborIndex < graph.size() && !Contains(closedList, graph[neighborIndex])) {
                    neighbors.push_back(graph[neighborIndex]);
                }
            }
        }

        // Ajouter les voisins à l’OpenList
        for (Node* neighbor : neighbors) {
            if (neighbor == targetNode) {
                targetIsFound = true;
            }

            float newGWeight = lessCostNode->GWeight + Distance(*lessCostNode, *neighbor);
            if (newGWeight < neighbor->GWeight) {
                            neighbor->GWeight = lessCostNode->GWeight + Distance(*lessCostNode, *neighbor);
                AddUnique(openList, neighbor);
            }
        }
        AddUnique(closedList, lessCostNode);


        // Faire le chemin retour pour trouver la route la plus rapide.


    }
}

void InitGraph(std::vector<Node>& graph, const int sizeX, const int sizeY) {
    graph.clear();
    graph.resize(sizeX * sizeY);

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            int index = x + y * sizeX;
            graph[index].x = x;
            graph[index].y = y;
            graph[index].GWeight = FLT_MAX;
        }
    }
}

int main() {

    // ********* Tableau *********
    const int sizeX = 30;
    const int sizeY = 15;

    std::vector<Node> graph;
    InitGraph(graph, sizeX, sizeY);


    // ********* Astar calcul *********

    int startNodeX = 0;
    int startNodeY = 0;

    int targetNodeX = 0;
    int targetNodeY = 0;

    int startNodeIndex = startNodeX + startNodeY * startNodeX;
    int targetNodeIndex = targetNodeX + targetNodeY * targetNodeX;

    AStar(graph, sizeX, sizeY, graph[startNodeIndex], graph[targetNodeIndex]);




    return 0;
}
