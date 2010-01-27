// Hive Mariachi Engine
// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Mariachi Engine. If not, see <http://www.gnu.org/licenses/>.

// __author__    = Tiago Silva <tsilva@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "../../structures/structures.h"

#include "a_star.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
AStar::AStar() {
}

AStar::AStar(PathNodesGraph *nodesGraph) : PathFinder(nodesGraph) {
}

/**
* Destructor of the class.
*/
AStar::~AStar() {
}

std::list<unsigned int> AStar::findPath(unsigned int startNodeId, unsigned int endNodeId, std::map<unsigned int, Coordinate3d_t *> nodesMap, std::map<unsigned int, std::vector<unsigned int>> neighboursMap) {
    // the found path
    std::list<unsigned int> path;

    // the unexplored nodes ordered by lowest cost
    std::priority_queue<std::pair<unsigned int, float>, std::vector<std::pair<unsigned int, float>>, AStar> openSetQueue;

    // map indicating if a node was explored or note
    std::map<unsigned int, bool> exploredMap;

    // map associating a node with the lowest cost found to reach it
    std::map<unsigned int, Cost_t *> costMap;

    // map associating a node with the lowest cost node that was used to reach it
    std::map<unsigned int, unsigned int> previousNodeMap;

    // retrieves the coordinates for the start and end nodes
    Coordinate3d_t *startNodeCoordinate3d = nodesMap[startNodeId];
    Coordinate3d_t *endNodeCoordinate3d = nodesMap[endNodeId];

    // stores the costs for the start node
    Cost_t startNodeCost;
    startNodeCost.cost = 0;
    startNodeCost.heuristic = this->calculateDistance(startNodeCoordinate3d, endNodeCoordinate3d);
    startNodeCost.total = startNodeCost.cost + startNodeCost.heuristic;
    costMap[startNodeId] = &startNodeCost;

    // adds the start node to the open set queue and marks it as unexplored
    openSetQueue.push(std::pair<unsigned int, float>(startNodeId, startNodeCost.total));
    exploredMap[startNodeId] = false;

    // while the open set is not empty
    while(openSetQueue.size() > 0) {
        // retrieves the node with lowest total cost
        std::pair<unsigned int, float> currentNodePair = openSetQueue.top();
        unsigned int currentNodeId = currentNodePair.first;

        // returns the path in case the goal as been reached
        if (currentNodeId == endNodeId) {
            // backtracks from the end node storing the nodes in a list
            while(previousNodeMap.find(currentNodeId) != previousNodeMap.end()) {
                path.push_front(currentNodeId);
                currentNodeId = previousNodeMap[currentNodeId];
            }
            path.push_front(currentNodeId);

            return path;
        }

        // removes the node from the open set and marks it as explored
        openSetQueue.pop();
        exploredMap[currentNodeId] = true;

        // iterates through the nodes neighbours
        std::vector<unsigned int> neighbourNodeIds = neighboursMap[currentNodeId];

        // iterates verl all the neighbour node ids
        for(std::vector<unsigned int>::iterator neighbourNodeIdsIterator = neighbourNodeIds.begin();  neighbourNodeIdsIterator != neighbourNodeIds.end(); neighbourNodeIdsIterator++) {
            // retrieves the neighbour's id
            unsigned int neighbourNodeId = (unsigned int) *neighbourNodeIdsIterator;

            // skips in case the neighbour was already explored
            bool neighbourFirstEncounter = exploredMap.find(neighbourNodeId) == exploredMap.end();
            if(!neighbourFirstEncounter && exploredMap[neighbourNodeId]) {
                continue;
            }

            // retrieves the current node's coordinates
            Coordinate3d_t *currentNodeCoordinate3d = nodesMap[currentNodeId];

            // calculates the cost between the current node and the neighbour
            Cost_t *currentNodeCost = costMap[currentNodeId];
            float cost = currentNodeCost->cost + this->calculateDistance(currentNodeCoordinate3d, endNodeCoordinate3d);

            // @todo: review this
            // creates a cost structure for the neighbour in case one doesn't exist yet
            Cost_t *neighbourNodeCost;
            if(costMap.find(neighbourNodeId) == costMap.end()) {
                Cost_t neighbourCost;
                costMap[neighbourNodeId] = &neighbourCost;
                neighbourNodeCost = &neighbourCost;
            } else {
                // otherwise retrieves it
                neighbourNodeCost = costMap[neighbourNodeId];
            }

            // figures out if this is a better path to the neighbour node
            bool better = neighbourFirstEncounter || cost < neighbourNodeCost->cost;

            // updates the neighbour's values in case this is a better path to it
            if(better) {
                // marks the current node as the best path to the neighbour node
                previousNodeMap[neighbourNodeId] = currentNodeId;

                // retrieves the neighbour node's coordinates
                Coordinate3d_t *neighbourNodeCoordinate3d = nodesMap[neighbourNodeId];

                // stores the cost values for the best path to the neighbour node
                neighbourNodeCost->cost = cost;
                neighbourNodeCost->heuristic = this->calculateDistance(neighbourNodeCoordinate3d, endNodeCoordinate3d);
                neighbourNodeCost->total = neighbourNodeCost->cost + neighbourNodeCost->heuristic;

                // adds the node to the open set in case it wasn't added before
                if(neighbourFirstEncounter) {
                    std::pair<unsigned int, float> neighbourNodePair = std::pair<unsigned int, float>(neighbourNodeId, neighbourNodeCost->total);
                    openSetQueue.push(neighbourNodePair);
                    exploredMap[neighbourNodeId] = false;
                }
            }
        }
    }

    return path;
}

bool AStar::operator()(std::pair<unsigned int, float> &firstPair, std::pair<unsigned int, float> &secondPair) {
    return firstPair.second > secondPair.second;
}

float AStar::calculateDistance(Coordinate3d_t *firstCoordinates, Coordinate3d_t *secondCoordinates) {
    return sqrt(pow(secondCoordinates->x - firstCoordinates->x, 2) + pow(secondCoordinates->y - firstCoordinates->y, 2) + pow(secondCoordinates->z - firstCoordinates->z, 2));
}
