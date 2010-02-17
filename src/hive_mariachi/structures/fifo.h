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

#include "../system/thread.h"

/**
* The default fifo size.
*/
#define DEFAULT_FIFO_SIZE 1

namespace mariachi {
    namespace structures {
        template<typename T> class Fifo {
            private:

            public:
                bool stopFlag;
                unsigned int size;
                std::deque<T> queue;
                CRITICAL_SECTION_HANDLE queueCriticalSection;
                CONDITION_HANDLE notEmptyCondition;
                CONDITION_HANDLE notFullCondition;

                /**
                * Constructor of the class.
                */
                Fifo() {
                    CRITICAL_SECTION_CREATE(this->queueCriticalSection);
                    CONDITION_CREATE(this->notEmptyCondition);
                    CONDITION_CREATE(this->notFullCondition);
                    this->stopFlag = false;
                    this->size = DEFAULT_FIFO_SIZE;
                }

                Fifo(unsigned int size) {
                    CRITICAL_SECTION_CREATE(this->queueCriticalSection);
                    CONDITION_CREATE(this->notEmptyCondition);
                    CONDITION_CREATE(this->notFullCondition);
                    this->stopFlag = false;
                }

                /**
                * Destructor of the class.
                */
                ~Fifo() {
                    CRITICAL_SECTION_CLOSE(this->queueCriticalSection);
                    CONDITION_CLOSE(this->notEmptyCondition);
                    CONDITION_CLOSE(this->notFullCondition);
                }

                inline void signal(T value) {
                    CRITICAL_SECTION_ENTER(this->queueCriticalSection);

                    while(this->queue.size() == this->size || this->stopFlag) {
                        CONDITION_WAIT(this->notFullCondition, this->queueCriticalSection);
                    }

                    this->queue.push_back(value);

                    CRITICAL_SECTION_LEAVE(this->queueCriticalSection);

                    CONDITION_SIGNAL(this->notEmptyCondition);
                }

                inline T wait() {
                    CRITICAL_SECTION_ENTER(this->queueCriticalSection);

                    // in case the queue is empty (waits or the signal)
                    if(this->queue.empty()) {
                        CONDITION_WAIT(this->notEmptyCondition, this->queueCriticalSection);
                    }

                    // retrieves the front value
                    T frontValue = this->queue.front();

                    // pops the first value in the queue
                    this->queue.pop_front();

                    CRITICAL_SECTION_LEAVE(this->queueCriticalSection);

                    CONDITION_SIGNAL(this->notFullCondition);

                    // returns the front value
                    return frontValue;
                }

                inline void start() {
                    this->stopFlag = false;
                }

                inline void stop() {
                    this->stopFlag = true;
                }
        };
    }
}
