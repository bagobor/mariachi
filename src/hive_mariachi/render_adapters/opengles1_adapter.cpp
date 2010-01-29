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

#ifdef MARIACHI_PLATFORM_OPENGLES

#include "../render_utils/opengles_uikit_window.h"
#include "definitions/opengles1_adapter_definitions.h"
#include "opengles1_adapter.h"

using namespace mariachi

float rotx = 0.0;
float roty = 0.0;
float rotz = 0.0;

/**
* Constructor of the class.
*/
Opengles1Adapter::Opengles1Adapter() : OpenglesAdapter() {
}

/**
* Destructor of the class.
*/
Opengles1Adapter::~Opengles1Adapter() {
}

void Opengles1Adapter::start(void *arguments) {
    int argc = ((int *) arguments)[0];
    char **argv = ((char ***) arguments)[1];
    RenderInformation *renderInformation = ((RenderInformation **) arguments)[2];
    RenderAdapter *renderAdapter = ((RenderAdapter **) arguments)[3];
    Engine *engine = ((Engine **) arguments)[4];

    // creates the arguments map
    std::map<std::string, void *> argumentsMap;

    // sets the arguments in the arguments map
    argumentsMap["argc"] = (void *) argc;
    argumentsMap["argv"] = (void *) argv;
    argumentsMap["render_information"] = (void *) renderInformation;
    argumentsMap["adapter"] = (void *) renderAdapter;
    argumentsMap["engine"] = (void *) engine;

    // resets the frame count
    this->frameCount = 0;

    // retrieves the current clock and sets it as
    // the base clock
    time(&this->baseClock);

    // sets the render information
    this->renderInformation = renderInformation;

    // creates the opengles window
    this->window = new DEFAULT_RENDER_OPENGLES1_WINDOW_CLASS();

    // starts the opengles window
    this->window->start(&argumentsMap);

    // starts the loop in the opengles window
    this->window->loop(&argumentsMap);

    // cleans the open gl system
    this->clean();
}

void Opengles1Adapter::stop(void *arguments) {
}

void Opengles1Adapter::init() {
    // clears the depth
    glClearDepthf(1.0);

    // sets the depth function
    glDepthFunc(GL_LEQUAL);

    // enables depth testing
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);

    // enables blending
    glEnable(GL_BLEND);

    // sets the blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enables the support for 2d textures
    glEnable(GL_TEXTURE_2D);

    // select clearing (background) color
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // initializes viewing values
    glMatrixMode(GL_PROJECTION);

    // loads the identity matrix
    glLoadIdentity();

    // enters into model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void Opengles1Adapter::clean() {
}

void Opengles1Adapter::display() {
    // updates the frame rate
    this->updateFrameRate();

    // clears all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // resets the view
    glLoadIdentity();

    // moves into the screen
    glTranslatef(0.0, 0.0, -1 * DEFAULT_ZOOM_LEVEL);

    // rotates the perspective
    glRotatef(rotx, 1.0, 0.0, 0.0);
    glRotatef(roty, 0.0, 1.0, 0.0);
    glRotatef(rotz, 0.0, 0.0, 1.0);

    // waits for the render information mutex
    MUTEX_LOCK(this->renderInformation->getMutex());

    // retrieves the render (node)
    SceneNode *render = this->renderInformation->getRender();

    std::list<Node *> renderChildrenList = render->getChildrenList();

    std::list<Node *>::iterator renderChildrenListIterator = renderChildrenList.begin();

    while(renderChildrenListIterator != renderChildrenList.end()) {
        Node *node = *renderChildrenListIterator;

        // in case the node is renderable
        if(node->renderable) {
            // casts the node as model node
            ModelNode *modelNode = (ModelNode *) node;

            // retrieves the mesh list
            std::vector<Mesh_t *> *meshList = modelNode->getMeshList();

            // retrieves the texture
            Texture *texture = modelNode->getTexture();

            // retrieves the position
            Coordinate3d_t &position = modelNode->getPosition();

            // retrieves the mesh list size
            size_t meshListSize = meshList->size();

            // sets the texture
            this->setTexture(texture);

            // pushes the transformation matrix
            glPushMatrix();

            // puts the element in the screen
            glTranslatef(position.x, position.y, position.z);

            // iterates over all the meshes
            for(unsigned int index = 0; index < meshListSize; index++) {
                // retrieves the current mesh
                Mesh_t *mesh = (*meshList)[index];

                // retrieves the position
                Coordinate3d_t position = mesh->position;

                // retrieves the vertex list
                float *vertexList = mesh->vertexList;

                // retrieves the texture vertex list
                float *textureVertexList = mesh->textureVertexList;

                // retrieves the number of vertices
                unsigned int numberVertices = mesh->numberVertices;

                // in case the number of vertices is valid
                if(numberVertices) {
                    // enables the client states
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glEnableClientState(GL_VERTEX_ARRAY);

                    // creates the vertex pointer
                    glVertexPointer(3, GL_FLOAT, 0, vertexList);

                    // creates the texture coordinate pointer
                    glTexCoordPointer(2, GL_FLOAT, 0, textureVertexList);

                    // switches over the mesh type
                    switch(mesh->type) {
                        case TRIANGLE:
                            break;
                        case TRIANGLE_STRIP:
                            glDrawArrays(GL_TRIANGLE_STRIP, 0, numberVertices);
                            break;
                        case TRIANGLE_FAN:
                            glDrawArrays(GL_TRIANGLE_FAN, 0, numberVertices);
                            break;
                    }

                    // disables the client states
                    glDisableClientState(GL_VERTEX_ARRAY);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                }
            }

            // pops the matrix
            glPopMatrix();
        }

        // increments the render children list iterator
        renderChildrenListIterator++;
    }

    rotx += 0.2;
    roty += 0.2;
    rotz += 0.2;

    // releases the render information mutex
    MUTEX_UNLOCK(this->renderInformation->getMutex());
}

