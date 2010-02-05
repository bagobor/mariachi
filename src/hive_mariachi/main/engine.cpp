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

#include "../exceptions/exceptions.h"
#include "../devices/devices.h"
#include "../system/system.h"
#include "../stages/stages.h"
#include "../configuration/configuration.h"
#include "../console/console.h"
#include "../script/script.h"
#include "../physics/physics.h"

#include "../algorithms/algorithms.h"

#include "engine.h"

using namespace mariachi;
using namespace mariachi::physics;

#define MATIAS 10
#define BUFFER_SIZE_T 102400

/**
* Thread that handles the main engine workload.
* Initializes the script engines and the stages.
*
* @param parameters The thread parameters.
* @return The thread result.
*/
THREAD_RETURN mainRunnerThread(THREAD_ARGUMENTS parameters) {
    // retrieves the engine from the parameters
    Engine *engine = (Engine *) parameters;

    try {
        // creates a new huffman object
       /* Huffman huffamn;

        // encodes the given file
        time_t initialTimeEncoding = time(NULL);

        huffamn.encode("C:/colony_production_database.db", "c:/colony_production_database.db.encoded");

        time_t endTimeEncoding = time(NULL);

        printf("Encoding took %d\n", endTimeEncoding - initialTimeEncoding);

        time_t initialTimeDecoding = time(NULL);

        huffamn.decode("c:/colony_production_database.db.encoded", "c:/colony_production_database.db.decoded");

        time_t endTimeDecoding = time(NULL);

        printf("Decoding took %d\n", endTimeDecoding - initialTimeDecoding);*/

        // prints the huffman table
       // huffamn.printTable();

		printf("Starting the configuration manager");

        // starts the configuration manager in the engine
        engine->startConfigurationManager();

		printf("Starting the comsole manager");

        // starts the console manager in the engine
        engine->startConsoleManager();

		printf("Starting the logger");

        // starts the logger in the engine
        engine->startLogger(DEBUG, true);

		engine->getLogger()->info("Starting the input devices");

        // starts the input devices in the engine
        engine->startInputDevices();

		engine->getLogger()->info("Starting script engines");

        // starts the script engines in the engine
        engine->startScriptEngines();

		engine->getLogger()->info("Starting physics engines");

        // starts the physics engines in the engine
        engine->startPhysicsEngines();

		engine->getLogger()->info("Starting stages");

        // starts the stages in the engine
        engine->startStages();

        // starts the run loop
        engine->startRunLoop();

        // stops the stages in the engine
        engine->stopStages();

        // returns valid value
        return THREAD_INVALID_RETURN_VALUE;
    } catch(Exception exception) {
		printf("Recebeu excepcao");

        // handles the exception
        engine->handleException(&exception);

        // returns invalid value
        return THREAD_INVALID_RETURN_VALUE;
    }
}

/**
* Thread than runs a new stage. It controls the frequency of the loading
* and manages it.
*
* @param parameters The thread parameters.
* @return The thread result.
*/
THREAD_RETURN stageRunnerThread(THREAD_ARGUMENTS parameters) {
    // retrieves the stage from the parameters
    Stage *stage = (Stage *) parameters;

    // retrieves the engine from the stage
    Engine *engine = (Engine *) stage->getEngine();

    try {
        // creates a new stage runner
        StageRunner stageRunner = StageRunner(stage);

        // registers the stage runner
        engine->setStageRunner(stage, &stageRunner);

        // starts the stage runner
        stageRunner.start(NULL);

        // returns valid value
        return THREAD_INVALID_RETURN_VALUE;
    } catch(Exception exception) {
        // handles the exception
        engine->handleException(&exception);

        // returns invalid value
        return THREAD_INVALID_RETURN_VALUE;
    }
}

/**
* Contructor of the class.
*/
Engine::Engine() {
    this->initRunningFlag();
    this->initRenders();
}

/**
* Contructor of the class.
*
* @param argc The number of arguments sent to the program.
* @param argv The value of the arguments sent to the program.
*/
Engine::Engine(int argc, char** argv) {
    this->initRunningFlag();
    this->initRenders();
    this->initArgs(argc, argv);
}

/**
* Destructor of the class.
*/
Engine::~Engine() {
}

/**
* Initializes the running flag.
*/
inline void Engine::initRunningFlag() {
    // sets the running flag
    this->runningFlag = true;
}

/**
* Initializes the renders.
*/
inline void Engine::initRenders() {
    // creates the render node
    this->render = new SceneNode(std::string("render"));

    // creates the render 2d node
    this->render2d = new Scene2dNode(std::string("render2d"));
}

