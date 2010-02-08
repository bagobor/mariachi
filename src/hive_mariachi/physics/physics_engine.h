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

#include "../nodes/physical_node.h"
#include "../nodes/collision_node.h"
#include "../main/main.h"
#include "../structures/collision.h"
#include "../structures/position.h"
#include "collision/cube_solid.h"
#include "collision/sphere_solid.h"

namespace mariachi {
    namespace physics {
        class PhysicsEngine {
            private:

            protected:
                Engine *engine;
				Coordinate3d_t gravity;

            public:
                PhysicsEngine();
                PhysicsEngine(Engine *engine);
                ~PhysicsEngine();
                virtual void load(void *arguments) {};
                virtual void unload(void *arguments) {};
                virtual void update(float delta, void *arguments) {};
                virtual std::vector<Collision3d_t> getCollisions(void *arguments) { return std::vector<Collision3d_t>(); };
                virtual void registerPhysics(PhysicalNode *physicalNode, void *arguments) {};
                virtual void registerCollision(CollisionNode *collisionNode, void *arguments) {};
				virtual void unregisterCollision(CollisionNode *collisionNode, void *arguments) {};
                virtual CubeSolid *createCubeSolid() { return NULL; };
                virtual SphereSolid *createSphereSolid() { return NULL; };
				virtual void updatePhysicalNodePosition(PhysicalNode *physicalNode, const Coordinate3d_t &position) {};
				virtual void addPhysicalNodeImpulse(PhysicalNode *physicalNode, const Coordinate3d_t &impulse, const Coordinate3d_t &relativePosition) {};
				virtual void setPhysicalNodeVelocity(PhysicalNode *physicalNode, const Coordinate3d_t &velocity) {};
				virtual const Coordinate3d_t &getGravity() { return this->gravity; };
				virtual void setGravity(const Coordinate3d_t &gravity) { };
                Engine *getEngine();
                void setEngine(Engine *engine);
        };
    }
}
