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

// __author__    = Jo�o Magalh�es <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#include "../../../lib/libbullet/src/btBulletDynamicsCommon.h"
#include "../nodes/physical_node.h"

#include "physics_engine.h"

namespace mariachi {
    namespace physics {
        class BulletPhysicsEngine : public PhysicsEngine {
            private:
                btDefaultCollisionConfiguration *collisionConfiguration;

                btCollisionDispatcher *dispatcher;

                btBroadphaseInterface *broadPhase;

                /**
                * The bullent engine impulse constraint solver.
                */
                btSequentialImpulseConstraintSolver *solver;

                /**
                * The bullet engine world representation object.
                */
                btDiscreteDynamicsWorld *dynamicsWorld;

                /**
                * The map associating the physical nodes with the bullet engine
                * rigid body structures.
                */
                std::map<CubeNode *, btRigidBody *> physicalNodeRigidBodyMap;

                btRigidBody *getRigidBody(PhysicalNode *physicalNode, CollisionNode *collisionNode = NULL);

            public:
                BulletPhysicsEngine();
                BulletPhysicsEngine(Engine *engine);
                ~BulletPhysicsEngine();
                void load(void *arguments);
                void unload(void *arguments);
                void update(float delta, void *arguments);
                std::vector<int> getCollisions(void *arguments);
                void registerPhysics(PhysicalNode *physicalNode, void *arguments);
                void registerCollision(CollisionNode *collisionNode, void *arguments);
                CubeSolid *createCubeSolid();
                SphereSolid *createSphereSolid();
        };
    }
}