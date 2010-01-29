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

#include "cube_node_motion_state.h"

using namespace mariachi;
using namespace mariachi::physics;

CubeNodeMotionState::CubeNodeMotionState(const btTransform &worldTransform) {
    this->initWorldTransform(worldTransform);
}

CubeNodeMotionState::CubeNodeMotionState(const btTransform &worldTransform, CubeNode *cubeNode) {
    this->initWorldTransform(worldTransform);
    this->initCubeNode(cubeNode);
}

CubeNodeMotionState::~CubeNodeMotionState() {
}

inline void CubeNodeMotionState::initWorldTransform(const btTransform &worldTransform) {
    this->worldTransform = worldTransform;
}

inline void CubeNodeMotionState::initCubeNode(CubeNode *cubeNode) {
    this->cubeNode = cubeNode;
}

void CubeNodeMotionState::getWorldTransform(btTransform &worldTransform) {
    worldTransform = this->worldTransform;
}

void CubeNodeMotionState::setWorldTransform(const btTransform &worldTransform) {
    // returns silently if node not set
    if(this->cubeNode == NULL) {
        return;
    }

    // retrieves the bullet position
    btVector3 positionBullet = worldTransform.getOrigin();

    // retrieves the bullet rotation
    btQuaternion rotationBullet = worldTransform.getRotation();

    // creates the position from the bullet position
    Coordinate3d_t position = { positionBullet.x(), positionBullet.y(), positionBullet.z() };

    // retrieves the rotation angle
    float angle = 2.0f * acos(rotationBullet.w()) * 180.0f / (float) M_PI;

    // retrieves the rotation axis vector
    float norm = sqrt(pow(rotationBullet.x(), 2.0f) + pow(rotationBullet.y(), 2.0f) + pow(rotationBullet.z(), 2.0f));
    float normalizedX = rotationBullet.x() / norm;
    float normalizedY = rotationBullet.y() / norm;
    float normalizedZ = rotationBullet.z() / norm;

    // creates the rotation for the orientation
    Rotation3d_t orientation = { angle, normalizedX, normalizedY, normalizedZ };

    // sets the position in the cube node
    this->cubeNode->setPosition(position);

    // sets the orientation in the cube node
    this->cubeNode->setOrientation(orientation);
}

CubeNode *CubeNodeMotionState::getCubeNode() {
    return this->cubeNode;
}

void CubeNodeMotionState::setCubeNode(CubeNode *cubeNode) {
    this->cubeNode = cubeNode;
}
