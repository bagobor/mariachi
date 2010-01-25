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

#pragma once

#include "../structures/mesh.h"
#include "model_importer.h"

/**
* The md2 header size.
*/
#define MD2_HEADER_SIZE 68

/**
* The md2 frame header size.
*/
#define MD2_FRAME_HEADER_SIZE 40

/**
* The md2 float size size.
*/
#define MD2_FLOAT_SIZE 4

/**
* The md2 vertex value size.
*/
#define MD2_VERTEX_VALUE_SIZE 4

/**
* The md2 coordinate number.
*/
#define MD2_COORDINATE_NUMBER 3

namespace mariachi {
    typedef struct Md2Header_t {
        char magicNumber[4];
        int md2Version;
        int textureWidth;
        int textureHeight;
        int frameSize;
        int numberTextures;
        int numberVertices;
        int numberTextureCordinates;
        int numberTriangles;
        int numberGlCommands;
        int numberFrames;
        int offsetSkins;
        int offsetSt;
        int offsetTriangles;
        int offsetFrames;
        int offsetGlCommands;
        int offsetEnd;
    } Md2Header;

    typedef struct Md2FrameHeader_t {
        float scale[3];
        float translate[3];
        char name[16];
    } Md2FrameHeader;

    typedef struct Md2VertexContents_t {
        unsigned char vertex[3];
        unsigned char lightnormalindex;
    } Md2VertexContents;

    typedef struct Md2VertexTextureInformation_t {
        float vertexTextureX;
        float vertexTextureY;
        int vertexIndex;
    } Md2VertexTextureInformation;

    /**
    * The md2 importer class.
    * Allows the importing to the mariachi engine of the quake 2 models.
    *
    * @see: wikipedia - http://en.wikipedia.org/wiki/MD2_(file_format)
    */
    class Md2Importer : public ModelImporter {
        private:
            std::vector<float> coordinatesList;
            std::vector<float *> vertexList;
            std::vector<void *> glCommandsList;
            std::vector<Mesh_t *> meshList;
            int meshCount;

        public:
            Md2Importer();
            ~Md2Importer();
            void generateModel(const std::string &filePath);
            void generateVertexList();
            void generateMeshList();
            void countMeshes();
            void compileDisplayList();
            ModelNode *getModelNode();
            void cleanModel();
            void cleanVertextList();
            void cleanMeshList();
    };
}
