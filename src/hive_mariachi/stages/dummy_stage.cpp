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

#include "../structures/structures.h"
#include "../importers/importers.h"
#include "../devices/devices.h"
#include "../user_interface/user_interface.h"

#include "dummy_stage.h"

using namespace mariachi;
using namespace mariachi::ui;
using namespace mariachi::nodes;
using namespace mariachi::stages;
using namespace mariachi::devices;
using namespace mariachi::importers;
using namespace mariachi::structures;

CameraNode *gCameraNode = NULL;

CameraNode *gCameraNode1;
CameraNode *gCameraNode2;
CameraNode *gCameraNode3;

CameraNode *gTempCameraNode;

// TESTING OVERHEAD SHOOTER CAMERA
ModelNode *gModelNode;
float posx = -25.0;
float posy = 0.0;
float posz = 0.0;

unsigned int counter = 0;

DummyStage::DummyStage() : Stage() {
}

DummyStage::DummyStage(Engine *engine) : Stage(engine) {
}

DummyStage::DummyStage(Engine *engine, const std::string &name) : Stage(engine, name) {
}

DummyStage::~DummyStage() {
}

void DummyStage::start(void *arguments) {
    Stage::start(arguments);


    // creates the importer
    Md2Importer *importer = new Md2Importer();







    // generates the model
    importer->generateModel(engine->getAbsolutePath("models/windmill.md2"));

    // generates the vertex list
    importer->generateVertexList();

    // generates the mesh list
    importer->generateMeshList();

    // generates the frame list
    importer->generateFrameList();

    // retrieves the actor node
    ActorNode *actorNode = importer->getActorNode();



/*
    // creates the importer
    Md2Importer *importer2 = new Md2Importer();

    // generates the model
    importer2->generateModel(engine->getAbsolutePath("models/light6.md2"));

    // generates the vertex list
    importer2->generateVertexList();

    // generates the mesh list
    importer2->generateMeshList();

    // retrieves the model node
    ModelNode *modelNode2 = importer2->getModelNode();*/




    BmpLoader *bmpLoader = new BmpLoader();
    bmpLoader->generateImage(engine->getAbsolutePath("models/windmill.bmp"));
    Texture *texture = bmpLoader->getTexture();

    // sets the texture in the model node
    actorNode->setTexture(texture);

 /*   BmpLoader *bmpLoader2 = new BmpLoader();
    bmpLoader2->generateImage(engine->getAbsolutePath("models/light6.bmp"));
    Texture *texture2 = bmpLoader2->getTexture();

    // sets the texture in the model node
    modelNode2->setTexture(texture2);*/

    actorNode->setPosition(0.0, 0.0, 0.0);
    //modelNode2->setPosition(-25.0, 0.0, 0.0);

    // retrieves the render
    SceneNode *render = this->engine->getRender();

    // adds the model node to the render
    render->addChild(actorNode);

    // adds the model node to the render
   // render->addChild(modelNode2);

    // prints an info message
    this->info("Dummy stage started successfully");

    // retrieves the render
    Scene2dNode *render2d = this->engine->getRender2d();



    BmpLoader *bmpLoader4 = new BmpLoader();
    bmpLoader4->generateImage(engine->getAbsolutePath("ui/background.bmp"));
    Texture *backgroundTexture = bmpLoader4->getTexture();

    // creates a new view port node
    ViewPortNode *viewPort2Node = new ViewPortNode();

    // sets the viewport node position
    viewPort2Node->setPosition(0.0f, 0.0f);

    // sets the size of the viewport node
    viewPort2Node->setSize((float) 100.0f, (float) 100.0f);

    viewPort2Node->setTexture(backgroundTexture);

    // adds the view port node to the render
    render2d->addChild(viewPort2Node);







/*    BmpLoader *bmpLoader34 = new BmpLoader();
    bmpLoader34->generateImage(engine->getAbsolutePath("ui/white.bmp"));
    Texture *panelTexture = bmpLoader34->getTexture();

    // creates a new panel node
    PanelNode *panelNode = new PanelNode();

    panelNode->setPositionReference(CENTER_REFERENCE_POSITION);

    // sets the viewport node position
    panelNode->setPosition(50.0f, 50.0f);

    panelNode->setSize(60.25f, 30.00f);

    panelNode->setTexture(panelTexture);

    viewPort2Node->addChild(panelNode);








    BmpLoader *bmpLoader33 = new BmpLoader();
    bmpLoader33->generateImage(engine->getAbsolutePath("ui/logo.bmp"));
    Texture *logoTexture = bmpLoader33->getTexture();

    // creates a new button node
    ButtonNode *imageNode = new ButtonNode();

    // sets the button node position
    imageNode->setPosition(3.13f, 2.50f);

    // sets the size of the image node
    imageNode->setSize(31.40f, 14.06f);

    // sets the texture
    imageNode->setTexture(logoTexture);

    // adds the view port node to the render
    panelNode->addChild(imageNode);



    BmpLoader *bmpLoader3 = new BmpLoader();
    bmpLoader3->generateImage(engine->getAbsolutePath("ui/about.bmp"));
    Texture *buttonTexture = bmpLoader3->getTexture();

    // creates a new button node
    ButtonNode *buttonNode = new ButtonNode();

    buttonNode->setPositionReference(CENTER_REFERENCE_POSITION);

    // sets the viewport node position
    buttonNode->setPosition(50.0, 30.0);

    // sets the size of the viewport node
    buttonNode->setSize((float) 21.25, (float) 10.94);

    // sets the texture in the button
    buttonNode->setTexture(buttonTexture);

    // adds the view port node to the render
    panelNode->addChild(buttonNode);*/

    /*PacketNetwork *packetNetwork = new PacketNetwork();

    std::map<std::string, void *> argumentsMap;

    argumentsMap["socketType"] = (void *) SOCKET_INTERNET_TYPE;

    packetNetwork->start(&argumentsMap);
    packetNetwork->bindConnection("", 8989);*/








    gCameraNode1 = new CameraNode();
    gCameraNode2 = new CameraNode();
    gCameraNode3 = new CameraNode();

    gTempCameraNode = new CameraNode();

    gCameraNode = gCameraNode3;

    //modelNode->setPosition(25.0f, 0.0f, 0.0f);
  //  modelNode2->setPosition(-25.0f, 0.0f, 0.0f);

    Coordinate3d zeroVector = { 0.0f, 0.0f, 0.0f };

    gCameraNode1->setPosition(-10.0f, 0.0f, 0.0f);
    gCameraNode2->setPosition(-1.0f, 0.0f, 0.0f);
    gCameraNode3->setPosition(0.0f, 0.0f, 100.0f);

    gCameraNode1->setRotation(0.0f, 0.0f, 0.0f, 0.0f);
    gCameraNode2->setRotation(0.0f, 0.0f, 0.0f, 0.0f);
    gCameraNode3->setRotation(0.0f, 0.0f, 0.0f, 0.0f);

    gCameraNode1->enableAutoTracking(actorNode, zeroVector);
    gCameraNode2->enableAutoTracking(actorNode, zeroVector);
    gCameraNode3->enableAutoTracking(actorNode, zeroVector);
    gTempCameraNode->enableAutoTracking(actorNode, zeroVector);

    gCameraNode1->setUpVector(0.0, 0.0, 1.0);
    gCameraNode2->setUpVector(0.0, 0.0, 1.0);
    gCameraNode3->setUpVector(0.0, 0.0, 1.0);

    // TESTING OVERHEAD SHOOTER CAMERA
    gModelNode = actorNode;
    Coordinate3d cameraNode3Offset = { -2.0f, -8.0f, 8.0f };
    gCameraNode3->enableAutoFollowing(actorNode, cameraNode3Offset);
    //gCameraNode2->enableAutoFollowing(modelNode, cameraNode3Offset);
}

