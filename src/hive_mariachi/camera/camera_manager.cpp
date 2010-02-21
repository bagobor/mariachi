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

#include "camera_manager.h"

using namespace mariachi;
using namespace mariachi::nodes;
using namespace mariachi::camera;

/**
* Constructor of the class.
*/
CameraManager::CameraManager() {
    this->initDefaultCamera();
}

CameraManager::CameraManager(Engine *engine) {
    this->initDefaultCamera();
    this->initEngine(engine);
}

/**
* Destructor of the class.
*/
CameraManager::~CameraManager() {
}

/**
* Initializes the default camera.
*/
inline void CameraManager::initDefaultCamera() {
    // creates the default camera
    this->defaultCamera = new CameraNode(std::string("default_camera"));

    // sets the default camera as the current active camera
    this->activeCamera = this->defaultCamera;
}

inline void CameraManager::initEngine(Engine *engine) {
    this->engine = engine;
}

void CameraManager::load(void *arguments) {
}

void CameraManager::unload(void *arguments) {
}

void CameraManager::update(void *arguments) {
    this->activeCamera->_autoFollow();
    this->activeCamera->_autoTrack();
}

/**
* Retrieves the default camera.
*
* @return The default camera.
*/
CameraNode *CameraManager::getDefaultCamera() {
    return this->defaultCamera;
}

/**
* Sets the default camera.
*
* @param defaultCamera The default camera.
*/
void CameraManager::setDefaultCamera(CameraNode *defaultCamera) {
    this->defaultCamera = defaultCamera;
}

/**
* Retrieves the current active camera.
*
* @return The current active camera.
*/
CameraNode *CameraManager::getActiveCamera() {
    return this->activeCamera;
}

/**
* Sets the current active camera.
*
* @param activeCamera The current active camera.
*/
void CameraManager::setActiveCamera(CameraNode *activeCamera) {
    this->activeCamera = activeCamera;
}

/**
* Retrieves the camera for the given camera name.
*
* @param cameraName The camera name to retrieve the camera.
* @return The camera for the given camera name.
*/
CameraNode *CameraManager::getCamera(const std::string &cameraName) {
    return this->camerasMap[cameraName];
}

/**
* Sets the camera with the given camera name.
*
* @param cameraName The name to be used to identify the camera.
* @param camera The camera to be set.
*/
void CameraManager::setCamera(const std::string &cameraName, CameraNode *camera) {
    this->camerasMap[cameraName] = camera;
}
