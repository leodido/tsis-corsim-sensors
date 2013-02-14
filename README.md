# Sensors

This is a TSIS-CORSIM run-time extension ([RTE][]).

This repository contains code to compile a **DLL** that extends, at runtime, [TSIS-CORSIM][] (version [6.2][]), the _"Traffic Software Integrated System - Corridor Simulation system"_.

## Goal

To extend TSIS-CORSIM in order to provide the **tracking** and the **logging** of **fine-grained data of the detectors in each traffic simulation model created** with the aforementioned simulator.

## Dependecies

This code depends on, as well as `corsim.dll` and `corwin.lib` (included in the source), from the **MFC** libraries (because most of classes inherit from its `CObject` class).

## Microsof Visual Studio C++ 2010

This project is written in Visual C++ and contained in a Microsoft Visual Studio C++ 2010 solution project.

### Configuration

To install and use **MFC** classes adapt this [guide][].

Then in the project general settings:

+	set _Use of MFC_ to _Use MFC in a shared DLL_
+	set _Use of ATL_ to _Dynamic linking to ATL_

While in the project VC++ directory settings:

+	add to _Inclusion directories_ the paths of installed **WinDDK**:

	`%WinDDK%\inc\atl71`

	`%WinDDK%\inc\mfc42`


+	add to _LIbrary directories_ this paths:

	`%WinDDK%\lib\ATL\i386`

	`%WinDDK%\lib\Mfc\i386`


Note that `%WinDDK%` is the path of your local **WinDDK** installation (i.e. mine is `C:\WinDDK\7600.16385.1`).

Finally in the linker input settings:

+	add to _Additional dependecies_ one row for `corsim.lib` and one for `corwin.lib`

[guide]: http://bit.ly/mfc-in-visual-c-express
[RTE]: http://mctrans.ce.ufl.edu/featured/tsis/version6/CORSIMRTE.htm
[6.2]: http://mctrans.ce.ufl.edu/featured/tsis/
[TSIS-CORSIM]: http://ops.fhwa.dot.gov/trafficanalysistools/corsim.htm