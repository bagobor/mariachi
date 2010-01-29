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

#pragma once

#include "../exceptions/exception.h"
#include "../system/thread.h"
#include "../devices/device.h"
#include "../logging/logger.h"
#include "../nodes/nodes.h"
#include "../user_interface/user_interface.h"
#include "../tasks/task.h"
#include "../debugging/debugging.h"

#ifdef MARIACHI_PLATFORM_WIN32
#define threadHandleStageMapPut(threadHandle, threadId, currentStage) this->threadHandleStageMap[threadHandle] = currentStage
#elif MARIACHI_PLATFORM_UNIX
#define threadHandleStageMapPut(threadHandle, threadId, currentStage) this->threadHandleStageMap[threadId] = currentStage
#endif

THREAD_RETURN mainRunnerThread(THREAD_ARGUMENTS parameters);
THREAD_RETURN stageRunnerThread(THREAD_ARGUMENTS parameters);

namespace mariachi {
    class ConfigurationManager;
    class Stage;

    class Engine {
        private:
            int argc;
            char **argv;
            bool runningFlag;
            Logger *logger;
            ConfigurationManager *configurationManager;
            SceneNode *render;
            Scene2dNode *render2d;
            DebugEngine *debugEngine;
            MUTEX_HANDLE taskListMutex;
            CONDITION_HANDLE taskListReadyCondition;
            std::map<std::string, Device *> devicesMap;
            std::list<Task *> taskList;
            std::list<Stage *> mainThreadStagesList;
            std::map<THREAD_REFERENCE, Stage *> threadHandleStageMap;

        public:
            Engine();
            Engine(int argc, char** argv);
            ~Engine();
            inline void initRunningFlag();
            inline void initRenders();
            inline void initArgs(int argc, char** argv);
            void start(void *arguments);
            void stop(void *arguments);
            void update();
            void handleException(Exception *exception);
            void startConfigurationManager();
            void stopConfigurationManager();
            void startLogger(int level, bool pidFile);
            void startInputDevices();
            void startScriptEngines();
            void startStages();
            void stopStages();
            void startDebugEngine();
            void stopDebugEngine();
            void startRunLoop();
            void addTask(Task *task);
            void removeTask(Task *task);
            void getCurrentProcessIdString(std::string &currentProcessIdString);
            Device *getDevice(const std::string &deviceName);
            void setDevice(const std::string &deviceName, Device *device);
            ConfigurationManager *getConfigurationManager();
            void setConfigurationManager(ConfigurationManager *configurationManager);
            Logger *getLogger();
            void setLogger(Logger *logger);
            SceneNode *getRender();
            void setRender(SceneNode *render);
            Scene2dNode *getRender2d();
            void setRender2d(Scene2dNode *render2d);
            int getArgc();
            char **getArgv();
    };
}