void gluPerspective(float fovy, float aspect, float zNear, float zFar) {
    float xmin, xmax, ymin, ymax;

    ymax = zNear * tan(fovy * (float) M_PI / 360.0f);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

void Opengles1Adapter::resizeScene(int windowWidth, int windowHeight) {
    // prevents a division by zero in case the window is to small
    if(windowHeight == 0) {
        // sets the window height to one
        windowHeight = 1;
    }

    // resets the current viewport and perspective transformation
    glViewport(0, 0, windowWidth, windowHeight);

    // sets the matrix mode to projection
    glMatrixMode(GL_PROJECTION);

    // loads the identity matrix
    glLoadIdentity();

    // recalculates the glu perspective
    gluPerspective(45.0, float(windowWidth) / float(windowHeight), 0.3, 1000.0);

    // sets the matrix mode to model view
    glMatrixMode(GL_MODELVIEW);
}

void Opengles1Adapter::keyPressed(unsigned char key, int x, int y) {
}

inline void Opengles1Adapter::setTexture(Texture *texture) {
    // allocates the texture id integer
    GLuint textureId;

    // in case the texture is already rendered in open gl
    if(!(textureId = this->textureTextureIdMap[texture])) {
        // retrieves the texture sizes
        IntSize2d_t textureSize = texture->getSize();

        // retrieves the image buffer
        ImageColor_t *imageBuffer = texture->getImageBuffer();

        // allocation space for the texture
        glGenTextures(1, &textureId);

        // binds the current context to the current texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        // sets the pixel store policy
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        // loads the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.width, textureSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *) imageBuffer);

        // sets some texture parameters
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        // sets the texture id for the current texture in the texture
        // texture id map
        this->textureTextureIdMap[texture] = textureId;
    }
    else {
        // binds the current context to the current texture
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}

inline time_t Opengles1Adapter::clockSeconds() {
    // allocates space for the current clock
    time_t currentClock;

    // sets the current clock
    time(&currentClock);

    // returns the current clock
    return currentClock;
}

inline void Opengles1Adapter::updateFrameRate() {
    // increments the frame count
    this->frameCount++;

    // retrieves the current clock
    time_t currentClock = this->clockSeconds();

    // calculates the delta clock
    float deltaClock = (float) difftime(currentClock, this->baseClock);

    // checks if the delta clock is greater than the frame
    // sampling limit
    if(deltaClock > FRAME_SAMPLING_LIMIT) {
        // calculates the frame rate
        float frameRate = (float) frameCount / deltaClock;

        // prints the frame rate
        printf("Frame rate: %.2f\n", frameRate);

        // resets the frame count
        this->frameCount = 0;

        // sets the base clock as the current clock
        this->baseClock = currentClock;
    }
}

#endif
