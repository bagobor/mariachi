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

#include "square_node.h"

using namespace mariachi;

SquareNode::SquareNode() : Node() {
}

SquareNode::SquareNode(const std::string &name) : Node(name) {
}

SquareNode::~SquareNode() {
}

Coordinate2d_t &SquareNode::getPosition() {
    return this->position;
}

void SquareNode::setPosition(Coordinate2d_t &position) {
    this->position = position;
}

void SquareNode::setPosition(float x, float y) {
    Coordinate2d_t position = { x, y } ;

    this->position = position;
}

FloatSize2d_t &SquareNode::getSize() {
    return this->size;
}

void SquareNode::setSize(FloatSize2d_t &size) {
    this->size = size;
}

void SquareNode::setSize(float width, float height) {
    FloatSize2d_t size = { width, height };

    this->size = size;
}
