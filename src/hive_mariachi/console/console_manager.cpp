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

// __author__    = Jo�o Magalh�es <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "../util/string_util.h"

#include "console_manager.h"

using namespace mariachi;
using namespace mariachi::util;

/**
* Constructor of the class.
*/
ConsoleManager::ConsoleManager() {
    this->initProcessInformationMap();
    this->initCurrentScriptEngine();
}

/**
* Constructor of the class.
*
* @param engine The cur
*/
ConsoleManager::ConsoleManager(Engine *engine) {
    this->initProcessInformationMap();
    this->initCurrentScriptEngine();
    this->initEngine(engine);
}

/**
* Destructor of the class.
*/
ConsoleManager::~ConsoleManager() {
}

/**
* Initializes the process information map.
*/
inline void ConsoleManager::initProcessInformationMap() {
    // iterates over the process information list
    for(unsigned int index = 0; ; index++) {
        // retrieves the current process information
        CommandProcessInformation_t currentProcessInformation = processInformationList[index];

        // in case the current process information is invalid
        if(currentProcessInformation.name == NULL) {
            break;
        }

        // sets the current process information in the process information map
        this->processInformationMap[currentProcessInformation.name] = currentProcessInformation;
    }
}

/**
* Initializes the current script engine.
*/
inline void ConsoleManager::initCurrentScriptEngine() {
    this->currentScriptEngine = NULL;
}

/**
* Initializes the engine.
*/
inline void ConsoleManager::initEngine(Engine *engine) {
    this->engine = engine;
}

void ConsoleManager::load(void *arguments) {
}

void ConsoleManager::unload(void *arguments) {
}

/**
* Processes the given command line, sending the output to the
* write function or to the standard output in case none is given.
*
* @param commandLine The command line to be processed.
* @param outputFunction The write function to receive the processing result.
*/
void ConsoleManager::processCommandLine(const char *commandLine, WriteOuputFunction_t outputFunction) {
    // in case the command line is empty
    if(!strcmp(commandLine, "")) {
        // returns immediately
        return;
    }

    // allocates space for the tokens vector
    std::vector<std::string> commandTokens;

    // tokenizes the command line
    StringUtil::tokenize(commandLine, commandTokens, " ");

    // retrieves the command
    std::string &command = commandTokens[0];

    // retrieves the command process information for the given command
    CommandProcessInformation_t commandProcessInformation = this->processInformationMap[command];

    // in case the command process information is valid
    if(commandProcessInformation.name == NULL && commandProcessInformation.processFunction == NULL) {
        // prints the invalid command message
        ConsoleManager::write(CONSOLE_MANAGER_INVALID_COMMAND_MESSAGE);
    } else {
        // calls the process function with the command tokens
        commandProcessInformation.processFunction(commandTokens, ConsoleManager::write, this);
    }
}

/**
* Retrieves the current carret value.
*
* @return The current carret value.
*/
std::string ConsoleManager::getCarretValue() {
    // allocates space for the carret
    std::string carret;

    // in case there is a selected script engine
    if(this->currentScriptEngine) {
        // adds the script engine name to the carret
        carret += "[" + this->currentScriptEngineName + "] ";
    }

    // adds the base carret value
    carret += CONSOLE_MANAGER_CARRET;

    // returns the carret
    return carret;
}

/**
* Retrieves the engine.
*
* @return The engine.
*/
Engine *ConsoleManager::getEngine() {
    return this->engine;
}

/**
* Sets the engine.
*
* @param engine The engine.
*/
void ConsoleManager::setEngine(Engine *engine) {
    this->engine = engine;
}

void ConsoleManager::write(const char *text, bool newline) {
    // prints the text
    std::cout << text;

    // in case a newline should be printed
    if(newline) {
        // prints a newline
        std::cout << "\n";
    }
}

void ConsoleManager::processHelp(std::vector<std::string> &commandTokens, WriteOuputFunction_t outputFunction, ConsoleManager *consoleManager) {
    // in case the number of arguments is invalid
    if(commandTokens.size() != 1) {
        // writes the invalid number of arguments text
        outputFunction(CONSOLE_INVALID_NUMBER_ARGUMENTS_MESSAGE, true);

        // returns in error
        return;
    }

    // writes the console help text
    outputFunction(CONSOLE_MANAGER_HELP_TEXT, true);
}

void ConsoleManager::processScript(std::vector<std::string> &commandTokens, WriteOuputFunction_t outputFunction, ConsoleManager *consoleManager) {
    // in case the number of arguments is invalid
    if(commandTokens.size() != 2) {
        // writes the invalid number of arguments text
        outputFunction(CONSOLE_INVALID_NUMBER_ARGUMENTS_MESSAGE, true);

        // returns in error
        return;
    }

    // retrieves the script engine name
    std::string &scriptEngineName = commandTokens[1];

    // sets the current script engine
    consoleManager->currentScriptEngine = consoleManager->engine->getScriptEngine(scriptEngineName);

    // sets the current script engine name
    consoleManager->currentScriptEngineName = scriptEngineName;

    // writes the console help text
    outputFunction(std::string("changed script engine to: " + scriptEngineName).c_str(), true);
}
