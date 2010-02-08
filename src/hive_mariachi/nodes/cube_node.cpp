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

#include "cube_node.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
CubeNode::CubeNode() : Node() {
    // initialize the cube node's transforms (translation, scale and rotation)
    this->initTransforms();
}

CubeNode::CubeNode(const std::string &name) : Node(name) {
    // initialize the cube node's transforms (translation, scale and rotation)
    this->initTransforms();
}

/**
* Destructor of the class.
*/
CubeNode::~CubeNode() {
}

inline void CubeNode::initTransforms() {
    // creats the origin position
    Coordinate3d_t initialPosition = { 0.0f, 0.0f, 0.0f };

    // initializes the node's position
    this->position = initialPosition;

    // creates an empty rotation
    Rotation3d_t initialRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

    // initializes the node's rotation
    this->rotation = initialRotation;

    // creates the identity scale
    Coordinate3d_t initialScale = { 1.0f, 1.0f, 1.0f };

    // initializes the node's scale
    this->scale = initialScale;
}

Coordinate3d_t &CubeNode::getPosition() {
    return this->position;
}

void CubeNode::setPosition(Coordinate3d_t &position) {
    this->position = position;
}

void CubeNode::setPosition(float x, float y, float z) {
    Coordinate3d_t position = {x, y, z};

    this->position = position;
}

Rotation3d_t &CubeNode::getRotation() {
    return this->rotation;
}

void CubeNode::setRotation(Rotation3d_t &rotation) {
    this->rotation = rotation;
}

void CubeNode::setRotation(float angle, float x, float y, float z) {
    Rotation3d_t rotation = {angle, x, y, z};

    this->rotation = rotation;
}

void CubeNode::setRotation(float x, float y, float z) {
    // @todo: convert the rotation vector into an angle rotation
    // rotation would be (x, 1.0f, 0.0f, 0.0f);
    // rotation would be (y, 0.0f, 1.0f, 0.0f);
    // rotation would be (z, 0.0f, 0.0f, 1.0f);
}

Coordinate3d_t &CubeNode::getScale() {
    return this->scale;
}

void CubeNode::setScale(Coordinate3d_t &scale) {
    this->scale = scale;
}

void CubeNode::setScale(float scaleX, float scaleY, float scaleZ) {
    Coordinate3d_t scale = {scaleX, scaleY, scaleZ};

    this->scale = scale;
}
