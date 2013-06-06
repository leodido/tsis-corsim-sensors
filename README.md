# SENSORS

This is a [TSIS-CORSIM][]™ RTE ([run-time extension][]).

This repository contains code to compile a **DLL** that extends, at runtime, TSIS-CORSIM™ (version [>=6.2][]), the _"Traffic Software Integrated System - Corridor Simulation system"_.

## GOAL

To extend TSIS-CORSIM™ in order to provide the **tracking** and the **logging** of **fine-grained data of the detectors in each traffic simulation model created** with the aforementioned simulator.

## DEPENDENCIES

This code depends on, as well as `corsim.dll` and `corwin.lib` (included in the source), from the **MFC** and **ATL** libraries (because most of classes inherit from its `CObject` class).

## MS VISUAL STUDIO C++ 2010

This project is written in Visual C++ and contained in a Microsoft Visual Studio C++ 2010 solution project.

### CONFIGURATION

To install and use **MFC** classes adapt this [guide][]. Futher informations are available [here][].

Then in the project general settings:

+	set _Use of MFC_ to _Use MFC in a shared DLL_
+	set _Use of ATL_ to _Dynamic linking to ATL_

While in the project VC++ directory settings:

+	add to _Inclusion directories_ this paths:

	`$(DDK_directory)\inc\atl71`

	`$(DDK_directory)\inc\mfc42`

and

+	add to _Library directories_ this paths:

	`$(DDK_directory)\lib\ATL\i386`

	`$(DDK_directory)\lib\Mfc\i386`

Finally in the linker input settings:

+	add to _Additional dependecies_ one row for `corsim.lib` and one for `corwin.lib`

Note that `$(DDK_directory)` is the path of your local **WinDDK** installation, i.e. `C:\WinDDK\7600.16385.1`.

[guide]: http://bit.ly/mfc-in-visual-c-express
[here]: http://www.hard-light.net/forums/index.php?topic=80725.0
[run-time extension]: http://mctrans.ce.ufl.edu/featured/tsis/version6/CORSIMRTE.htm
[>=6.2]: http://mctrans.ce.ufl.edu/featured/tsis/
[TSIS-CORSIM]: http://ops.fhwa.dot.gov/trafficanalysistools/corsim.htm