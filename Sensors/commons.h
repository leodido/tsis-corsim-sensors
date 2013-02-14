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
    Definition of global variables only related to the RTE code.
*****************************************************************************************/

#ifndef COMMONS_SENSORS
#define COMMONS_SENSORS

#pragma once

// define a color (forest green) for display messages issued by the RTE
#define RTE_MESSAGE_RGB 0x00116400L

// define a global character string used for displaying messages issued by the RTE
extern char out_buf[132];

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

#define DTTMFMT " %Y%m%d_%H%M%S"
#define DTTMSZ 17 // must allow extra character for the null terminator

static char* getDateTime(char* buff) {
    time_t t = time (0);
    strftime(buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}

#endif // COMMONS_SENSORS