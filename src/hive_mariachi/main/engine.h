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
#include "../console/console.h"
#include "../nodes/nodes.h"
#include "../user_interface/user_interface.h"
#include "../tasks/task.h"
#include "../debugging/debugging.h"

#ifdef MARIACHI_PLATFORM_WIN32
#define threadHandleStageMapPut(threadHandle, threadId, currentStage) this->threadHandleStageMap[threadHandle] = currentStage
#elif MARIACHI_PLATFORM_UNIX
#define threadHandleStageMapPut(threadHandle, threadId, currentStage) this->threadHandleStageMap[threadId] = currentStage
#endif

/**
* The branding text value.
*/
#define MARIACHI_BRANDING_TEXT "Hive Mariachi Engine"

/**
* The version value.
*/
#define MARIACHI_VERSION "1.0.0"

/**
* The realse information value.
*/
#define MARIACHI_RELEASE_INFO "(Hive Solutions Lda. r1:Jan 19 2010)"

/**
* The help text value.
*/
#define MARIACHI_HELP_TEXT "Type \"help\" for more information."

THREAD_RETURN mainRunnerThread(THREAD_ARGUMENTS parameters);
THREAD_RETURN stageRunnerThread(THREAD_ARGUMENTS parameters);

namespace mariachi {
    namespace physics {
        class PhysicsEngine;
    }

    class ConfigurationManager;
    class Stage;

    class Engine {
        private:
            /**
            * The number of arguments sent to the program.
            */
            int argc;

            /**
            * The value of the arguments sent to the program.
            */
            char **argv;

            /**
            * The flag that controls the running of the engine.
            */
            bool runningFlag;

            /**
            * The logger used in the message logging.
            */
            Logger *logger;

            /**
            * The manager used to provide configuration options.
            */
            ConfigurationManager *configurationManager;

            /**
            * The manager used to control the console.
            */
            ConsoleManager *consoleManager;

            /**
            * The top level 3d scene render node.
            */
            SceneNode *render;

            /**
            * The top level 2d scene render node.
            */
            Scene2dNode *render2d;

            /**
            * The engine used for debuging provisioning.
            */
            DebugEngine *debugEngine;

            /**
            * The mutex that controls the task list access.
            */
            MUTEX_HANDLE taskListMutex;

            /**
            * The task list ready condition.
            */
            CONDITION_HANDLE taskListReadyCondition;

            /**
            * The map associating the stage name with the
            * stage reference.
            */
            std::map<std::string, Stage *> stagesMap;

            /**
            * The map associating the device name with the
            * device reference.
            */
            std::map<std::string, Device *> devicesMap;

            /**
            * The list of currently available tasks.
            */
            std::list<Task *> taskList;

            /**
            * The list of stages to be run in the main thread.
            */
            std::list<Stage *> mainThreadStagesList;

            /**
            * The map associating the thread handle with the associated stage.
            */
            std::map<THREAD_REFERENCE, Stage *> threadHandleStageMap;

            /**
            * The currently active physics engine, to be used in physics operations.
            */
            physics::PhysicsEngine *activePhysicsEngine;

            inline void initRunningFlag();
            inline void initRenders();
            inline void initArgs(int argc, char** argv);

        public:
            Engine();
            Engine(int argc, char** argv);
            ~Engine();
            void start(void *arguments);
            void stop(void *arguments);
            void update();
            void printInformation();
            void handleException(Exception *exception);
            void startConfigurationManager();
            void stopConfigurationManager();
            void startConsoleManager();
            void stopConsoleManager();
            void startLogger(int level, bool pidFile);
            void startInputDevices();
            void startScriptEngines();
            void startPhysicsEngines();
            void startStages();
            void stopStages();
            void startDebugEngine();
            void stopDebugEngine();
            void startRunLoop();
            void addTask(Task *task);
            void removeTask(Task *task);
            void getCurrentProcessIdString(std::string &currentProcessIdString);
            Stage *getStage(const std::string &stageName);
            void setStage(const std::string &stageName, Stage *stage);
            Device *getDevice(const std::string &deviceName);
            void setDevice(const std::string &deviceName, Device *device);
            ConfigurationManager *getConfigurationManager();
            void setConfigurationManager(ConfigurationManager *configurationManager);
            ConsoleManager *getConsoleManager();
            void setConsoleManager(ConsoleManager *consoleManager);
            Logger *getLogger();
            void setLogger(Logger *logger);
            SceneNode *getRender();
            void setRender(SceneNode *render);
            Scene2dNode *getRender2d();
            void setRender2d(Scene2dNode *render2d);
            physics::PhysicsEngine *getActivePhysicsEngine();
            void setActivePhysicsEngine(physics::PhysicsEngine *activePhysicsEngine);
            int getArgc();
            char **getArgv();
    };
}
