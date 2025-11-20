#include "AStar.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cfloat>

float Distance(const Node& A, const Node& B) {
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

std::vector<Node*> ReconstructPath(const std::vector<Node*>& from, Node* startNode, Node* targetNode, int sizeX) {
    std::vector<Node*> path;

    Node* currentNode = targetNode;

    while (currentNode != nullptr)
    {
        path.push_back(currentNode);
        if (currentNode == startNode)
            break;
        currentNode = from[currentNode->x + currentNode->y * sizeX];
    }

    std::reverse(path.begin(), path.end());
    return path;
}

AStarResult AStar(std::vector<Node*>& graph, int sizeX, int sizeY, Node* startNode, Node* targetNode) {
    std::vector<Node*> closedList;
    std::vector<Node*> openList;
    std::vector<Node*> pathToTarget(sizeX * sizeY, nullptr); // Tableau pour enregistrer le chemin le plus court

    bool targetIsFound = false;

    startNode->GWeight = 0;
    openList.push_back(startNode);

    while (!openList.empty() && !targetIsFound) {
        // Sortir l'élément le moins coûteux de l'OpenList
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

                // Calcul correct de l'index du voisin
                int neighborX = lessCostNode->x + x;
                int neighborY = lessCostNode->y + y;

                // Si le voisin est dans la grille
                if (neighborX < 0 || neighborY < 0 || neighborX >= sizeX || neighborY >= sizeY)
                    continue;

                int neighborIndex = neighborX + neighborY * sizeX;

                // Vérifier si le voisin n'est pas dans la ClosedList et n'est pas un obstacle
                if (neighborIndex >= 0 && neighborIndex < graph.size() &&
                    !Contains(closedList, graph[neighborIndex]) && !graph[neighborIndex]->isObstacle) {
                    neighbors.push_back(graph[neighborIndex]);
                }
            }
        }

        // Ajouter les voisins à l'OpenList
        for (Node* neighbor : neighbors) {
            if (neighbor == targetNode) {
                targetIsFound = true;
                int neighborIdx = neighbor->x + neighbor->y * sizeX;
                pathToTarget[neighborIdx] = lessCostNode;
            }

            float newGWeight = lessCostNode->GWeight + Distance(*lessCostNode, *neighbor);

            if (newGWeight < neighbor->GWeight) {
                neighbor->GWeight = newGWeight;
                int neighborIdx = neighbor->x + neighbor->y * sizeX;
                pathToTarget[neighborIdx] = lessCostNode; // Tracer le chemin
                AddUnique(openList, neighbor);
            }
        }
        AddUnique(closedList, lessCostNode);
    }

    // Faire le chemin retour pour trouver la route la plus rapide.
    if (!targetIsFound)
    {
        // pas de chemin
        std::cout << "Target not found\n";
        return {std::vector<Node*>(), closedList};
    }

    // reconstituer le chemin complet
    std::vector<Node*> path = ReconstructPath(pathToTarget, startNode, targetNode, sizeX);

    // ********* Astar Result *********
    std::cout << "\n=== DEBUG A* ===\n";
    std::cout << "Path length: " << path.size() << " nodes\n";
    std::cout << "Visited nodes: " << closedList.size() << " nodes\n";

    if (!path.empty()) {
        std::cout << "Path: ";

        for (Node* node : path) {
            std::cout << "(" << node->x << "," << node->y << ") ";
        }

        std::cout << "\n";
    }

    return {path, closedList};
}

void DisplayGrid(const std::vector<Node*>& graph, int sizeX, int sizeY,
                 const std::vector<Node*>& closedList, const std::vector<Node*>& path,
                 Node* startNode, Node* targetNode) {

    std::cout << "\n\n== GRILLE A* ===\n";
    std::cout << "Légende: . = vide | A = visité | O = obstacle | P = chemin | S = départ | T = cible\n\n";

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            int index = x + y * sizeX;
            Node* node = graph[index];

            char symbol = '.';

            // Priorité d'affichage
            if (node == startNode) {
                symbol = 'S';
            }
            else if (node == targetNode) {
                symbol = 'T';
            }
            else if (node->isObstacle) {
                symbol = 'O';
            }
            else if (Contains(path, node)) {
                symbol = 'P';
            }
            else if (Contains(closedList, node)) {
                symbol = 'A';
            }

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
