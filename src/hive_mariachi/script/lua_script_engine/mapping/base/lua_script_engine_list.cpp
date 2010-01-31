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

#include "lua_script_engine_list.h"

template<class T> bool lua_mariachi_new_List(lua_State *luaState, std::list<T> *value) {
    bool return_value;

    // in case the reference is new
    validate_reference(return_value, luaState, value, LUA_SCRIPT_ENGINE_LIST_TYPE, lua_mariachi_new_Object) {
        // sets the type of the node
        lua_settype(luaState, LUA_SCRIPT_ENGINE_LIST_TYPE);

        // sets the methods
        lua_setnamefunction(luaState, "front", lua_mariachi_list_front<T>);
        lua_setnamefunction(luaState, "get", lua_mariachi_list_get<T>);
    }

    return return_value;
}

template<class T> int lua_mariachi_list_front(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 0);

    // retrieves self
    std::list<T> *self = (std::list<T> *) lua_get_self(luaState);

    // retrieves the front value
    T frontValue self->front();

    // aki nao tinha outro remedio senao ir ao registo
    // e usar o T para criar um constructor ou se possivel
    // sacar a referencia de c++
    // tenho de conseguir sacar tipos de dados dos varios apontadores

    // a partir dai inicio o constructor automaticamente

    // essa interface tem de ser do tipo
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // loadOrCreateValue<T>(frontValue)
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // returns the number of return values
    return 1;
}

template<class T> int lua_mariachi_list_get(lua_State *luaState) {
    // validates the number of arguments
    lua_assertargsmethod(luaState, 1);

    // retrieves self
    std::list<T> *self = (std::list<T> *) lua_get_self(luaState);

    // retrieves the index
    int index = (int) lua_tointeger(luaState, 1);

    //T &listValue = (*self)[index];

    //self

    // retrieves the position
   // Coordinate3d_t position = self->getPosition();

    // creates and loads a coordinate 3d
   // lua_mariachi_new_Coordinate3d_t(luaState, &position);

    // returns the number of return values
    return 1;
}
