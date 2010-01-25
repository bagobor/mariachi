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

#include "../importers/importers.h"
#include "../devices/devices.h"
#include "dummy_stage.h"

using namespace mariachi;

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
    importer->generateModel(std::string(HIVE_MARIACHI_BASE_PATH) + std::string("/models/pallette.md2"));

    // generates the vertex list
    importer->generateVertexList();

    // generates the mesh list
    importer->generateMeshList();

    // retrieves the model node
    ModelNode *modelNode = importer->getModelNode();

    // creates the importer
    Md2Importer *importer2 = new Md2Importer();

    // generates the model
    importer2->generateModel(std::string(HIVE_MARIACHI_BASE_PATH) + std::string("/models/light6.md2"));

    // generates the vertex list
    importer2->generateVertexList();

    // generates the mesh list
    importer2->generateMeshList();

    // retrieves the model node
    ModelNode *modelNode2 = importer2->getModelNode();

    BmpLoader * bmpLoader = new BmpLoader();
    bmpLoader->generateImage(std::string(HIVE_MARIACHI_BASE_PATH) + std::string("/models/pallette.bmp"));
    Texture *texture = bmpLoader->getTexture();

    // sets the texture in the model node
    modelNode->setTexture(texture);

    BmpLoader * bmpLoader2 = new BmpLoader();
    bmpLoader2->generateImage(std::string(HIVE_MARIACHI_BASE_PATH) + std::string("/models/light6.bmp"));
    Texture *texture2 = bmpLoader2->getTexture();

    // sets the texture in the model node
    modelNode2->setTexture(texture2);

    modelNode->setPosition(25.0, 0.0, 0.0);
    modelNode2->setPosition(-25.0, 0.0, 0.0);

    // retrieves the render
    Node *render = this->engine->getRender();

    // adds the model node to the render
    render->addChild(modelNode);

    // adds the model node to the render
    render->addChild(modelNode2);

    // prints an info message
    this->info("Dummy stage started successfully");

    PacketNetwork *packetNetwork = new PacketNetwork();

    std::map<std::string, void *> argumentsMap;

    argumentsMap["socketType"] = (void *) SOCKET_INTERNET_TYPE;

    packetNetwork->start(&argumentsMap);

    packetNetwork->bindConnection("", 8989);
}

void DummyStage::stop(void *arguments) {
    Stage::stop(arguments);
}

void DummyStage::update(void *arguments) {
    Stage::update(arguments);

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
