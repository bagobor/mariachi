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
#include "../../serialization/json.h"

#include "json_configuration_parser.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
JsonConfigurationParser::JsonConfigurationParser() {
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

	std::string encoding = rootNode.get("encoding", "UTF-8" ).asString();
}

/**
* Parses the given resource generating the internal structure.
*
* @param resource The resource to be parsed.
*/
void JsonConfigurationParser::parseResource(void *resource) {
}
