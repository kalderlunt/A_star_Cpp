#include "AStar.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <thread>
#include <chrono>
#include <cstdlib>

// Fonction pour effacer l'écran de manière cross-platform
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // Essayer d'abord avec les codes ANSI
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();

    // Si ça ne marche pas, on peut aussi essayer avec system
    // system("clear");
#endif
}

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

// Reconstruit le chemin avec visualisation étape par étape
std::vector<Node*> ReconstructPathWithVisualization(const std::vector<Node*>& from, Node* startNode, Node* targetNode, int sizeX,
                                                     const std::vector<Node*>& graph, int sizeY,
                                                     const std::vector<Node*>& closedList, float delayBetweenSteps) {
    std::vector<Node*> path;
    Node* currentNode = targetNode;
    int step = 0;

    // Construire le chemin du target vers le start
    while (currentNode != nullptr)
    {
        path.push_back(currentNode);
        step++;

        // Afficher la reconstruction en cours
        ClearScreen();
        std::cout << "=== RECONSTRUCTION DU CHEMIN ===\n";
        std::cout << "== Etape " << step << " ==\n";
        std::cout << "Légende: . = vide | # = obstacle | A = closedList | P = chemin | S = départ | T = cible | - = noeud actuel\n\n";
        std::cout << "Noeuds dans le chemin: " << path.size() << "\n\n";

        // Afficher la grille avec le chemin en cours de reconstruction
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                int index = x + y * sizeX;
                Node* node = graph[index];

                char symbol = '.';

                if (node == startNode) {
                    symbol = 'S';
                }
                else if (node == targetNode) {
                    symbol = 'T';
                }
                else if (node == currentNode) {
                    symbol = '-'; // Nœud en cours de traitement
                }
                else if (node->isObstacle) {
                    symbol = '#';
                }
                else if (Contains(path, node)) {
                    symbol = 'P'; // Chemin reconstruit
                }
                else if (Contains(closedList, node)) {
                    symbol = 'A'; // Nœuds visités
                }

                std::cout << symbol << " ";
            }
            std::cout << "\n";
        }

        // Pause pour visualisation
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayBetweenSteps * 1000)));

        if (currentNode == startNode)
            break;
        currentNode = from[currentNode->x + currentNode->y * sizeX];
    }

    // Inverser le chemin pour aller du start au target
    std::reverse(path.begin(), path.end());

    // Afficher le chemin final
    ClearScreen();
    std::cout << "=== RECONSTRUCTION DU CHEMIN ===\n";
    std::cout << "== Chemin Final ==\n";
    std::cout << "Légende: . = vide | # = obstacle | A = closedList | P = chemin | S = départ | T = cible\n\n";
    std::cout << "Longueur du chemin: " << path.size() << " noeuds\n\n";

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            int index = x + y * sizeX;
            Node* node = graph[index];

            char symbol = '.';

            if (node == startNode) {
                symbol = 'S';
            }
            else if (node == targetNode) {
                symbol = 'T';
            }
            else if (node->isObstacle) {
                symbol = '#';
            }
            else if (Contains(path, node)) {
                symbol = 'P'; // Chemin final
            }
            else if (Contains(closedList, node)) {
                symbol = 'A'; // Nœuds visités
            }

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayBetweenSteps * 2000)));

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

    std::cout << "=== GRILLE A* ===\n";
    std::cout << "== Resultat Final ==\n";
    std::cout << "Légende: . = vide | # = obstacle | A = closedList | P = chemin | S = départ | T = cible\n\n";
    std::cout << "Noeuds visités: " << closedList.size() << " | Longueur du chemin: " << path.size() << "\n\n";

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
                symbol = '#';
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

