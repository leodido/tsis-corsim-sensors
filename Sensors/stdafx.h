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
    Inclusion of MFC components.
*****************************************************************************************/

#ifndef STDAFX
#define STDAFX

#pragma once

#pragma warning(push)           // start disable warnings
#pragma warning(disable: 4244)  // possible loss of data

#include <afx.h>
#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions
#include <afxtempl.h>           // MFC templates

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>			// MFC automation classes
#endif // _AFX_NO_OLE_SUPPORT

#pragma warning(pop)            // stop disable warnings

#endif // STDAFX
