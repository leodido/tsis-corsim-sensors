/*****************************************************************************************
    Copyright 2013 Leonardo Di Donato

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and 
    limitations under the License.

    Application: tsis-corsim-sensors

    Description:
    Entry point header file.
*****************************************************************************************/

#ifndef SENSORS
#define SENSORS

#pragma once

extern bool is_log_active;

// declare global variables related to time
extern int end_of_init;
extern int prev_init;
extern int prev_time;

// helper function for textual date and time
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#define DTTMFMT "_%Y%m%d_%H%M%S"
#define DTTMSZ 17 // must allow extra character for the null terminator

static char* getDateTime(char* buff) {
    time_t t = time (0);
    strftime(buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}

#endif // SENSORS