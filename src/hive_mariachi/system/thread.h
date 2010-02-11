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

#ifdef MARIACHI_PLATFORM_WIN32
#define THREAD_VALID_RETURN_VALUE 0
#define THREAD_INVALID_RETURN_VALUE 1
#define THREAD_RETURN DWORD WINAPI
#define THREAD_ARGUMENTS LPVOID
#define THREAD_HANDLE HANDLE
#define THREAD_IDENTIFIER DWORD
#define THREAD_REFERENCE THREAD_HANDLE
#define THREAD_CREATE(threadAttributes, stackSize, startAddress, parameter, creationFlags, threadId) CreateThread(threadAttributes, stackSize, startAddress, (THREAD_ARGUMENTS) parameter, creationFlags, &threadId)
#define THREAD_CREATE_BASE(threadId, startAddress, parameter) THREAD_CREATE(NULL, 0, startAddress, (THREAD_ARGUMENTS) parameter, 0, threadId)
#define THREAD_JOIN(threadHandle) WaitForSingleObject(threadHandle, INFINITE)
#define THREAD_JOIN_BASE(threadHandle, threadIdentifier) THREAD_JOIN(threadHandle)
#define THREAD_CLOSE(threadHandle) CloseHandle(threadHandle)
#define MUTEX_HANDLE HANDLE
#define MUTEX_CREATE(mutexHandle) mutexHandle = CreateMutex(NULL, false, NULL)
#define MUTEX_LOCK(mutexHandle) WaitForSingleObject(mutexHandle, INFINITE)
#define MUTEX_UNLOCK(mutexHandle) ReleaseMutex(mutexHandle)
#define MUTEX_CLOSE(mutexHandle) CloseHandle(mutexHandle)
#define CONDITION_HANDLE HANDLE
#define CONDITION_CREATE(conditionHandle) conditionHandle = CreateEvent(NULL, true, false, NULL)
#define CONDITION_WAIT(conditionHandle) WaitForSingleObject(conditionHandle, INFINITE)
#define CONDITION_SIGNAL(conditionHandle) SetEvent(conditionHandle)
#define CONDITION_RESET(conditionHandle) ResetEvent(conditionHandle)
#define CONDITION_CLOSE(conditionHandle) CloseHandle(conditionHandle)
#elif MARIACHI_PLATFORM_UNIX
typedef struct ConditionHandle_t {
    pthread_cond_t condition;
    pthread_mutex_t mutex;
} ConditionHandle;
#define THREAD_VALID_RETURN_VALUE 0
#define THREAD_INVALID_RETURN_VALUE 0
#define THREAD_RETURN void *
#define THREAD_ARGUMENTS void *
#define THREAD_HANDLE int
#define THREAD_IDENTIFIER pthread_t
#define THREAD_REFERENCE THREAD_IDENTIFIER
#define THREAD_CREATION_FUNCTION CreateThread
#define THREAD_CREATE(threadId, creationFlags, startAddress, parameter) pthread_create(&threadId, creationFlags, startAddress, (THREAD_ARGUMENTS) parameter)
#define THREAD_CREATE_BASE(threadId, startAddress, parameter) THREAD_CREATE(threadId, NULL, startAddress, parameter)
#define THREAD_JOIN(threadIdentifier) pthread_join(threadIdentifier, NULL)
#define THREAD_JOIN_BASE(threadHandle, threadIdentifier) THREAD_JOIN(threadIdentifier)
#define THREAD_CLOSE(threadHandle)
#define MUTEX_HANDLE pthread_mutex_t *
#define MUTEX_CREATE(mutexHandle) mutexHandle = (MUTEX_HANDLE) malloc(sizeof(pthread_mutex_t));\
    pthread_mutex_init(mutexHandle, NULL)
#define MUTEX_LOCK(mutexHandle) pthread_mutex_lock(mutexHandle)
#define MUTEX_UNLOCK(mutexHandle) pthread_mutex_unlock(mutexHandle)
#define MUTEX_CLOSE(mutexHandle) pthread_mutex_destroy(mutexHandle);\
    free(mutexHandle)
#define CONDITION_HANDLE ConditionHandle_t *
#define CONDITION_CREATE(conditionHandle) conditionHandle = (CONDITION_HANDLE) malloc(sizeof(ConditionHandle_t));\
    pthread_mutex_init(&conditionHandle->mutex, NULL);\
    pthread_cond_init(&conditionHandle->condition, NULL)
#define CONDITION_WAIT(conditionHandle) pthread_mutex_lock(&conditionHandle->mutex);\
    pthread_cond_wait(&conditionHandle->condition, &conditionHandle->mutex);\
    pthread_mutex_unlock(&conditionHandle->mutex)
#define CONDITION_SIGNAL(conditionHandle) pthread_cond_signal(&conditionHandle->condition)
#define CONDITION_RESET(conditionHandle)
#define CONDITION_CLOSE(conditionHandle) pthread_mutex_destroy(&conditionHandle->mutex);\
    pthread_cond_destroy(&conditionHandle->condition);\
    free(conditionHandle)
#endif
