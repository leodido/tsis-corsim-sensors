/*******************************************************************************
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
    Definition of the CORWIN API.
********************************************************************************/

#ifndef CORSIM_WINDOWS_INTERFACE
#define CORSIM_WINDOWS_INTERFACE

#pragma once

#ifdef CORWIN_IMPLEMENTATION
#define CORWINAPI __declspec(dllexport)
#else
#define CORWINAPI __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////////////
// simulation message codes
////////////////////////////////////////////////////////////////////////////////
#define SIM_YIELD            0x0000  // yield time
#define SIM_CALLRTE          0x0001  // call RTE function
#define SIM_GETKEYMESSAGE    0x0005  // get user keyboard input
#define SIM_ADDVEHICLE       0x0008  // add vehicle to message file
#define SIM_LINKSTATEVALID   0x0009  // add link to message file
#define SIM_STATUS           0x000a  // status of simulation
#define SIM_ADDRAMPMETER     0x000b  // add ramp meter to message file
#define SIM_LINKTIDVALID     0x000c  // current time interval data valid for link
#define SIM_ADDINCIDENT      0x000d  // add incident to message file
#define SIM_SECTIONDATAREADY 0x000f  // section data valid

////////////////////////////////////////////////////////////////////////////////
// simulation status codes
////////////////////////////////////////////////////////////////////////////////
#define SIM_NORMAL                   0x0000  // ok
#define SIM_ABORT                    0x0001  // aborting
#define SIM_TIMESTEPCOMPLETE         0x0002  // time step complete
#define SIM_LINKCOMPLETE             0x0003  // link data valid
#define SIM_INVARIANTPROCESSCOMPLETE 0x0004  // invariant data valid
#define SIM_BEGININTERVAL            0x0005  // start new time interval
#define SIM_INTERVALCOMPLETE         0x0006  // end current time interval
#define SIM_BEGINTIMESTEP            0x0007  // start new time step
#define SIM_COMPLETE                 0x0008  // simulation is finished
#define SIM_BEGINSIMULATION          0x0009  // start of simulation data
#define SIM_TIMEPERIODCOMPLETE       0x000a  // end current time period
#define SIM_TIMEPERIOD_VALIDATION    0x000f  // time period validation complete

////////////////////////////////////////////////////////////////////////////////
// colors
////////////////////////////////////////////////////////////////////////////////
#define SIM_COLOR_RGB     0x0000 // use RGB value
#define SIM_COLOR_INFO    0x0001 // normal
#define SIM_COLOR_WARNING 0x0002 // warning
#define SIM_COLOR_ERROR   0x0003 // error

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// send message
////////////////////////////////////////////////////////////////////////////////
CORWINAPI void __stdcall SendMsg(int msgID, int msgCode);

////////////////////////////////////////////////////////////////////////////////
// output string
////////////////////////////////////////////////////////////////////////////////
CORWINAPI void __stdcall OutputString(const char *str, unsigned int size, int msgCode, unsigned long color);

////////////////////////////////////////////////////////////////////////////////
// message box
////////////////////////////////////////////////////////////////////////////////
CORWINAPI void __stdcall MsgBox(const char *message, unsigned int msize, const char *title, unsigned int tsize);

////////////////////////////////////////////////////////////////////////////////
// request keyboard input
////////////////////////////////////////////////////////////////////////////////
CORWINAPI void __stdcall RequestKeyInput(const char *prompt, unsigned int size);

#ifdef __cplusplus
}
#endif // extern C

#if defined(_MSC_VER) && !defined(CORWIN_IMPLEMENTATION)
// the following pragma causes the linker to search for the corwin.lib library while linking (first in the current working directory and then in the path specified in the LIB environment variable)
#pragma comment(lib, "corwin")
#endif

#if defined(CORWIN_IMPLEMENTATION) || defined(NOT_RTE)
////////////////////////////////////////////////////////////////////////////////
// message identifiers
////////////////////////////////////////////////////////////////////////////////
#define WM_SIMMESSAGE (WM_USER + 100)
#define WM_OUTPUTMSG  (WM_USER + 102)

////////////////////////////////////////////////////////////////////////////////
// this is a color code, not an RGB value
// flag used by WM_OUTPUTMSG messages
////////////////////////////////////////////////////////////////////////////////
#define SIM_COLOR_CODE 0xff00

////////////////////////////////////////////////////////////////////////////////
// set window handle
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
#endif
CORWINAPI int __stdcall SetHWND(HWND hwnd);

#endif // CORWIN_IMPLEMENTATION || NOT_RTE

#endif // CORSIM_WINDOWS_INTERFACE
