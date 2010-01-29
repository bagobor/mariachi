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

// __author__    = Luís Martinho <lmartinho@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#include "../structures/position.h"

namespace mariachi {
    class VectorUtil {
        private:

        public:
            static Coordinate3d_t normalize(const Coordinate3d_t &vector);
            static Coordinate3d_t crossProduct(const Coordinate3d_t &vector1, const Coordinate3d_t &vector2);
            static Coordinate3d_t add(const Coordinate3d_t &vector1, const Coordinate3d_t &vector2);
            static Coordinate3d_t subtract(const Coordinate3d_t &vector1, const Coordinate3d_t &vector2);
    };
};