// Fonction helper pour afficher la grille avec le nœud actuel en cours de traitement
void DisplayGridWithCurrentNode(const std::vector<Node*>& graph, int sizeX, int sizeY,
                                 const std::vector<Node*>& closedList, const std::vector<Node*>& openList,
                                 Node* startNode, Node* targetNode, Node* currentNode, int iteration) {

    // Effacer l'écran de manière cross-platform
    ClearScreen();

    std::cout << "=== PROPAGATION A* ===\n";
    std::cout << "== Iteration " << iteration << " ==\n";
    std::cout << "Légende: . = vide | # = obstacle | + = openList | A = closedList | S = départ | T = cible | - = noeud actuel en traitement\n\n";
    std::cout << "OpenList: " << openList.size() << " noeuds | ClosedList: " << closedList.size() << " noeuds\n\n";

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            int index = x + y * sizeX;
            Node* node = graph[index];

            char symbol = '.';

            // Priorité d'affichage
            if (node == currentNode) {
                symbol = '-'; // Nœud en cours de traitement
            }
            else if (node == startNode) {
                symbol = 'S';
            }
            else if (node == targetNode) {
                symbol = 'T';
            }
            else if (node->isObstacle) {
                symbol = '#';
            }
            else if (Contains(closedList, node)) {
                symbol = 'A';
            }
            else if (Contains(openList, node)) {
                symbol = '+'; // Dans l'open list
            }

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

AStarResult AStarWithVisualization(std::vector<Node*>& graph, int sizeX, int sizeY, Node* startNode, Node* targetNode, float delayBetweenSteps) {
    std::vector<Node*> closedList;
    std::vector<Node*> openList;
    std::vector<Node*> pathToTarget(sizeX * sizeY, nullptr);

    bool targetIsFound = false;
    int iteration = 0;

    startNode->GWeight = 0;
    openList.push_back(startNode);

    while (!openList.empty() && !targetIsFound) {
        iteration++;

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

        // Afficher la grille avec le nœud actuel
        DisplayGridWithCurrentNode(graph, sizeX, sizeY, closedList, openList, startNode, targetNode, lessCostNode, iteration);

        // Pause pour voir la propagation
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayBetweenSteps * 1000)));

        openList.erase(std::remove(openList.begin(), openList.end(), lessCostNode), openList.end());

        // Récupérer les voisins de l'élément le moins coûteux
        std::vector<Node*> neighbors;
        int widthToCheckNeighbor = 1;

        for (int x = -widthToCheckNeighbor; x <= widthToCheckNeighbor; x++) {
            for (int y = -widthToCheckNeighbor; y <= widthToCheckNeighbor; y++) {

                if (x == 0 && y == 0)
                    continue;

                int neighborX = lessCostNode->x + x;
                int neighborY = lessCostNode->y + y;

                if (neighborX < 0 || neighborY < 0 || neighborX >= sizeX || neighborY >= sizeY)
                    continue;

                int neighborIndex = neighborX + neighborY * sizeX;

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
                pathToTarget[neighborIdx] = lessCostNode;
                AddUnique(openList, neighbor);
            }
        }
        AddUnique(closedList, lessCostNode);
    }

    // Afficher le résultat final et reconstruire le chemin avec visualisation
    std::vector<Node*> path;
    if (targetIsFound) {
        ClearScreen();
        std::cout << "\n=== CIBLE TROUVEE ===\n";
        std::cout << "Visited nodes: " << closedList.size() << " nodes\n";
        std::cout << "Iterations: " << iteration << "\n";
        std::cout << "\nDébut de la reconstruction du chemin...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Reconstruire le chemin avec visualisation
        path = ReconstructPathWithVisualization(pathToTarget, startNode, targetNode, sizeX, graph, sizeY, closedList, delayBetweenSteps);

        std::cout << "\n=== RECONSTRUCTION TERMINEE ===\n";
        std::cout << "Path length: " << path.size() << " nodes\n\n";
    } else {
        std::cout << "\n=== CIBLE NON TROUVEE ===\n";
    }

    return {path, closedList};
}

