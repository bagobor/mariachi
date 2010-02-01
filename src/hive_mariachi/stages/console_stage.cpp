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

#include "console_stage.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
ConsoleStage::ConsoleStage() : Stage() {
    this->initThread();
}

/**
* Constructor of the class.
*
* @param engine The currently used engine.
*/
ConsoleStage::ConsoleStage(Engine *engine) : Stage(engine) {
    this->initThread();
}

/**
* Constructor of the class.
*
* @param engine The currently used engine.
* @param name The name of the stage.
*/
ConsoleStage::ConsoleStage(Engine *engine, const std::string &name) : Stage(engine, name) {
    this->initThread();
}

/**
* Destructor of the class.
*/
ConsoleStage::~ConsoleStage() {
}

inline void ConsoleStage::initThread() {
    this->thread = true;
}

void ConsoleStage::start(void *arguments) {
    Stage::start(arguments);

    // sets the running flag
    this->runningFlag = true;

    // creates the string buffer
    std::string stringBuffer;

    // iterates while the running flag is active
    while(this->runningFlag) {
        // prints the carret
        std::cout << CONSOLE_MANAGER_CARRET << " ";

        // retrieves the current line from standard input
        getline(std::cin, stringBuffer);

        this->engine->getConsoleManager()->processCommandLine(stringBuffer.c_str());
    }
}

void ConsoleStage::stop(void *arguments) {
    Stage::stop(arguments);

    // unsets the running flag
    this->runningFlag = false;
}

void ConsoleStage::update(void *arguments) {
    Stage::update(arguments);
}
