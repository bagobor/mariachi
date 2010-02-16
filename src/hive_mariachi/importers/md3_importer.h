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

#include "model_importer.h"

namespace mariachi {
    namespace importers {
        /**
        * The md2 header structure, containing information about
        * md2 metadata.
        *
        * @param magicNumber The md2 magic number (IDP2).
        * @param md2Version The version of the md2 implementation.
        * @param textureWidth The width of the texture.
        * @param textureHeight The height of the texture.
        * @param frameSize The size of a frame (in bytes).
        * @param numberTextures The number of textures.
        * @param numberVertices The number of vertices.
        * @param numberTextureCordinates The number of texture coordinates.
        * @param numberTriangles The number of triangles.
        * @param numberGlCommands The number of gl commands.
        * @param numberFrames The number of frames included in the model.
        * @param offsetSkins The offset address to the skins.
        * @param offsetSt The offset address to the texture coordinates.
        * @param offsetTriangles The offset address to the trinagles.
        * @param offsetFrames The offset address to the frames.
        * @param offsetGlCommands The offset address to the gl commands.
        * @param offsetEnd The offset address to the end.
        */
        typedef struct Md3Header_t {
            char magicNumber[4];
            int md3Version;
            char name[64];
            int flags;
            int numberFrames;
            int numberTags;
            int numberSurfaces;
            int numberSkins;



          /*  int textureWidth;
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
            int offsetEnd;*/
        } Md3Header;

/*
S32    NUM_FRAMES    Number of Frame objects, with a maximum of MD3_MAX_FRAMES. Current value of MD3_MAX_FRAMES is 1024.
S32    NUM_TAGS    Number of Tag objects, with a maximum of MD3_MAX_TAGS. Current value of MD3_MAX_TAGS is 16. There is one set of tags per frame so the total number of tags to read is (NUM_TAGS * NUM_FRAMES).
S32    NUM_SURFACES    Number of Surface objects, with a maximum of MD3_MAX_SURFACES. Current value of MD3_MAX_SURFACES is 32.
S32    NUM_SKINS    Number of Skin objects. I should note that I have not seen an MD3 using this particular field for anything; this appears to be an artifact from the Quake 2 MD2 format. Surface objects have their own Shader field.
S32    OFS_FRAMES    Relative offset from start of MD3 object where Frame objects start. The Frame objects are written sequentially, that is, when you read one Frame object, you do not need to seek() for the next object.
S32    OFS_TAGS    Relative offset from start of MD3 where Tag objects start. Similarly written sequentially.
S32    OFS_SURFACES    Relative offset from start of MD3 where Surface objects start. Again, written sequentially.
S32    OFS_EOF    Relative offset from start of MD3 to the end of the MD3 object. Note there is no offset for Skin objects.
 !    (Frame)    The array of Frame objects usually starts immediately afterwards, but OFS_FRAMES should be used.
 !    (Tag)    The array of Tag objects usually starts immediately after FRAMES, but OFS_TAGS should be used.
 !    (Surface)    The array of Surface objects usually start after TAGS, but OFS_SURFACES should be used.
-    MD3_END    End of MD3 object. Should match MD3_START.
*/


        /**
        * Class that represents an md3 (quake 3) frame, the contents
        * include coordinates list, vertex list and mesh list.
        *
        * @see wikipedia - http://en.wikipedia.org/wiki/MD3_(file_format)
        */
        class Md3Importer : public ModelImporter {
            private:

            public:
                Md3Importer();
                ~Md3Importer();
        };
    }
}
