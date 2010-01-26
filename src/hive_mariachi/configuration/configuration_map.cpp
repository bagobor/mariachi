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

#include "configuration_map.h"

using namespace mariachi;

ConfigurationMap::ConfigurationMap() {
}

ConfigurationMap::~ConfigurationMap() {
    // cleans the map
    this->cleanMap();
}

inline void ConfigurationMap::cleanMap() {
    // retrieves the configuration map iterator
    std::map<std::string, ConfigurationValue_t *>::iterator configurationMapIterator = this->configurationMap.begin();

    // iterates over the configuration map
    while(configurationMapIterator != this->configurationMap.end()) {
        // retrieves the current key
        std::string key = configurationMapIterator->first;

        // removes the property with the given key
        this->removeProperty(key);
    }
}

/**
* Retrieves a property from configuration.
*
* @param key The key used to identify the property.
* @return The property value for the given key.
*/
ConfigurationValue_t *ConfigurationMap::getProperty(const std::string &key) {
    return this->configurationMap[key];
}

/**
* Sets the propery with the given key.
*
* @param key The key used to identify the property.
* @param value The property value to be used.
*/
void ConfigurationMap::setProperty(const std::string &key, ConfigurationValue_t *value) {
    this->configurationList.push_back(value);
    this->configurationMap[key] = value;
}

void ConfigurationMap::removeProperty(const std::string &key) {
    // retrieves the configuration value
    ConfigurationValue_t *configurationValue = this->configurationMap[key];

    // retrieves the configuration value type
    ConfigurationValueType_t configurationValueType = configurationValue->type;

    // switches over the configuration value type
    switch(configurationValueType) {
        // in case is of type int
        case CONFIGURATION_VALUE_INT_TYPE:
            break;

        // in case is of type string
        case CONFIGURATION_VALUE_STRING_TYPE:
            // releases the string value
            free(configurationValue->structure.stringValue);
            break;

        // in case is of type object
        case CONFIGURATION_VALUE_OBJECT_TYPE:
            // deletes the object value
            delete (ConfigurationMap *) configurationValue->structure.objectValue;

            break;
    }

    // releases the configuration value memory
    free(configurationValue);
}

void ConfigurationMap::setIntProperty(const std::string &key, int intValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t)) ;

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_INT_TYPE;

    // sets the int value in the configuration value
    configurationValue->structure.intValue = intValue;

    // sets the property
    this->setProperty(key, configurationValue);
}

void ConfigurationMap::setStringProperty(const std::string &key, const std::string &stringValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t)) ;

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_STRING_TYPE;

    // sets the string value in the configuration value
    configurationValue->structure.stringValue = new std::string(stringValue);

    // sets the property
    this->setProperty(key, configurationValue);
}

void ConfigurationMap::setObjectProperty(const std::string &key, ConfigurationMap *objectValue) {
    // allocates space for a new configuration value
    ConfigurationValue_t *configurationValue = (ConfigurationValue_t *) malloc(sizeof(ConfigurationValue_t)) ;

    // sets the type in the configuration value
    configurationValue->type = CONFIGURATION_VALUE_OBJECT_TYPE;

    // sets the object value in the configuration value value
    configurationValue->structure.objectValue = objectValue;

    // sets the property
    this->setProperty(key, configurationValue);
}