/**
* Initializes the command line arguments.
*/
inline void Engine::initArgs(int argc, char** argv) {
    this->argc = argc;
    this->argv = argv;
}

/**
* Starts the engine running all the required bootstrap operations.
*
* @param arguments The arguments for the engine start.
*/
void Engine::start(void *arguments) {
    // prints the information
    this->printInformation();

	printf("Creating the mutex");

    // creates the task list mutex
    MUTEX_CREATE(this->taskListMutex);

    // creates the task list ready condition
    CONDITION_CREATE(this->taskListReadyCondition);

    // allocates space for the thread id
    THREAD_IDENTIFIER threadId;

	printf("Creating the engine runner thread");

    // creates the engine runnner thread
    THREAD_HANDLE mainRunnerThreadHandle = THREAD_CREATE_BASE(threadId, mainRunnerThread, this);

	printf("Finished creating the engine runner thread");

/*SLEEP(100);

    // iterates while the running flag is active
    while(this->runningFlag) {
		//printf("Entering while");

        // iterates while the task list contains
        // valid task elements*/
        /*while(this->taskList.size()) {
            // retrieves the current task
            Task *currentTask = this->taskList.front();

            // pops the task list
            this->taskList.pop_front();

            // starts the current task
            currentTask->start(NULL);
        }*/

        // waits for the task list ready condition
        //CONDITION_WAIT(this->taskListReadyCondition);

        // resets the condition
        //CONDITION_RESET(this->taskListReadyCondition);
/*		SLEEP(100);
    }*/
}

/**
* Stops the engine running all the required unloading operations.
*
* @param arguments The arguments for the engine stop.
*/
void Engine::stop(void *arguments) {
    // unsets the running flag
    this->runningFlag = false;

    // signals the task list ready condition (to unblock the thread)
    CONDITION_SIGNAL(this->taskListReadyCondition);
}

/**
* Updates the engine state, running the main thread stages and any other
* recursive engine operations.
*/
void Engine::update() {
    // retrieves the main thread stages list iterator
    std::list<Stage *>::iterator mainThreadStagesListIterator = this->mainThreadStagesList.begin();

    // iterates over all the main thread stages
    while(mainThreadStagesListIterator != this->mainThreadStagesList.end()) {
        // retrieves the main thread stage
        Stage *mainThreadStage = *mainThreadStagesListIterator;

        // updates the current main thread stage
        mainThreadStage->update(NULL);

        // increments the main thread stages list iterator
        mainThreadStagesListIterator++;
    }
}

/**
* Prints the branding information into the standard output.
*/
void Engine::printInformation() {
	printf("started printing branding");

    // prints the branding information
	//std::cout << MARIACHI_BRANDING_TEXT << " " MARIACHI_VERSION << " " << MARIACHI_RELEASE_INFO << std::endl;

    // prints the help text
    //std::cout << MARIACHI_HELP_TEXT << std::endl;

	printf("finished printing branding information\n");
}

/**
* Handles the given exception, taking the necessary measures to
* minimize damage.
*
* @param exception The exception to be handled.
*/
void Engine::handleException(Exception *exception) {
    // prints the exception
    this->logger->critical("[Exception]: " + exception->getMessage());
}

/**
* Starts the configuration manager in the engine.
* Starting the configuration manager implies booting the necessary
* structures for data serialization.
*/
void Engine::startConfigurationManager() {
    // creates a configuration manager
    this->configurationManager = new ConfigurationManager(this);

    // creates the configuration arguments
    ConfigurationArguments_t configurationArguments = { "config.json" };

    // loads the configuration manager
    this->configurationManager->load(&configurationArguments);
}

/**
* Stops the configuration manager in the engine.
*/
void Engine::stopConfigurationManager() {
    // loads the configuration manager
    this->configurationManager->unload(NULL);

    // deletes the configuration manager
    delete this->configurationManager;
}

/**
* Starts the console manager in the engine.
*/
void Engine::startConsoleManager() {
    // creates a console manager
    this->consoleManager = new ConsoleManager(this);

    // loads the configuration manager
    this->consoleManager->load(NULL);
}

/**
* Stops the console manager in the engine.
*/
void Engine::stopConsoleManager() {
    // unloads the console manager
    this->consoleManager->unload(NULL);

    // deletes the console manager
    delete this->consoleManager;
}

