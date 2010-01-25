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

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#include "../structures/position.h"

/**
* The node type.
*/
#define NODE_TYPE 1

namespace mariachi {
    class Node {
        private:
            Node *parent;
            std::list<Node *> childrenList;
            Coordinate3d_t position;
            Coordinate3d_t rotation;

        public:
            std::string name;
            bool renderable;

            Node();
            Node(const std::string &name);
            ~Node();
            inline void initRenderable();
            Node *getParent();
            void setParent(Node *parent);
            void reparentTo(Node *parent);
            void addChild(Node *child);
            void removeChild(Node *child);
            bool isRenderable();
            Coordinate3d_t &getPosition();
            void setPosition(Coordinate3d_t &position);
            void setPosition(float x, float y, float z);
            std::list<Node *> &getChildrenList();
            void setChildrenList(std::list<Node *> &childrenList);
            bool getRenderable();
            void setRenderable(bool renderable);
            virtual inline unsigned int getNodeType() { return NODE_TYPE; };
    };
}
