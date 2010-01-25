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

#include "lua_script_engine_mapping.h"

using namespace mariachi;

bool lua_mariachi_get_reference(lua_State *luaState, void *value) {
    // retrieves the lua script engine
    LuaScriptEngine *luaScriptEngine = lua_getscriptengine(luaState);

    // allocates the reference
    int reference;

    if((reference = luaScriptEngine->getObjectReference(value))) {
        // retrieves the value from the registry index
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, reference);

        // returns false
        return false;
    }
    else {
        // creates a new table for the reference
        lua_newtable(luaState);

        // retrieves the reference index
        reference = luaL_ref(luaState, LUA_REGISTRYINDEX);
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, reference);

        // sets the association between the c++ reference and the lua index
        luaScriptEngine->setObjectReference(value, reference);

        // returns true
        return true;
    }
}

bool lua_mariachi_new_Object(lua_State *luaState, SceneNode *value) {
    bool return_value;

    // in case the reference is new
    if((return_value = lua_mariachi_get_reference(luaState, value))) {
        lua_set_self(luaState, value);
    }

    return return_value;
}

int lua_mariachi_get_engine(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargs(luaState, 0);

    // retrieves the lua script engine
    LuaScriptEngine *luaScriptEngine = lua_getscriptengine(luaState);

    // creates a new table (the engine object)
    lua_newtable(luaState);
    lua_set_self(luaState, luaScriptEngine->getEngine());
    lua_setnamefunction(luaState, "get_render", lua_mariachi_engine_get_render);

    // returns the number of return values
    return 1;
}

LuaScriptEngine *lua_getscriptengine(lua_State *luaState) {
    // loads the global variable
    lua_getglobal(luaState, LUA_SCRIPT_ENGINE_GLOBAL_VARIABLE);

    // retrieves the lua script engine
    LuaScriptEngine *luaScriptEngine = (LuaScriptEngine *)  lua_tointeger(luaState, -1);

    // returns the lua script engine
    return luaScriptEngine;
}
