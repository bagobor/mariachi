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
    this->collisionConfiguration = new btDefaultCollisionConfiguration();

    // creates the default collision dispatcher
    this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // creates a broad phase as a general purpose broadphase
    this->broadPhase = new btDbvtBroadphase();

    // creates a default constraint solver
    this->solver = new btSequentialImpulseConstraintSolver;

    // creates the dynamics world
    this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);

    // sets the gravity in the dynamic world (default gravity)
    this->dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, -10.0f));
}

void BulletPhysicsEngine::unload(void *arguments) {
    // retrieves the physical node rigid body map iterator
    std::map<CubeNode *, btRigidBody *>::iterator physicalNodeRigidBodyMapIterator = this->physicalNodeRigidBodyMap.begin();

    // iterates over all the current rigid bodies
    while(physicalNodeRigidBodyMapIterator != this->physicalNodeRigidBodyMap.end()) {
        // retrieves the current rigid body
        btRigidBody *rigidBody = physicalNodeRigidBodyMapIterator->second;

        // removes the rigid body from the dynamics world
        this->dynamicsWorld->removeRigidBody(rigidBody);

        // deletes the rigid body motion state
        delete rigidBody->getMotionState();

        // deletes the rigid body
        delete rigidBody;

        // increments the physical node rigid body map iterator
        physicalNodeRigidBodyMapIterator++;
    }

    // clears the physical node rigid body map
    this->physicalNodeRigidBodyMap.clear();

    // deletes the dynamics world
    delete this->dynamicsWorld;

    // deletes the solver
    delete this->solver;

    // deletes the collision configuration
    delete this->collisionConfiguration;

    // deletes the dispatcher
    delete this->dispatcher;

    // deletes the borad phase
    delete this->broadPhase;
}

void BulletPhysicsEngine::update(float delta, void *arguments) {
    // the default number of sub steps (complexity)
    int maximumSubSteps = 1;

    // in case the arguments are valid
    if(arguments) {
        // retrieves the arguments map from the arguments
        std::map<std::string, void *> argumentsMap = *(std::map<std::string, void *> *) arguments;

        // retrieves the maximum sub steps
        maximumSubSteps = (int) argumentsMap["maximum_sub_steps"];
    }

    // runs a simulation step
    this->dynamicsWorld->stepSimulation(delta, maximumSubSteps);
}

std::vector<int> BulletPhysicsEngine::getCollisions(void *arguments) {
    // retrieves the number of manifolds
    int numberManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();

    // iterates over all the contact manifolds
    for(int index = 0; index < numberManifolds; index++) {
        // retrieves the current manifold
        btPersistentManifold *contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(index);

        // retrieves the collision objects
        btCollisionObject *firstCollisionObject = (btRigidBody *) contactManifold->getBody0();
        btCollisionObject *secondCollisionObject = (btCollisionObject *) contactManifold->getBody1();

        // retrieves the number of contacts
        int numberContacts = contactManifold->getNumContacts();

        // interates over all the contacts
        for(int _index = 0; _index < numberContacts; _index++) {
            // retrieves the current contact point
            btManifoldPoint &contactPoint = contactManifold->getContactPoint(_index);

            // in case the contact point distance is valid
            if(contactPoint.getDistance() < 0.0f) {
                // retrieves the contact point in both objects
                const btVector3 &contactPointFirstCollisionObject = contactPoint.getPositionWorldOnA();
                const btVector3 &contactPointSecondCollisionObject = contactPoint.getPositionWorldOnB();

                // retrieves the normal vectors in both objects
                const btVector3 &contactPointNormalScondCollisionObject = contactPoint.m_normalWorldOnB;
            }
        }
    }

    return std::vector<int>();
}

void BulletPhysicsEngine::registerPhysics(PhysicalNode *physicalNode, void *arguments) {
    // retrieves the rigid body
    this->getRigidBody(physicalNode);
}

void BulletPhysicsEngine::registerCollision(CollisionNode *collisionNode, void *arguments) {
    // retrieves the physical node
    PhysicalNode *physicalNode = (PhysicalNode *) collisionNode->getParent();

    // retrieves the rigid body
    this->getRigidBody(physicalNode, collisionNode);
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
