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

#include "stdafx.h"

#include "node.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
Node::Node() {
    this->initRenderable();
}

Node::Node(const std::string &name) {
    this->initRenderable();
    this->name = name;
}

/**
* Destructor of the class.
*/
Node::~Node() {
}

inline void Node::initRenderable() {
    this->renderable = false;
}

Node *Node::getParent() {
    return this->parent;
}

void Node::setParent(Node *parent) {
    this->parent = parent;
}

void Node::reparentTo(Node *parent) {
    parent->addChild(this);
}

void Node::addChild(Node *child) {
    this->childrenList.push_back(child);
    child->setParent(this);
}

void Node::removeChild(Node *child) {
    this->childrenList.remove(child);
    child->setParent(NULL);
}

std::list<Node *> &Node::getChildrenList() {
    return this->childrenList;
}

void Node::setChildrenList(std::list<Node *> &childrenList) {
    this->childrenList = childrenList;
}
