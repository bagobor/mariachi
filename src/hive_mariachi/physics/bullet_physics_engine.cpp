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

#include "bullet_physics_engine.h"

using namespace mariachi;
using namespace mariachi::physics;

BulletPhysicsEngine::BulletPhysicsEngine() : PhysicsEngine() {
}

BulletPhysicsEngine::BulletPhysicsEngine(Engine *engine) : PhysicsEngine(engine) {
}

BulletPhysicsEngine::~BulletPhysicsEngine() {
}

void BulletPhysicsEngine::load(void *arguments) {
    // creates the collision configuration (contains default setup for memory collision setup)
    btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();

    // creates the default collision dispatcher
    btCollisionDispatcher *dispatcher = new    btCollisionDispatcher(collisionConfiguration);

    // creates a broad phase as a general purpose broadphase
    btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();

    // creates a default constraint solver
    btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;

    // creates the dynamics world
    this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    // sets the gravity in the dynamic world (default gravity)
    this->dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, -10.0f));
}

void BulletPhysicsEngine::unload(void *arguments) {
}