void DummyStage::stop(void *arguments) {
    Stage::stop(arguments);
}

void DummyStage::update(void *arguments) {
    Stage::update(arguments);

    // updates the frame
    ((ActorNode *)gModelNode)->updateFrame();

    gCameraNode1->_autoTrack();
    gCameraNode2->_autoTrack();
    gCameraNode3->_autoTrack();

    gCameraNode1->_autoFollow();
   // gCameraNode2->_autoFollow();
    gCameraNode3->_autoFollow();

    gCameraNode2->setPosition(posx, posy, posz);

    // TESTING OVERHEAD SHOOTER CAMERA
    posx += 5.0f;

    gModelNode->setRotation(posx, 1.0, 1.0, 1.0);

    std::list<InputDeviceEvent_t> *inputEventQueue;

    // retrieves the keyboard
    Keyboard *keyboard = (Keyboard *) this->engine->getDevice("keyboard");

    // retrieves the input event queue
    inputEventQueue = keyboard->getInputEventQueue();

    // while the size of the input event queue
    // is valid
    while(inputEventQueue->size()) {
        // retrieves the front of the queue
        InputDeviceEvent_t &inputEvent = inputEventQueue->front();

        // prints a debug message
        std::cout << "Key pressed (keycode: " << ((KeyboardEvent_t *) inputEvent.event)->keyCode << ")\n";

        if(((KeyboardEvent_t *) inputEvent.event)->keyCode == KEY_CODE_C) {
            unsigned int value = counter % 3;

            switch(value) {
                case 0:
                    gCameraNode = gCameraNode1;
                    break;

                case 1:
                    gCameraNode = gCameraNode2;
                    break;

                case 2:
                    gCameraNode = gCameraNode3;
                    break;
            }

            // increments the coutner
            counter++;
        }

        // pops the front
        inputEventQueue->pop_front();
    }

    // retrieves the keyboard
    Mouse *mouse = (Mouse *) this->engine->getDevice("mouse");

    // retrieves the input event queue
    inputEventQueue = mouse->getInputEventQueue();

    // while the size of the input event queue
    // is valid
    while(inputEventQueue->size()) {
        // retrieves the front of the queue
        InputDeviceEvent_t &inputEvent = inputEventQueue->front();

        MouseEvent_t *mouseEvent = (MouseEvent_t *) inputEvent.event;

        // prints a debug message
        std::cout << "Mouse button pressed (button: " << mouseEvent->buttonCode << " & position x: " << mouseEvent->position.mousePositionX << " y: " << mouseEvent->position.mousePositionY << ")\n";

        // pops the front
        inputEventQueue->pop_front();
    }
}
