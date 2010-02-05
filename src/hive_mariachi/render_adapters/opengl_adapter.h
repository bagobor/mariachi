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

#ifdef MARIACHI_PLATFORM_OPENGL

#ifdef __MACH__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "../nodes/nodes.h"
#include "../user_interface/user_interface.h"
#include "../structures/texture.h"
#include "../structures/size.h"
#include "../render_utils/opengl_window.h"
#include "render_adapter.h"

/**
* The default zoom level value.
*/
#define DEFAULT_ZOOM_LEVEL 100.0

namespace mariachi {
    class OpenglAdapter : public RenderAdapter {
        private:
            OpenglWindow *window;
            IntSize2d_t windowSize;
            float windowAspectRatio;
            float widthRatio;
            float heightRatio;
            float lowestRatio;
            float highestRatio;
            float bestRatio;
            float lowestWidthRevertRatio;
            float lowestHeightRevertRatio;
            std::map<Texture *, int> textureTextureIdMap;

            inline time_t clockSeconds();
            inline void updateFrameRate();
            inline void display2d();
            inline void display3d();
            inline void setupDisplay2d();
            inline void setupDisplay3d();
            inline void renderCameraNode(CameraNode *cameraNode);
            inline void renderNode2d(Node *node);
            inline void renderSquare(float x1, float y1, float x2, float y2);
            inline void renderModelNode(ModelNode *modelNode);
            inline void renderViewPortNode(ui::ViewPortNode *viewPortNode, SquareNode *targetNode);
            inline void renderPanelNode(ui::PanelNode *panelNode, SquareNode *targetNode);
            inline void renderButtonNode(ui::ButtonNode *buttonNode, SquareNode *targetNode);
            inline Coordinate2d_t getRealPosition2d(SquareNode *squareNode, SquareNode *targetNode);
            inline FloatSize2d_t getRealSize2d(SquareNode *squareNode);

        public:
            OpenglAdapter();
            ~OpenglAdapter();
            void start(void *arguments);
            void stop(void *arguments);
            void init();
            void clean();
            void display();
            void resizeScene(int windowWidth, int windowHeight);
            void keyPressed(unsigned char key, int x, int y);
            inline void setTexture(Texture *texture);
    };
}

#endif
