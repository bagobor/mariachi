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

#include "console_manager.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
ConsoleManager::ConsoleManager() {
    this->initProcessInformationMap();
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

void ConsoleManager::load(void *arguments) {
}

void ConsoleManager::unload(void *arguments) {
}

/**
* Processes the given command line, sending the output to the
* write function or to the standard output in case none is given.
*
* @param commandLine The command line to be processed.
* @param writeFunction The write function to receive the processing result.
*/
void ConsoleManager::processCommandLine(const char *commandLine, WriteOuputFunction_t writeFunction) {
    // in case the command line is empty
    if(!strcmp(commandLine, "")) {
        // returns immediately
        return;
    }

    // retrieves the command process information
    CommandProcessInformation_t commandProcessInformation = this->processInformationMap[commandLine];

    // in case the command process information is valid
    if(commandProcessInformation.name == NULL && commandProcessInformation.processFunction == NULL) {
        // prints the invalid command message
        ConsoleManager::write(CONSOLE_MANAGER_INVALID_COMMAND_MESSAGE);
    } else {
        // calls the process function
        commandProcessInformation.processFunction(commandLine, ConsoleManager::write);
    }
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

void ConsoleManager::processHelp(const char *commandLine, WriteOuputFunction_t writeFunction) {
    writeFunction(CONSOLE_MANAGER_HELP_TEXT, true);
}
