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

#include "lua_script_engine/_lua_script_engine.h"

#include "lua_script_engine.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
LuaScriptEngine::LuaScriptEngine() : ScriptEngine() {
}

LuaScriptEngine::LuaScriptEngine(Engine *engine) : ScriptEngine(engine) {
}

/**
* Destructor of the class.
*/
LuaScriptEngine::~LuaScriptEngine() {
}

void LuaScriptEngine::load(void *arguments) {
    // allocates the defined lua libs
    luaL_reg lualibs[] = { { "base", luaopen_base }, { NULL, NULL } };

    // opens a new lua state
    this->luaState = lua_open();

    // loads the lua base libraries
    luaL_openlibs(this->luaState);

    // opens the defined lua libs
    this->openLuaLibs(lualibs);

    // registers the scrip engine global variable
    lua_pushinteger(luaState, (long long) this);
    lua_setglobal(luaState, LUA_SCRIPT_ENGINE_GLOBAL_VARIABLE);

    // creates a new table and registers the entry point global function
    lua_newtable(luaState);
    lua_setnamefunction(luaState, LUA_SCRIPT_ENGINE_GET_ENGINE_METHOD, lua_mariachi_get_engine);
    lua_setglobal(luaState, LUA_SCRIPT_ENGINE_BASE_NAMESPACE);

    // generates the constructors
    lua_generateconstructors(luaState);
}

void LuaScriptEngine::unload(void *arguments) {
    // destroys the lua state
    lua_close(this->luaState);
}

void LuaScriptEngine::runScript(Script_t *script) {
}

void LuaScriptEngine::runScriptFile(const std::string &scriptPath) {
    luaL_dofile(this->luaState, scriptPath.c_str());
}

void LuaScriptEngine::runScriptString(const std::string &scriptString) {
    luaL_dostring(this->luaState, scriptString.c_str());
}

int LuaScriptEngine::getObjectReference(void *cObjectReference) {
    return this->objectMap[cObjectReference];
}

void LuaScriptEngine::setObjectReference(void *cObjectReference, int luaObjectIndex) {
    this->objectMap[cObjectReference] = luaObjectIndex;
}

inline void LuaScriptEngine::openLuaLibs(luaL_reg *luaLibs) {
    for(luaL_reg *lib = luaLibs; lib->func != NULL; lib++) {
        lib->func(luaState);
        lua_settop(luaState, 0);
    }
}
