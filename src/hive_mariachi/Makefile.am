# Hive Mariachi Engine
# Copyright (C) 2008 Hive Solutions Lda.
#
# This file is part of Hive Mariachi Engine.
#
# Hive Mariachi Engine is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Hive Mariachi Engine is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Hive Mariachi Engine. If not, see <http:#www.gnu.org/licenses/>.

# __author__    = João Magalhães <joamag@hive.pt>
# __version__   = 1.0.0
# __revision__  = $LastChangedRevision: 2390 $
# __date__      = $LastChangedDate: 2009-04-02 08:36:50 +0100 (qui, 02 Abr 2009) $
# __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
# __license__   = GNU General Public License (GPL), Version 3

include $(top_srcdir)/Common.am

lib_LIBRARIES = libmariachi.a

libmariachi_a_CFLAGS = $(AM_CFLAGS)
libmariachi_a_CXXFLAGS = $(AM_CXXFLAGS)

lua_sources = script/lua_script_engine.cpp \
script/lua_script_engine/mapping/lua_script_engine_mapping.cpp \
script/lua_script_engine/mapping/main/lua_script_engine_engine.cpp \
script/lua_script_engine/mapping/nodes/lua_script_engine_node.cpp \
script/lua_script_engine/mapping/nodes/lua_script_engine_scene_node.cpp \
script/lua_script_engine/mapping/structures/lua_script_engine_position.cpp \
script/lua_script_engine/util/lua_script_engine_util.cpp

libmariachi_a_SOURCES = model_viewer.cpp \
stdafx.cpp \
importers/bmp_loader.cpp \
importers/md2_importer.cpp \
importers/model_importer.cpp \
importers/texture_importer.cpp \
main/engine.cpp \
nodes/camera_node.cpp \
nodes/lens_node.cpp \
nodes/model_node.cpp \
nodes/node.cpp \
nodes/scene_node.cpp \
tasks/task.cpp \
tasks/function_caller_task.cpp \
render/render_information.cpp \
render_adapters/direct3d9_adapter.cpp \
render_adapters/direct3d_adapter.cpp \
render_adapters/opengl_adapter.cpp \
render_adapters/render_adapter.cpp \
render_utils/direct3d_win32_window.cpp \
render_utils/direct3d_window.cpp \
render_utils/opengl_cocoa_window.cpp \
render_utils/opengl_glut_window.cpp \
render_utils/opengl_win32_window.cpp \
render_utils/opengl_window.cpp \
script/script_engine.cpp \
stages/dummy_stage.cpp \
stages/render_stage.cpp \
stages/stage.cpp \
stages/stage_runner.cpp \
structures/oct_tree.cpp \
structures/texture.cpp

if LINK_LUA
    libmariachi_a_SOURCES += $(lua_sources)
endif
