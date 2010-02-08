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

#include "../structures/size.h"
#include "../structures/position.h"

#include "node.h"

typedef enum PositionReferenceType_t {
    TOP_LEFT_REFERENCE_POSITION = 1,
    CENTER_REFERENCE_POSITION
} PositionReferenceType;

namespace mariachi {
    class SquareNode : public Node {
        private:
            Coordinate2d_t position;
            Coordinate2d_t rotation;
            FloatSize2d_t size;
            Coordinate2d_t topLeftPosition;
            bool topLeftPositionDirty;
            PositionReferenceType_t positionReference;

            inline void initPositionReference();

        public:
            SquareNode();
            SquareNode(const std::string &name);
            ~SquareNode();
            Coordinate2d_t &getTopLeftPosition();
            Coordinate2d_t &getPosition();
            void setPosition(Coordinate2d_t &position);
            void setPosition(float x, float y);
            FloatSize2d_t &getSize();
            void setSize(FloatSize2d_t &size);
            void setSize(float width, float height);
            PositionReferenceType_t getPositionReference();
            void setPositionReference(PositionReferenceType_t positionReference);
            virtual inline unsigned int getNodeType() { return SQUARE_NODE_TYPE; };
    };
}
