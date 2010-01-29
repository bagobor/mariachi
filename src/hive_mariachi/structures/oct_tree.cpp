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

// __author__    = Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "oct_tree.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
OctTree::OctTree() {
}

OctTree::OctTree(float boxWidth, float boxHeight, float boxDepth) {
    // updates the bounding box limits
    this->boundingBox.x1 = 0.0;
    this->boundingBox.y1 = 0.0;
    this->boundingBox.z1 = 0.0;
    this->boundingBox.x2 = boxWidth;
    this->boundingBox.y2 = boxHeight;
    this->boundingBox.z2 = boxDepth;
}

OctTree::OctTree(Box3d_t boundingBox) {
    // copies the provided bounding box
    this->boundingBox = boundingBox;
}

OctTree::~OctTree() {
    // @todo: delete the bounding box?
    // @todo: delete the child nodes?
    unsigned int childNodesSize = this->childNodes.size();
    for(unsigned int i = 0; i < childNodesSize; i++) {
        delete this->childNodes[i];
    }
}

/**
* Inserts in the target list in case of overlapping (black boxes),
* recursively tries to insert the target inside the target list of the containing tree nodes
* (discards white boxes and subdivides grey boxes).
*/
void OctTree::insertTargetBox(Node *targetNode, Box3d_t *targetBoundingBox) {
    // initializes the stack of nodes to process
    std::vector<OctTreeBox_t> nodesStack;

    // creates the variables for the current values
    OctTreeBox_t currentNodeBox;
    OctTree *currentNode;
    Box3d_t currentBox;

    // declares the oct tree node boxes list
    OctTreeBox_t nodeBoxes[8];

    // declares the list of oct tree node pointers
    OctTree *nodes[8];

    // declares the list of bounding boxes
    Box3d_t boxes[8];

    // creates a node box with the start element
    nodeBoxes[0].node = this;
    nodeBoxes[0].box = *targetBoundingBox;

    // pushes the first oct tree node onto the stack
    nodesStack.push_back(nodeBoxes[0]);

    // until the stack is not empty
    while(!nodesStack.empty()) {
        // retrieves the current node box structure
        currentNodeBox = nodesStack.back();

        // removes the current node from the stack
        nodesStack.pop_back();

        // retrieves the current node
        currentNode = currentNodeBox.node;

        // retrieves the box with which to bound
        currentBox = currentNodeBox.box;

        // tests for overlapping
        if (currentNode->overlaps(&currentNode->boundingBox, &currentBox)) {
            // adds the target
            currentNode->targets.push_back(targetNode);

            // continues the loop
            continue;
        }

        // in case the octant nodes have not been yet generated
        if (currentNode->childNodes.size() == 0) {
            currentNode->generateChildNodes();
        }

        // retrieves the target's bounding box's vertexes
        Coordinate3d_t leftTopBackPoint = {currentBox.x1, currentBox.y2, currentBox.z1};
        Coordinate3d_t rightTopBackPoint = {currentBox.x2, currentBox.y2, currentBox.z1};
        Coordinate3d_t leftBottomBackPoint = {currentBox.x1, currentBox.y1, currentBox.z1};
        Coordinate3d_t rightBottomBackPoint = {currentBox.x2, currentBox.y1, currentBox.z1};
        Coordinate3d_t leftTopFrontPoint = {currentBox.x1, currentBox.y2, currentBox.z2};
        Coordinate3d_t rightTopFrontPoint = {currentBox.x2, currentBox.y2, currentBox.z2};
        Coordinate3d_t leftBottomFrontPoint = {currentBox.x1, currentBox.y1, currentBox.z2};
        Coordinate3d_t rightBottomFrontPoint = {currentBox.x2, currentBox.y1, currentBox.z2};

        // retrieves the octants for each of the target's bounding box's vertexes
        int leftTopBackPointOctant = currentNode->getPointOctant(leftTopBackPoint);
        int rightTopBackPointOctant = currentNode->getPointOctant(rightTopBackPoint);
        int leftBottomBackPointOctant = currentNode->getPointOctant(leftBottomBackPoint);
        int rightBottomBackPointOctant = currentNode->getPointOctant(rightBottomBackPoint);
        int leftTopFrontPointOctant = currentNode->getPointOctant(leftTopFrontPoint);
        int rightTopFrontPointOctant = currentNode->getPointOctant(rightTopFrontPoint);
        int leftBottomFrontPointOctant = currentNode->getPointOctant(leftBottomFrontPoint);
        int rightBottomFrontPointOctant = currentNode->getPointOctant(rightBottomFrontPoint);

        // in case the target is contained by a single octant
        if(leftTopBackPointOctant == rightTopBackPointOctant &&
            leftTopBackPointOctant == leftBottomBackPointOctant &&
            leftTopBackPointOctant == leftTopFrontPointOctant) {
                // determines the octant
                nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                // determines the octant box
                boxes[0] = currentBox;

                // wraps and pushes the node for processing, along with respective box
                this->pushNodeBoxes(1, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target horizontally spans two octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
                    // determines the lower x octant
                    nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x octant
                    nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

                    // creates the lower x octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, currentBox.y2, currentBox.z2);

                    // creates the higher x octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the two nodes for processing, along with respective boxes
                    this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target vertically spans two octants
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
                    // determines the lower y octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher y octant
                    nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

                    // creates the lower y octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[0]->boundingBox.y2, currentBox.z2);

                    // creates the higher y octant box
                    this->fillBox(&boxes[1], currentBox.x1, nodes[1]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the two nodes for processing, along with respective boxes
                    this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans two octants accross the z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower z octant
                    nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher z octant
                    nodes[1] = currentNode->childNodes[leftTopFrontPointOctant];

                    // creates the lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher z octant box
                    this->fillBox(&boxes[1], currentBox.x1, currentBox.y1, nodes[1]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the two nodes for processing, along with respective boxes
                    this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans four octants accross the x and y-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
                    // determines the lower x, lower y octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher x, lower y octant
                    nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

                    // determines the lower x, higher y octant
                    nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x, higher y octant
                    nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

                    // creates the lower x, lower y octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, nodes[0]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, lower y octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[1]->boundingBox.y2, currentBox.z2);

                    // creates the lower x, higher y octant box
                    this->fillBox(&boxes[2], currentBox.x1, currentBox.y1, currentBox.z1, nodes[2]->boundingBox.x2, nodes[2]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, higher y octant box
                    this->fillBox(&boxes[3], nodes[3]->boundingBox.x1, nodes[3]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the four nodes for processing, along with respective boxes
                    this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans four octants accross the x and z-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower x, lower z octant
                    nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x, lower z octant
                    nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

                    // determines the lower x, higher z octant
                    nodes[2] = currentNode->childNodes[leftTopFrontPointOctant];

                    // determines the higher x, higher z octant
                    nodes[3] = currentNode->childNodes[rightTopFrontPointOctant];

                    // creates the lower x, lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, currentBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher x, lower z octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[1]->boundingBox.z2);

                    // creates the lower x, higher z octant box
                    this->fillBox(&boxes[2], currentBox.x1, currentBox.y1, nodes[2]->boundingBox.z1, nodes[2]->boundingBox.x2, currentBox.y2, currentBox.z2);

                    // creates the higher x, higher z octant box
                    this->fillBox(&boxes[3], nodes[3]->boundingBox.x1, currentBox.y1, nodes[3]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the four nodes for processing, along with respective boxes
                    this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans four octants accross the y and z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower y, lower z octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher y, lower z octant
                    nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the lower y, higher z octant
                    nodes[2] = currentNode->childNodes[leftBottomFrontPointOctant];

                    // determines the higher y, higher z octant
                    nodes[3] = currentNode->childNodes[leftTopFrontPointOctant];

                    // creates the lower y, lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[0]->boundingBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher y, lower z octant box
                    this->fillBox(&boxes[1], currentBox.x1, nodes[1]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[1]->boundingBox.z2);

                    // creates the lower y, higher z octant box
                    this->fillBox(&boxes[2], currentBox.x1, currentBox.y1, nodes[2]->boundingBox.z1, currentBox.x2, nodes[2]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, higher z octant box
                    this->fillBox(&boxes[3], currentBox.x1, nodes[3]->boundingBox.y1, nodes[3]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the four nodes for processing, along with respective boxes
                    this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans eight octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower x, lower y, lower z octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher x, lower y, lower z octant
                    nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

                    // determines the lower x, higher y, lower z octant
                    nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x, higher y, lower z octant
                    nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

                    // determines the lower x, lower y, higher z octant
                    nodes[4] = currentNode->childNodes[leftBottomFrontPointOctant];

                    // determines the higher x, lower y, higher z octant
                    nodes[5] = currentNode->childNodes[rightBottomFrontPointOctant];

                    // determines the lower x, higher y, higher z octant
                    nodes[6] = currentNode->childNodes[leftTopFrontPointOctant];

                    // determines the higher x, higher y, higher z octant
                    nodes[7] = currentNode->childNodes[rightTopFrontPointOctant];

                    // creates the lower x, lower y, lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, nodes[0]->boundingBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher x, lower y, lower z octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[1]->boundingBox.y2, nodes[1]->boundingBox.z2);

                    // creates the lower x, higher y, lower z octant box
                    this->fillBox(&boxes[2], currentBox.x1, nodes[2]->boundingBox.y1, currentBox.z1, nodes[2]->boundingBox.x2, currentBox.y2, nodes[2]->boundingBox.z2);

                    // creates the higher x, higher y, lower z octant box
                    this->fillBox(&boxes[3], nodes[3]->boundingBox.x1, nodes[3]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[3]->boundingBox.z2);

                    // creates the lower x, lower y, higher z octant box
                    this->fillBox(&boxes[4], currentBox.x1, currentBox.y1, nodes[4]->boundingBox.z1, nodes[4]->boundingBox.x2, nodes[4]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, lower y, higher z octant box
                    this->fillBox(&boxes[5], nodes[5]->boundingBox.x1, currentBox.y1, nodes[5]->boundingBox.z1, currentBox.x2, nodes[5]->boundingBox.y2, currentBox.z2);

                    // creates the lower x, higher y, higher z octant box
                    this->fillBox(&boxes[6], currentBox.x1, nodes[6]->boundingBox.y1, nodes[6]->boundingBox.z1, nodes[6]->boundingBox.x2, currentBox.y2, currentBox.z2);

                    // creates the higher x, higher y, higher z octant box
                    this->fillBox(&boxes[7], nodes[7]->boundingBox.x1, nodes[7]->boundingBox.y1, nodes[7]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the eight nodes for processing, along with respective boxes
                    this->pushNodeBoxes(8, nodes, boxes, nodeBoxes, nodesStack);
        }
    }
}

std::vector<Node *> OctTree::getBoxTargets(Box3d_t *queryBox) {
    std::map<Node *, bool> targetsMap;
    std::vector<Node *> targets;

    // initializes the stack of nodes to process
    std::vector<OctTreeBox_t> nodesStack;

    // creates the variables for the current values
    OctTreeBox_t currentNodeBox;
    OctTree *currentNode;
    Box3d_t currentBox;
    Node *currentTarget;

    // declares the oct tree node boxes list
    OctTreeBox_t nodeBoxes[8];

    // declares the list of oct tree node pointers
    OctTree *nodes[8];

    // declares the list of bounding boxes
    Box3d_t boxes[8];

    // creates a node box with the start element
    nodeBoxes[0].node = this;
    nodeBoxes[0].box = *queryBox;

    // pushes the first oct tree node onto the stack
    nodesStack.push_back(nodeBoxes[0]);

    // until the stack is not empty
    while(!nodesStack.empty()) {
        // retrieves the current node box structure
        currentNodeBox = nodesStack.back();

        // removes the current node from the stack
        nodesStack.pop_back();

        // retrieves the current node
        currentNode = currentNodeBox.node;

        // retrieves the box with which to bound
        currentBox = currentNodeBox.box;

        // retrieves the number of targets in the current node
        unsigned int currentNodeTargetsSize = currentNode->targets.size();

        // adds the current node's targets
        for(unsigned int i = 0; i < currentNodeTargetsSize; i++) {
            currentTarget = currentNode->targets[i];

            // in case the target is not in the bit map
            if(targetsMap[currentTarget] != true) {
                // adds the current target
                targets.push_back(currentTarget);

                // signals the target's bit in the map
                targetsMap[currentTarget] = true;
            }
        }

        // tests for overlapping
        if (currentNode->overlaps(&currentNode->boundingBox, &currentBox)) {
            // continues the loop
            continue;
        }

        // in case the octant nodes have not been yet generated
        if (currentNode->childNodes.size() == 0) {
            // continues the loop
            continue;
        }

        // retrieves the query's bounding box's vertexes
        Coordinate3d_t leftTopBackPoint = {currentBox.x1, currentBox.y2, currentBox.z1};
        Coordinate3d_t rightTopBackPoint = {currentBox.x2, currentBox.y2, currentBox.z1};
        Coordinate3d_t leftBottomBackPoint = {currentBox.x1, currentBox.y1, currentBox.z1};
        Coordinate3d_t rightBottomBackPoint = {currentBox.x2, currentBox.y1, currentBox.z1};
        Coordinate3d_t leftTopFrontPoint = {currentBox.x1, currentBox.y2, currentBox.z2};
        Coordinate3d_t rightTopFrontPoint = {currentBox.x2, currentBox.y2, currentBox.z2};
        Coordinate3d_t leftBottomFrontPoint = {currentBox.x1, currentBox.y1, currentBox.z2};
        Coordinate3d_t rightBottomFrontPoint = {currentBox.x2, currentBox.y1, currentBox.z2};

        // retrieves the octants for each of the query's bounding box's vertexes
        int leftTopBackPointOctant = currentNode->getPointOctant(leftTopBackPoint);
        int rightTopBackPointOctant = currentNode->getPointOctant(rightTopBackPoint);
        int leftBottomBackPointOctant = currentNode->getPointOctant(leftBottomBackPoint);
        int rightBottomBackPointOctant = currentNode->getPointOctant(rightBottomBackPoint);
        int leftTopFrontPointOctant = currentNode->getPointOctant(leftTopFrontPoint);
        int rightTopFrontPointOctant = currentNode->getPointOctant(rightTopFrontPoint);
        int leftBottomFrontPointOctant = currentNode->getPointOctant(leftBottomFrontPoint);
        int rightBottomFrontPointOctant = currentNode->getPointOctant(rightBottomFrontPoint);

        // in case the query is contained by a single octant
        if(leftTopBackPointOctant == rightTopBackPointOctant &&
            leftTopBackPointOctant == leftBottomBackPointOctant &&
            leftTopBackPointOctant == leftTopFrontPointOctant) {
                // determines the octant
                nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                // determines the octant box
                boxes[0] = currentBox;

                // wraps and pushes the node for processing, along with respective box
                this->pushNodeBoxes(1, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target horizontally spans two octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
                    // determines the lower x octant
                    nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x octant
                    nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

                    // creates the lower x octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, currentBox.y2, currentBox.z2);

                    // creates the higher x octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the two nodes for processing, along with respective boxes
                    this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target vertically spans two octants
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
                    // determines the lower y octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher y octant
                    nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

                    // creates the lower y octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[0]->boundingBox.y2, currentBox.z2);

                    // creates the higher y octant box
                    this->fillBox(&boxes[1], currentBox.x1, nodes[1]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the two nodes for processing, along with respective boxes
                    this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans two octants accross the z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower z octant
                    nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher z octant
                    nodes[1] = currentNode->childNodes[leftTopFrontPointOctant];

                    // creates the lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher z octant box
                    this->fillBox(&boxes[1], currentBox.x1, currentBox.y1, nodes[1]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the two nodes for processing, along with respective boxes
                    this->pushNodeBoxes(2, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans four octants accross the x and y-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant == leftTopFrontPointOctant) {
                    // determines the lower x, lower y octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher x, lower y octant
                    nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

                    // determines the lower x, higher y octant
                    nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x, higher y octant
                    nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

                    // creates the lower x, lower y octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, nodes[0]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, lower y octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[1]->boundingBox.y2, currentBox.z2);

                    // creates the lower x, higher y octant box
                    this->fillBox(&boxes[2], currentBox.x1, currentBox.y1, currentBox.z1, nodes[2]->boundingBox.x2, nodes[2]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, higher y octant box
                    this->fillBox(&boxes[3], nodes[3]->boundingBox.x1, nodes[3]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the four nodes for processing, along with respective boxes
                    this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans four octants accross the x and z-axis
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant == leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower x, lower z octant
                    nodes[0] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x, lower z octant
                    nodes[1] = currentNode->childNodes[rightTopBackPointOctant];

                    // determines the lower x, higher z octant
                    nodes[2] = currentNode->childNodes[leftTopFrontPointOctant];

                    // determines the higher x, higher z octant
                    nodes[3] = currentNode->childNodes[rightTopFrontPointOctant];

                    // creates the lower x, lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, currentBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher x, lower z octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[1]->boundingBox.z2);

                    // creates the lower x, higher z octant box
                    this->fillBox(&boxes[2], currentBox.x1, currentBox.y1, nodes[2]->boundingBox.z1, nodes[2]->boundingBox.x2, currentBox.y2, currentBox.z2);

                    // creates the higher x, higher z octant box
                    this->fillBox(&boxes[3], nodes[3]->boundingBox.x1, currentBox.y1, nodes[3]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the four nodes for processing, along with respective boxes
                    this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans four octants accross the y and z-axis
        else if(leftTopBackPointOctant == rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower y, lower z octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher y, lower z octant
                    nodes[1] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the lower y, higher z octant
                    nodes[2] = currentNode->childNodes[leftBottomFrontPointOctant];

                    // determines the higher y, higher z octant
                    nodes[3] = currentNode->childNodes[leftTopFrontPointOctant];

                    // creates the lower y, lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[0]->boundingBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher y, lower z octant box
                    this->fillBox(&boxes[1], currentBox.x1, nodes[1]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[1]->boundingBox.z2);

                    // creates the lower y, higher z octant box
                    this->fillBox(&boxes[2], currentBox.x1, currentBox.y1, nodes[2]->boundingBox.z1, currentBox.x2, nodes[2]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, higher z octant box
                    this->fillBox(&boxes[3], currentBox.x1, nodes[3]->boundingBox.y1, nodes[3]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the four nodes for processing, along with respective boxes
                    this->pushNodeBoxes(4, nodes, boxes, nodeBoxes, nodesStack);
        }
        // in case the target spans eight octants
        else if(leftTopBackPointOctant != rightTopBackPointOctant &&
                leftTopBackPointOctant != leftBottomBackPointOctant &&
                leftTopBackPointOctant != leftTopFrontPointOctant) {
                    // determines the lower x, lower y, lower z octant
                    nodes[0] = currentNode->childNodes[leftBottomBackPointOctant];

                    // determines the higher x, lower y, lower z octant
                    nodes[1] = currentNode->childNodes[rightBottomBackPointOctant];

                    // determines the lower x, higher y, lower z octant
                    nodes[2] = currentNode->childNodes[leftTopBackPointOctant];

                    // determines the higher x, higher y, lower z octant
                    nodes[3] = currentNode->childNodes[rightTopBackPointOctant];

                    // determines the lower x, lower y, higher z octant
                    nodes[4] = currentNode->childNodes[leftBottomFrontPointOctant];

                    // determines the higher x, lower y, higher z octant
                    nodes[5] = currentNode->childNodes[rightBottomFrontPointOctant];

                    // determines the lower x, higher y, higher z octant
                    nodes[6] = currentNode->childNodes[leftTopFrontPointOctant];

                    // determines the higher x, higher y, higher z octant
                    nodes[7] = currentNode->childNodes[rightTopFrontPointOctant];

                    // creates the lower x, lower y, lower z octant box
                    this->fillBox(&boxes[0], currentBox.x1, currentBox.y1, currentBox.z1, nodes[0]->boundingBox.x2, nodes[0]->boundingBox.y2, nodes[0]->boundingBox.z2);

                    // creates the higher x, lower y, lower z octant box
                    this->fillBox(&boxes[1], nodes[1]->boundingBox.x1, currentBox.y1, currentBox.z1, currentBox.x2, nodes[1]->boundingBox.y2, nodes[1]->boundingBox.z2);

                    // creates the lower x, higher y, lower z octant box
                    this->fillBox(&boxes[2], currentBox.x1, nodes[2]->boundingBox.y1, currentBox.z1, nodes[2]->boundingBox.x2, currentBox.y2, nodes[2]->boundingBox.z2);

                    // creates the higher x, higher y, lower z octant box
                    this->fillBox(&boxes[3], nodes[3]->boundingBox.x1, nodes[3]->boundingBox.y1, currentBox.z1, currentBox.x2, currentBox.y2, nodes[3]->boundingBox.z2);

                    // creates the lower x, lower y, higher z octant box
                    this->fillBox(&boxes[4], currentBox.x1, currentBox.y1, nodes[4]->boundingBox.z1, nodes[4]->boundingBox.x2, nodes[4]->boundingBox.y2, currentBox.z2);

                    // creates the higher x, lower y, higher z octant box
                    this->fillBox(&boxes[5], nodes[5]->boundingBox.x1, currentBox.y1, nodes[5]->boundingBox.z1, currentBox.x2, nodes[5]->boundingBox.y2, currentBox.z2);

                    // creates the lower x, higher y, higher z octant box
                    this->fillBox(&boxes[6], currentBox.x1, nodes[6]->boundingBox.y1, nodes[6]->boundingBox.z1, nodes[6]->boundingBox.x2, currentBox.y2, currentBox.z2);

                    // creates the higher x, higher y, higher z octant box
                    this->fillBox(&boxes[7], nodes[7]->boundingBox.x1, nodes[7]->boundingBox.y1, nodes[7]->boundingBox.z1, currentBox.x2, currentBox.y2, currentBox.z2);

                    // wraps and pushes the eight nodes for processing, along with respective boxes
                    this->pushNodeBoxes(8, nodes, boxes, nodeBoxes, nodesStack);
        }
    }

    return targets;
}

/*
* Initializes the octant nodes by building eight sub-partitions for the node's partitioned space.
*/
inline void OctTree::generateChildNodes() {
    float x1 = this->boundingBox.x1;
    float y1 = this->boundingBox.y1;
    float z1 = this->boundingBox.z1;

    float x2 = this->boundingBox.x2;
    float y2 = this->boundingBox.y2;
    float z2 = this->boundingBox.z2;

    float width = x2 - x1;
    float height = y2 - y1;
    float depth = z2 - z1;

    float halfWidth = floor(width / 2);
    float halfHeight = floor(height / 2);
    float halfDepth = floor(depth / 2);

    Box3d_t firstOctantBox = {x1, y1, z1, x1 + halfWidth, y1 + halfHeight, z1 + halfDepth};
    Box3d_t secondOctantBox = {x1 + halfWidth, y1, z1, x2, y1 + halfHeight, z1 + halfDepth};
    Box3d_t thirdOctantBox = {x1, y1 + halfHeight, z1, x1 + halfWidth, y2, z1 + halfDepth};
    Box3d_t fourthOctantBox = {x1 + halfWidth, y1 + halfHeight, z1, x2, y2, z1 + halfDepth};
    Box3d_t fifthOctantBox = {x1, y1, z1 + halfWidth, x1 + halfWidth, y1 + halfHeight, z2};
    Box3d_t sixthOctantBox = {x1 + halfWidth, y1, z1 + halfDepth, x2, y1 + halfHeight, z2};
    Box3d_t seventhOctantBox = {x1, y1 + halfHeight, z1 + halfDepth, x1 + halfWidth, y2, z2};
    Box3d_t eighthOctantBox = {x1 + halfWidth, y1 + halfHeight, z1 + halfDepth, x2, y2, z2};

    // creates the octant nodes
    OctTree *firstOctantNode = this->createChildNode(firstOctantBox);
    OctTree *secondOctantNode = this->createChildNode(secondOctantBox);
    OctTree *thirdOctantNode = this->createChildNode(thirdOctantBox);
    OctTree *fourthOctantNode = this->createChildNode(fourthOctantBox);
    OctTree *fifthOctantNode = this->createChildNode(fifthOctantBox);
    OctTree *sixthOctantNode = this->createChildNode(sixthOctantBox);
    OctTree *seventhOctantNode = this->createChildNode(seventhOctantBox);
    OctTree *eighthOctantNode = this->createChildNode(eighthOctantBox);

    // adds the octant nodes to the list
    this->childNodes.push_back(firstOctantNode);
    this->childNodes.push_back(secondOctantNode);
    this->childNodes.push_back(thirdOctantNode);
    this->childNodes.push_back(fourthOctantNode);
    this->childNodes.push_back(fifthOctantNode);
    this->childNodes.push_back(sixthOctantNode);
    this->childNodes.push_back(seventhOctantNode);
    this->childNodes.push_back(eighthOctantNode);
}

inline OctTree *OctTree::createChildNode(Box3d_t &boundingBox) {
    return new OctTree(boundingBox);
}

inline bool OctTree::overlaps(Box3d_t *box1, Box3d_t *box2) {
    return box1->x1 == box2->x1
        && box1->x2 == box2->x2
        && box1->y1 == box2->y1
        && box1->y2 == box2->y2
        && box1->z1 == box2->z1
        && box1->z2 == box2->z2;
}

inline bool OctTree::contains(Box3d_t &box) {
    Coordinate3d_t point1 = {box.x1, box.y1, box.z1};
    Coordinate3d_t point2 = {box.x2, box.y2, box.z2};

    return this->containsPoint(point1) && this->containsPoint(point2);
}

/*
* Determines if the specified point in contained in the node's bounding box.
*/
inline bool OctTree::containsPoint(Coordinate3d_t &point) {
    return point.x >= this->boundingBox.x1
        && point.y >= this->boundingBox.y1
        && point.z >= this->boundingBox.z1
        && point.x <= this->boundingBox.x2
        && point.y <= this->boundingBox.y2
        && point.z <= this->boundingBox.z2;
}

/*
* Determines in which octant is the given point contained.
*
* @param x
* @param y
* @param z
* @return The octant node index which contains the specified point.
*/
inline int OctTree::getPointOctant(Coordinate3d_t &point) {
    int octantIndex = -1;

    int childNodesSize = this->childNodes.size();

    // for each node
    for (int i = 0; i < childNodesSize; i++) {
        // retrieves the node
        OctTree *octTreeNode = this->childNodes[i];

        // in case the given point is inside the node
        if (octTreeNode->containsPoint(point)) {
            // signals the index was found
            octantIndex = i;
            break;
        }
    }

    if(octantIndex == -1)
        printf("debug");

    // returns the retrieved index
    return octantIndex;
}

inline void OctTree::pushNodeBoxes(int node_count, OctTree **nodes, Box3d_t *boxes, OctTreeBox_t *nodeBoxes, std::vector<OctTreeBox_t> &nodesStack) {
    for(int i = 0; i < node_count; i++) {
        // wraps the node and box
        nodeBoxes[i].node = nodes[i];
        nodeBoxes[i].box = boxes[i];

        // pushes the node box for processing
        nodesStack.push_back(nodeBoxes[i]);

        if(!nodes[i]->contains(boxes[i]))
            printf("debug");
    }
}

inline void OctTree::fillBox(Box3d_t *box, float x1, float y1, float z1, float x2, float y2, float z2) {
    if(x1 > x2 || y1 > y2 || z1 > z2)
        printf("debug");

    box->x1 = x1;
    box->y1 = y1;
    box->z1 = z1;
    box->x2 = x2;
    box->y2 = y2;
    box->z2 = z2;
}

std::string OctTree::toString(std::string padding) {
    char buffer[10000];
    const char *paddingCString = padding.c_str();

    //sprintf_s(buffer, "\n%s<OctTree(0x%x)>", paddingCString, this);
    std::string octTreeNodeString = buffer;

    //sprintf_s(buffer, "\n%s  Bounding Box", paddingCString);
    octTreeNodeString += buffer;

    //sprintf_s(buffer, "\n%s    x1: %f", paddingCString, this->boundingBox.x1);
    octTreeNodeString += buffer;
    //sprintf_s(buffer, "\n%s    y1: %f", paddingCString, this->boundingBox.y1);
    octTreeNodeString += buffer;
    //sprintf_s(buffer, "\n%s    z1: %f", paddingCString, this->boundingBox.z1);
    octTreeNodeString += buffer;
    //sprintf_s(buffer, "\n%s    x2: %f", paddingCString, this->boundingBox.x2);
    octTreeNodeString += buffer;
    //sprintf_s(buffer, "\n%s    y2: %f", paddingCString, this->boundingBox.y2);
    octTreeNodeString += buffer;
    //sprintf_s(buffer, "\n%s    z2: %f", paddingCString, this->boundingBox.z2);
    octTreeNodeString += buffer;

    unsigned int childNodesSize = this->childNodes.size();

    if(childNodesSize != 0) {
        //sprintf_s(buffer, "\n%s  Child Nodes", paddingCString);
        octTreeNodeString += buffer;
        std::string childPadding = padding + "    ";

        for(unsigned int i = 0; i < childNodesSize; i++) {
            OctTree *childNode = this->childNodes[i];
            octTreeNodeString += childNode->toString(childPadding);
        }
    }

    unsigned int targetsSize = this->targets.size();

    if(targetsSize != 0) {
        //sprintf_s(buffer, "\n%s  Targets", paddingCString);
        octTreeNodeString += buffer;


        for(unsigned int i = 0; i < targetsSize; i++) {
            Node *target = this->targets[i];
            //sprintf_s(buffer, "\n%s    Target %d: %s", paddingCString, i, target->name.c_str());
            octTreeNodeString += buffer;
        }
    }
    else {
        //sprintf_s(buffer, "\n%s  <empty targets>", paddingCString);
        octTreeNodeString += buffer;
    }

    //octTreeNodeString += "\n";

    return octTreeNodeString;
}
