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
    Definition of macros that make easier the process (import and export)
    of interfacing between CORSIM and a RTE.
********************************************************************************/

#ifndef CORSIM_RTE_INTERFACE
#define CORSIM_RTE_INTERFACE

#pragma once

#ifdef __cplusplus

// when compiling c++ code, the extern "c" directive is required to support the c-style interfaces between corsim and the run time extension
#define DLL_IMPORT extern "C" __declspec(dllimport)
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else

// when compiling c code, there is no need for the extern "c" directive
#define DLL_IMPORT extern __declspec(dllimport)
#define DLL_EXPORT __declspec(dllexport)

#endif // __cplusplus

#endif // CORSIM_RTE_INTERFACE
