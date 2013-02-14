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
    Import and definition of NETSIM exported variables. 
*****************************************************************************************/

#ifndef NETSIM_SHARED_MEMORY
#define NETSIM_SHARED_MEMORY

#pragma once

// include interfacing helpers and linked corsim.dll
#include "corsim.h"

////////////////////////////////////////////////////////////////////////////////
// scalars definitions
////////////////////////////////////////////////////////////////////////////////
#define IMXNOD 8999                                                                         // maximum allowable number of nodes in a network

////////////////////////////////////////////////////////////////////////////////
// netsim scalar non-array variables, directly exported
////////////////////////////////////////////////////////////////////////////////
DLL_IMPORT int NETSIM_DETECTORS_mp_NUMDET;                                                  
#define net_det_num NETSIM_DETECTORS_mp_NUMDET                                              // actual number of detectors on the network

DLL_IMPORT int NETSIM_DETECTORS_mp_IMXDET;                      
#define net_det_max_num NETSIM_DETECTORS_mp_IMXDET                                          // maximum allowable number of detectors in a network

DLL_IMPORT struct
{
   char linfname[512];
   char loutfname[512];
   int linflen;
   int loutflen;
} LIOFILES;
#define file_path LIOFILES.linfname                                                         // input .trf file name
#define file_path_len LIOFILES.linflen                                                      // input .trf file name length

////////////////////////////////////////////////////////////////////////////////
// netsim scalar non-array variables, exported through fortran common blocks
////////////////////////////////////////////////////////////////////////////////
DLL_IMPORT struct { int CLOCK; } SIN104;
#define net_clock SIN104.CLOCK                                                              // current time since start of the simulation in seconds

DLL_IMPORT struct { int TTLNK; } SIN116;
#define ttlnk SIN116.TTLNK                                                                  // TODO: comment and refactor

DLL_IMPORT struct { int TTLVEH; } SIN117;
#define net_veh_max_id SIN117.TTLVEH                                                        // highest vehicle id used so far in this subnetwork

DLL_IMPORT struct { int YINIT; } GLR091;
#define is_init GLR091.YINIT                                                                // initialization flag: TRUE during initialization, FALSE when it has been completed

////////////////////////////////////////////////////////////////////////////////
// netsim scalar non-array variables, directly exported
////////////////////////////////////////////////////////////////////////////////
DLL_IMPORT int* NETSIM_DETECTORS_mp_DTLANE;                                                 
#define net_det_lane NETSIM_DETECTORS_mp_DTLANE                                             // lane numbers (two entries always allocated) in which each detector is located

DLL_IMPORT int* NETSIM_DETECTORS_mp_DTMOD;              
#define net_det_mod NETSIM_DETECTORS_mp_DTMOD                                               // bit 1-3: detector type, bit 4-10: vehicle speed, bit 11-32: cumulative vehicle count

DLL_IMPORT int* NETSIM_DETECTORS_mp_DTNLNK;            
#define net_det_link NETSIM_DETECTORS_mp_DTNLNK                                             // link number surveillance detector is on

DLL_IMPORT int* NETSIM_DETECTORS_mp_DTPOS;                                                  
#define net_det_pos NETSIM_DETECTORS_mp_DTPOS                                               // distance (tenths of foot) between the downstream edge and the downstream stop-bar

DLL_IMPORT int* NETSIM_DETECTORS_mp_DETID;              
#define net_det_id NETSIM_DETECTORS_mp_DETID                                                // detector identifiers (surveillance detectors only)

DLL_IMPORT int* NETSIM_DETECTORS_mp_DETON;              
#define net_det_on NETSIM_DETECTORS_mp_DETON                                                // bit #i: (0, 1) if detector was (on, off) for the #i-th tenth of second (tot bits: 10)

DLL_IMPORT int* NETSIM_VEHICLES_mp_WDTYPE;
#define net_det_is_surveillance NETSIM_VEHICLES_mp_WDTYPE                                   // flag (T, F) if this detector (is, is not) a surveillance detector

DLL_IMPORT int* NETSIM_VEHICLES_mp_NETGVH;
#define net_veh_id NETSIM_VEHICLES_mp_NETGVH                                                // global netsim vehicle id

DLL_IMPORT int* NETSIM_LINKS_mp_DWNOD;
#define net_lnk_dn NETSIM_LINKS_mp_DWNOD                                                    // TODO: commentare

DLL_IMPORT int* NETSIM_LINKS_mp_UPNOD;
#define net_lnk_up NETSIM_LINKS_mp_UPNOD                                                    // TODO: commentare

DLL_IMPORT int* NETSIM_LINKS_mp_DTFLNK;
#define net_lnk_lnk NETSIM_LINKS_mp_DTFLNK                                                  // TODO: commentare

////////////////////////////////////////////////////////////////////////////////
// netsim statically allocated arrays, exported through fortran common blocks
////////////////////////////////////////////////////////////////////////////////
DLL_IMPORT struct { int NMAP[IMXNOD]; } SIN075;
#define net_node_num SIN075.NMAP                                                            // user-specified node number

////////////////////////////////////////////////////////////////////////////////
// netsim exported functions
////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllimport) int __stdcall GetNetsimVehicleID(int);                     
#define get_net_vehicle_id(int) GetNetsimVehicleID(int)                                     // TODO: comment 

extern "C" __declspec(dllimport) void __stdcall abortcorsim(void);                          // safely abort corsim

#endif // NETSIM_SHARED_MEMORY