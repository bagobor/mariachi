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

#include "bullet_physics_engine/_bullet_physics_engine.h"

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
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);

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

void BulletPhysicsEngine::registerCollision(CollisionNode *collisionNode, void *arguments) {

    // retrieves the physical node
    PhysicalNode *physicalNode = (PhysicalNode *) collisionNode->getParent();




    this->getRigidBody(physicalNode, collisionNode);


    //this->dynamicsWorld
}

CubeSolid *BulletPhysicsEngine::createCubeSolid() {
    return new BulletPhysicsEngineCubeSolid();
}

SphereSolid *BulletPhysicsEngine::createSphereSolid() {
    return new BulletPhysicsEngineSphereSolid();
}

btRigidBody *BulletPhysicsEngine::getRigidBody(PhysicalNode *physicalNode, CollisionNode *collisionNode) {
    // the physical node rigid body reference
    btRigidBody *physicalNodeRigidBody;

    if(physicalNodeRigidBody = this->physicalNodeRigidBodyMap[physicalNode]) {
        return physicalNodeRigidBody;
    }

    // the collision shape reference
    btCollisionShape *collisionShape;

    // in case a collision node is defined
    if(collisionNode) {
        // retrieves the bulled physics collision solid
        BulletPhysicsEngineCollisionSolid *bulletPhysicsEngineCollisionSolid = (BulletPhysicsEngineCollisionSolid *) collisionNode->getCollisionSolid();

        // retrieves the bullet collision shape
        collisionShape = bulletPhysicsEngineCollisionSolid->getCollisionShape();
    } else {
        // sets the collision shape as null (invalid)
        collisionShape = NULL;
    }

    // retrieves the physical node position
    Coordinate3d_t &physicalNodePosition = physicalNode->getPosition();

    // converts the physical node position
    btVector3 physicalNodePositionVector = btVector3(physicalNodePosition.x, physicalNodePosition.y, physicalNodePosition.z);

    // initializes the physical node transform
    btTransform physicalNodeTransform;
    physicalNodeTransform.setIdentity();
    physicalNodeTransform.setOrigin(physicalNodePositionVector);

    // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    PhysicalNodeMotionState *physicalNodeMotionState = new PhysicalNodeMotionState(physicalNodeTransform, physicalNode);

    // retrieves the physical node mass
    float physicalNodeMass = physicalNode->getMass();

    // retrieves the physical node inertia
    Coordinate3d_t &physicalNodeInertia = physicalNode->getInertia();

    // converts the physical node inertia
    btVector3 physicalNodeInertiaVector = btVector3(physicalNodeInertia.x, physicalNodeInertia.y, physicalNodeInertia.z);

    btRigidBody::btRigidBodyConstructionInfo physicalNodeRigidBodyInfo(physicalNodeMass, physicalNodeMotionState, collisionShape, physicalNodeInertiaVector);

    // creates the physical node rigid body
    physicalNodeRigidBody = new btRigidBody(physicalNodeRigidBodyInfo);

    // sets the physical node rigid body in the physical node rigid body map
    this->physicalNodeRigidBodyMap[physicalNode] = physicalNodeRigidBody;

    // returns the physical node rigid body
    return physicalNodeRigidBody;
}
