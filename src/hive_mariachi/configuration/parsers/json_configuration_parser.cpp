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

#include "../../structures/structures.h"

#include "json_configuration_parser.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
JsonConfigurationParser::JsonConfigurationParser() {
}

/**
* Constructor of the class.
*
* @param configurationManager The configuration manager that contains the reference to the configuration parser.
*/
JsonConfigurationParser::JsonConfigurationParser(ConfigurationManager *configurationManager) : ConfigurationParser(configurationManager) {
}

/**
* Destructor of the class.
*/
JsonConfigurationParser::~JsonConfigurationParser() {
}

/**
* Parses the given configuration generating the internal structure.
*
* @param configuration The configuration to be parsed.
*/
void JsonConfigurationParser::parseConfiguration(void *configuration) {
    // root json node
    Json::Value rootNode;

    // creates the json reader
    Json::Reader reader = Json::Reader(Json::Features());

    // retrieves the file data from configuration
    FileData_t *fileData = (FileData_t *) configuration;

    // retrieves the reference to the begining of the document
    const char *beginningDocument = (const char *) fileData->data;

    // retrieves the reference to the end of the document
    const char *endDocument = (const char *) &fileData[fileData->size];

    // parses the json file
    bool parsingSuccessful = reader.parse(beginningDocument, endDocument, rootNode);

    // in case the parsing was not succsessful
    if(!parsingSuccessful) {
        // retrieves the error message
        std::string errorMessage = reader.getFormatedErrorMessages();

        // throws an exception
        throw "Failed to parse configuration: " + errorMessage;
    }

    this->updateConfigurationMap(this->configurationManager, rootNode);
}

/**
* Parses the given resource generating the internal structure.
*
* @param resource The resource to be parsed.
*/
void JsonConfigurationParser::parseResource(void *resource) {
}

void JsonConfigurationParser::updateConfigurationMap(ConfigurationMap *configurationMap, const Json::Value &currentNode) {
    // retrieves the menber names
    Json::Value::Members currentNodeMembers = currentNode.getMemberNames();

    // retrieves the current node members iterator
    Json::Value::Members::iterator currentNodeMembersIterator = currentNodeMembers.begin();

    // itearates over all the current node members
    while(currentNodeMembersIterator != currentNodeMembers.end()) {
        // retrieves the property name
        std::string &propertyName = *currentNodeMembersIterator;

        // retrieves the property value
        Json::Value propertyValue = currentNode[propertyName];

        // retrieves the property value type
        Json::ValueType propertyValueType = propertyValue.type();

        int propertyValueIntValue;
        std::string propertyValueStringValue;

        switch(propertyValueType) {
            case Json::nullValue:
                break;

            case Json::intValue:
                // retrieves the property value as int
                propertyValueIntValue = propertyValue.asInt();

                // sets the int property in the configuration manager
                this->configurationManager->setIntProperty(propertyName, propertyValueIntValue);

                break;

            case Json::uintValue:
                // retrieves the property value as int
                propertyValueIntValue = propertyValue.asUInt();

                // sets the int property in the configuration manager
                this->configurationManager->setIntProperty(propertyName, propertyValueIntValue);

                break;

            case Json::realValue:
                break;

            case Json::stringValue:
                // retrieves the property value as string
                propertyValueStringValue = propertyValue.asString();

                // sets the string property in the configuration manager
                this->configurationManager->setStringProperty(propertyName, propertyValueStringValue);

                break;

            case Json::booleanValue:
                break;

            case Json::arrayValue:
                break;

            case Json::objectValue:
                // creates a new configuration map
                ConfigurationMap *configurationMap = new ConfigurationMap();

                // updates the configuration map with the new "current" node
                this->updateConfigurationMap(configurationMap, propertyValue);

                // sets the object property in the configuration manager
                this->configurationManager->setObjectProperty(propertyName, configurationMap);

                break;
        }

        // increments the current node members iterator
        currentNodeMembersIterator++;
    }
}
