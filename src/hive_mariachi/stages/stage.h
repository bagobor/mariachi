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

#include "../main/main.h"

namespace mariachi {
    class Stage : public Module {
        private:
            std::string name;

            inline void initThread();
            inline void initEngine(Engine *engine);
            inline void initName(std::string name);

        protected:
            Engine *engine;
            bool thread;

        public:
            Stage();
            Stage(Engine *engine);
            Stage(Engine *engine, const std::string &name);
            ~Stage();
            virtual void start(void *arguments);
            virtual void stop(void *arguments);
            virtual void update(void *arguments);
            virtual void debug(const std::string &value) { this->engine->getLogger()->debug(this->formatLoggerValue(value)); };
            virtual void info(const std::string &value) { this->engine->getLogger()->info(this->formatLoggerValue(value)); };
            virtual void warning(const std::string &value) {};
            virtual void fault(const std::string &value) {};
            virtual void critical(const std::string &value) {};
            virtual std::string formatLoggerValue(const std::string &value);
            bool requiresThread();
            std::string &getName();
            void setName(std::string &name);
            Engine *getEngine();
            void setEngine(Engine *engine);
    };
}
