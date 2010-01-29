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

#pragma once

#include "../nodes/nodes.h"
#include "box.h"
#include "position.h"

namespace mariachi {
    class OctTree;

    typedef struct OctTreeBox_t {
        OctTree *node;
        Box3d_t box;
    } OctTreeBox;

    class OctTree {
        private:
            Box3d_t boundingBox;
            std::vector<OctTree *> childNodes;
            std::vector<Node *> targets;

            inline void generateChildNodes();
            inline OctTree *createChildNode(Box3d_t &boundingBox);
            inline bool overlaps(Box3d_t *box1, Box3d_t *box2);
            inline bool intersects(Box3d_t &box);
            inline bool contains(Box3d_t &box);
            inline bool isContainedBy(Box3d_t &box);
            inline bool containsPoint(Coordinate3d_t &point);
            inline int getPointOctant(Coordinate3d_t &point);
            inline void pushNodeBoxes(int node_count, OctTree **nodes, Box3d_t *boxes, OctTreeBox_t *nodeBoxes, std::vector<OctTreeBox_t> &nodesStack);
            inline void fillBox(Box3d_t *box, float x1, float y1, float z1, float x2, float y2, float z2);

        public:
            OctTree();
            OctTree(float boxWidth, float boxHeight, float boxDepth);
            OctTree(Box3d_t boundingBox);
            ~OctTree();
            void insertTargetBox(Node *targetNode, Box3d_t *targetBoundingBox);
            std::vector<Node *> getBoxTargets(Box3d_t *queryBox);
            std::string toString(std::string padding = "");
    };
}
