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
/*
#include "md2_importer.h"
#include "bmp_loader.h"
#include "model_viewer.h"

BmpLoader bmpLoader;
Md2Importer importer;

float xRotation = 0.0;
float yRotation = 0.0;
float zRotation = 0.0;

int window;

clock_t baseClock;
unsigned int frameCount = 0;

inline float clockSeconds() {
    // returns the clock value in seconds
    return (float) clock() / (float) CLOCKS_PER_SEC;
}

inline void frameRate() {
    // increments the frame count
    frameCount++;

    // retrieves the current clock
    float currentClock = clockSeconds();

    // calculates the delta clock
    float deltaClock = currentClock - baseClock;

    // checks if the delta clock is greater than the frame
    // sampling limit
    if(deltaClock > FRAME_SAMPLING_LIMIT) {
        // calculates the frame rate
        float frameRate = (float) frameCount / deltaClock;

        // prints the frame rate
        printf("Frame rate: %.2f\n", frameRate);

        // resets the frame count
        frameCount = 0;

        // sets the base clock as the current clock
        baseClock = currentClock;
    }
}

void display(void) {
    // calculates the frame rate
    frameRate();

    // clears all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // resets the view
    glLoadIdentity();

    // moves into the screen
    glTranslatef(0.0, 0.0, -1 * DEFAULT_ZOOM_LEVEL);

    // rotates the perspective
    glRotatef(xRotation, 1.0, 0.0, 0.0);
    glRotatef(yRotation, 0.0, 1.0, 0.0);
    glRotatef(zRotation, 0.0, 0.0, 1.0);

    glPushMatrix();

    // moves into the screen
    glTranslatef(25.0, 0.0, 1.0);

    // binds the current context to the current texture
    glBindTexture(GL_TEXTURE_2D, 1);

    // calls the list
    glCallList(1);

    glPopMatrix();

    glPushMatrix();

    // moves into the screen
    glTranslatef(-25.0, 0.0, 1.0);

    // binds the current context to the current texture
    glBindTexture(GL_TEXTURE_2D, 1);

    // calls the list
    glCallList(1);

    glPopMatrix();

    // swaps the double buffering buffers
    glutSwapBuffers();

    // sets the rotation values for the animation
    xRotation += 0.2;
    yRotation += 0.2;
    zRotation += 0.2;
}

void init(void) {
    // creates a new list
    glNewList(1, GL_COMPILE);

    // compiles the display list
    importer.compileDisplayList();

    // ends the list
    glEndList();

    // retrieves the bitmap data
    BmpColor_t *bitmapData = bmpLoader.getBitmapData();

    // retrieves the bitmap size
    BmpSize_t bitmapSize = bmpLoader.getBitmapSize();

    int textureIds[1];

    // allocation space for the texture
    glGenTextures(1, textureIds);

    // binds the current context to the current texture
    glBindTexture(GL_TEXTURE_2D, textureIds[0]);

    // sets the pixel store policy
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // loads the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmapSize.width, bitmapSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *) bitmapData);

    // sets some texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    // clears the depth
    glClearDepth(1.0);

    // sets the depth function
    glDepthFunc(GL_LEQUAL);

    // enables depth testing
    glEnable(GL_DEPTH_TEST);

    // enables the support for 2d textures
    glEnable(GL_TEXTURE_2D);

    // select clearing (background) color
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // initializes viewing values
    glMatrixMode(GL_PROJECTION);

    // loads the identity matrix
    glLoadIdentity();

    // calculates the aspect ratio of the window
    gluPerspective(45.0, (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1, 1000.0);

    // enters into model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void resizeScene(int windowWidth, int windowHeight) {
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
    gluPerspective(45.0, float(windowWidth) / float(windowHeight), 0.1, 1000.0);

    // sets the matrix mode to model view
    glMatrixMode(GL_MODELVIEW);
}

void keyPressed(unsigned char key, int x, int y) {
    // in case the escape key is pressed
    switch(key) {
        case ESCAPE_KEY:
            // destroys the window
            glutDestroyWindow(window);

            // exits the system
            exit(0);

            break;
        case F_KEY:
            // sets the window to be fullscreen
            glutFullScreen();

            break;
        case W_KEY:
            // reshapes the window
            glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

            // sets the window position
            glutPositionWindow(100, 100);

            break;
    }
}

int model_runner(int argc, char** argv) {
    // sets the base clock
    clock_t baseClock = clockSeconds();

    BmpLoader *bmpLoader2;
    bmpLoader2 = new BmpLoader();
    bmpLoader2->generateImage("models/light6.bmp");
    delete bmpLoader2;

    clock_t start, end;
    Md2Importer *importer2;
    start = clock();
    importer2 = new Md2Importer();
    importer2->generateModel("models/pallette.md2");
    importer2->generateVertexList();
    importer2->generateMeshList();
    delete importer2;
    end = clock();
    clock_t delta = end - start;
    float deltaSeconds = (float) delta / (float) CLOCKS_PER_SEC;
    printf("demorou %f\n", deltaSeconds);

    bmpLoader.generateImage("models/pallette.bmp");
    importer.generateModel("models/pallette.md2");
    importer.generateVertexList();

    // inializes the glut system
    glutInit(&argc, argv);

    // initializes the display mode
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    // initializes the window size
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // initializes the window position
    glutInitWindowPosition(100, 100);

    // creates the window
    window = glutCreateWindow("Hello World");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(resizeScene);
    glutKeyboardFunc(keyPressed);

    // runs the glut main loop
    glutMainLoop();

    // returns zero
    return 0;
}
*/