/**
* Starts the logging system, creating a file and a console handler.
*
* @param level The level of debugging to be used in the logging.
* @param pidFile If the process id information should be used for the file.
*/
void Engine::startLogger(int level, bool pidFile) {
    // allocates a new string for the current process id string
    std::string currentProcessIdString = std::string();

    // in case the pid file is active
    if(pidFile) {
        // retrieves the current process id string
        getCurrentProcessIdString(currentProcessIdString);
    }

    // retrieves the default logger
    this->logger = Logger::getLogger();

    // retrieves the logging verbosity value
    ConfigurationValue_t *loggingVerbosityProperty = this->configurationManager->getProperty("logging/verbosity");

    // in case a verbosity level is defined in the configuration
    if(loggingVerbosityProperty) {
        // sets the default logging level
        this->logger->setLevel(loggingVerbosityProperty->structure.intValue);
    } else {
        // sets the default logging level
        this->logger->setLevel(level);
    }

    // retrieves the logging file property
    ConfigurationValue_t *loggingFileProperty = this->configurationManager->getProperty("logging/file");

    // in case the a file logger should be used
    if(loggingFileProperty && loggingFileProperty->structure.booleanValue) {
        // creates a new file handler
        LoggerFileHandler *fileHandler = new LoggerFileHandler(std::string("service_pid" + currentProcessIdString + ".log"));

        // adds the file handler to the default logger
        this->logger->addHandler(fileHandler);
    }
}

/**
* Starts the input devices in the engine.
* Starting the input devices implies starting the drivers and start listning
* to them.
*/
void Engine::startInputDevices() {
    // creates a new keyboard
    Keyboard *keyboard = new Keyboard();

    // creates a new mouse
    Mouse *mouse = new Mouse();

    // sets the keyboard in the devices registry
    this->setDevice("keyboard", keyboard);

    // sets the mouse in the devices registry
    this->setDevice("mouse", mouse);
}

/**
* Starts the script engines in the engine.
* Starting the script engines implies loading them and running
* the initial script files.
*/
void Engine::startScriptEngines() {
    // creates a new lua script engine
    LuaScriptEngine *luaScriptEngine = new LuaScriptEngine(this);

    // loads the lua script engine
    luaScriptEngine->load(NULL);

    ConfigurationValue_t *luaConfigurationExtraPaths = this->configurationManager->getProperty("scripting/lua/extra_paths");

    if(luaConfigurationExtraPaths) {
        // retrieves the list value
        ConfigurationList *listValue = (ConfigurationList *) luaConfigurationExtraPaths->structure.listValue;

        // retrieves the string vector value
        std::vector<std::string *> stringVectorValue = listValue->getAsStringVector();

        // adds the extra paths
        luaScriptEngine->addExtraPaths(stringVectorValue);
    }

    // sets the lua script engine in the script engines registry
    this->setScriptEngine("lua", luaScriptEngine);
}

/**
* Starts the physics engines in the engine.
* Starting the physics engines implies loading them.
*/
void Engine::startPhysicsEngines() {
    // creates a new bullet physics engine
    BulletPhysicsEngine *bulletPhysicsEngine = new BulletPhysicsEngine(this);

    // loads the bullet physics engine
    bulletPhysicsEngine->load(NULL);

    // sets the bullet physics engine as the active physics engine
    this->setActivePhysicsEngine(bulletPhysicsEngine);
}

/**
* Starts the stages in the engine.
* Starting the stages implies loading them and creating new threads
* for the ones than require a new thread to be created.
*/
void Engine::startStages() {
    // creates the stages list
    std::vector<Stage *> stagesList;

    // includes the stages initialization
    #include "../stages/stages_init.h"

    // retrieves the stages list iterator
    std::vector<Stage *>::iterator stagesListIterator = stagesList.begin();

    // iterates over all the stages in the stages list
    while(stagesListIterator != stagesList.end()) {
        // retrieves the current stage
        Stage *currentStage = *stagesListIterator;

        // retrieves the current stage name
        std::string &currentStageName = currentStage->getName();

        // in case the current stage requires a thread to run
        if(currentStage->requiresThread()) {
            // allocates space for the thread id
            THREAD_IDENTIFIER threadId;

            // creates the engine runnner thread
            THREAD_HANDLE threadHandle = THREAD_CREATE_BASE(threadId, stageRunnerThread, currentStage);

            // sets the current stage in the thread handle stage map
            threadHandleStageMapPut(threadHandle, threadId, currentStage);
        } else {
            // starts the current stage
            currentStage->start(NULL);

            // adds the current stage to the list of main thread stages
            this->mainThreadStagesList.push_back(currentStage);
        }

        // sets the stage in the stages registry
        this->setStage(currentStageName, currentStage);

        // increments the stages list iterator
        stagesListIterator++;
    }
}

/**
* Stops the stages in the engine.
* Starting the stages implies unloading them and joining their threads.
*/
void Engine::stopStages() {
    // retrieves the thread handle stage map iterator
    std::map<THREAD_REFERENCE, Stage *>::iterator threadHandleStageMapIterator = this->threadHandleStageMap.begin();

    // iterates over all the stage thread handles
    while(threadHandleStageMapIterator != this->threadHandleStageMap.end()) {
        // retrieves the thread handle
        THREAD_REFERENCE threadHandle = threadHandleStageMapIterator->first;

        // retrieves the stage
        Stage *stage = threadHandleStageMapIterator->second;

        // retrieves the stage runner for the stage
        StageRunner *stageRunner = this->stageRunnersMap[stage];

        // in case the stage runner is invalid
        if(!stageRunner) {
            // throws a runtime exception
            throw RuntimeException("No stage runner associated with stage: " + stage->getName());
        }

        // stops the stage runner
        stageRunner->stop(NULL);

        // joins the stage thread
        THREAD_JOIN(threadHandle);

        // closes the thread handle
        THREAD_CLOSE(threadHandle);

        // increments the thread handle stage map iterator
        threadHandleStageMapIterator++;
    }

    // retrieves the main thread stages list iterator
    std::list<Stage *>::iterator mainThreadStagesListIterator = this->mainThreadStagesList.begin();

    // iterates over all the main thread stages
    while(mainThreadStagesListIterator != this->mainThreadStagesList.end()) {
        // retrieves the main thread stage
        Stage *mainThreadStage = *mainThreadStagesListIterator;

        // stops the current main thread stage
        mainThreadStage->stop(NULL);

        // increments the main thread stages list iterator
        mainThreadStagesListIterator++;
    }
}

/**
* Starts the debug engine, instantiating it and loading.
*/
void Engine::startDebugEngine() {
    // creates a new debug engine
    this->debugEngine = new DebugEngine();

    // starts the debug engine
    this->debugEngine->start(NULL);
}

/**
* Stops the debug engine, unloading it.
*/
void Engine::stopDebugEngine() {
    // stops the debug engine
    this->debugEngine->stop(NULL);

    // deletes the debug engine
    delete this->debugEngine;
}

/**
* Starts the engine run loop (where it runs the main stages).
*/
void Engine::startRunLoop() {
    // iterates while the running flag is active
    while(this->runningFlag) {
        // updates the engine state
        this->update();

        // @todo changes this hardcoded sleep
        SLEEP(100);
    }
}

/**
* Adds the given task to the task list.
*
* @param task The task to be added to the task list.
*/
void Engine::addTask(Task *task) {
    // locks the list mutex
    MUTEX_LOCK(this->taskListMutex);

    // adds the task to the task list
    this->taskList.push_back(task);

    // signals the task list ready condition
    CONDITION_SIGNAL(this->taskListReadyCondition);

    // unlocks the task list mutex
    MUTEX_UNLOCK(this->taskListMutex);
}

/**
* Adds the given task from the task list.
*
* @param task The task to be removed from the task list.
*/
void Engine::removeTask(Task *task) {
    // locks the task list mutex
    MUTEX_LOCK(this->taskListMutex);

    // removes the task from the task list
    this->taskList.remove(task);

    // unlocks the task list mutex
    MUTEX_UNLOCK(this->taskListMutex);
}

/**
* Retrieves the current process id string and puts it into the given string.
*
* @param currentProcessIdString The string to be updated with the process id string.
*/
void Engine::getCurrentProcessIdString(std::string &currentProcessIdString) {
    // retrieves the current process id
    PID_TYPE currentProcessId = GET_PID();

    // creates the current process id stream
    std::stringstream currentProcessIdStream;

    // pipes the current process id to the current process id stream
    currentProcessIdStream << currentProcessId;

    // pipes the current process id stream to the current process id string
    currentProcessIdStream >> currentProcessIdString;
}

/**
* Retrieves the stage runner for the given stage.
*
* @param stage The stage to retrieve the stage runner.
* @return The stage runner for the given stage.
*/
StageRunner *Engine::getStageRunner(Stage *stage) {
    return this->stageRunnersMap[stage];
}

/**
* Sets the stage runner with the given stage.
*
* @param stage The stage to be used to identify the stage runner.
* @param stageRunner The stage runner to be set.
*/
void Engine::setStageRunner(Stage *stage, StageRunner *stageRunner) {
    // sets the stage runner in the stage runners map with
    // the given stage
    this->stageRunnersMap[stage] = stageRunner;
}

/**
* Retrieves the stage for the given stage name.
*
* @param stageName The stage name to retrieve the stage.
* @return The stage for the given stage name.
*/
Stage *Engine::getStage(const std::string &stageName) {
    return this->stagesMap[stageName];
}

/**
* Sets the stage with the given stage name.
*
* @param stageName The name to be used to identify the stage.
* @param stage The stage to be set.
*/
void Engine::setStage(const std::string &stageName, Stage *stage) {
    // sets the stage in the stages map with
    // the given stage name
    this->stagesMap[stageName] = stage;
}

/**
* Retrieves the device for the given device name.
*
* @param deviceName The device name to retrieve the device.
* @return The device for the given device name.
*/
Device *Engine::getDevice(const std::string &deviceName) {
    return this->devicesMap[deviceName];
}

/**
* Sets the device with the given device name.
*
* @param deviceName The name to be used to identify the device.
* @param device The device to be set.
*/
void Engine::setDevice(const std::string &deviceName, Device *device) {
    // sets the device in the devices map with
    // the given device name
    this->devicesMap[deviceName] = device;
}

/**
* Retrieves the script engine for the given script engine name.
*
* @param scriptEngineName The script engine name to retrieve the script engine.
* @return The script engine for the given script engine name.
*/
ScriptEngine *Engine::getScriptEngine(const std::string &scriptEngineName) {
    return this->scriptEnginesMap[scriptEngineName];
}

/**
* Sets the script engine with the given script engine name.
*
* @param scriptEngineName The name to be used to identify the script engine.
* @param scriptEngine The script engine to be set.
*/
void Engine::setScriptEngine(const std::string &scriptEngineName, ScriptEngine *scriptEngine) {
    // sets the script engine in the script engines map with
    // the given script engine name
    this->scriptEnginesMap[scriptEngineName] = scriptEngine;
}

/**
* Retrieves the physics engine for the given physics engine name.
*
* @param physicsEngineName The physics engine name to retrieve the physics engine.
* @return The physics engine for the given physics engine name.
*/
PhysicsEngine *Engine::getPhysicsEngine(const std::string &physicsEngineName) {
    return this->physicsEnginesMap[physicsEngineName];
}

/**
* Sets the physics engine with the given physics engine name.
*
* @param physicsEngineName The name to be used to identify the physics engine.
* @param physicsEngine The physics engine to be set.
*/
void Engine::setPhysicsEngine(const std::string &physicsEngineName, PhysicsEngine *physicsEngine) {
    // sets the physics engine in the physics engines map with
    // the given physics engine name
    this->physicsEnginesMap[physicsEngineName] = physicsEngine;
}

/**
* Retrieves the configuration manager.
*
* @return The configuration manager.
*/
ConfigurationManager *Engine::getConfigurationManager() {
    return this->configurationManager;
}

/**
* Sets the configuration manager.
*
* @param configurationManager The configuration manager.
*/
void Engine::setConfigurationManager(ConfigurationManager *configurationManager) {
    this->configurationManager = configurationManager;
}

/**
* Retrieves the console manager.
*
* @return The console manager.
*/
ConsoleManager *Engine::getConsoleManager() {
    return this->consoleManager;
}

/**
* Sets the console manager.
*
* @param consoleManager The console manager.
*/
void Engine::setConsoleManager(ConsoleManager *consoleManager) {
    this->consoleManager = consoleManager;
}


/**
* Retrieves the logger.
*
* @return The logger.
*/
Logger *Engine::getLogger() {
    return this->logger;
}

/**
* Sets the logger.
*
* @param logger The logger.
*/
void Engine::setLogger(Logger *logger) {
    this->logger = logger;
}

/**
* Retrieves the render.
*
* @return The render.
*/
SceneNode *Engine::getRender() {
    return this->render;
}

/**
* Sets the render.
*
* @param render The render.
*/
void Engine::setRender(SceneNode *render) {
    this->render = render;
}

/**
* Retrieves the render 2d.
*
* @return The render 2d.
*/
Scene2dNode *Engine::getRender2d() {
    return this->render2d;
}

/**
* Sets the render 2d.
*
* @param render2d The render 2d.
*/
void Engine::setRender2d(Scene2dNode *render2d) {
    this->render2d = render2d;
}

/**
* Retrieves the active physics engine.
*
* @return The active physics engine.
*/
PhysicsEngine *Engine::getActivePhysicsEngine() {
    return this->activePhysicsEngine;
}

/**
* Sets the active physics engine.
*
* @param activePhysicsEngine The active physics engine.
*/
void Engine::setActivePhysicsEngine(PhysicsEngine *activePhysicsEngine) {
    this->activePhysicsEngine = activePhysicsEngine;
}

int Engine::getArgc() {
    return this->argc;
}

char **Engine::getArgv() {
    return this->argv;
}
