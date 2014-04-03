SENSORS [![Analytics](https://ga-beacon.appspot.com/UA-49657176-1/tsis-corsim-sensors)](https://github.com/igrigorik/ga-beacon)
=================

This is a [TSIS-CORSIM][]™ (Traffic Software Integrated System - Corridor Simulation) RTE ([run-time extension][]).

This repository contains code to compile a **DLL** that extends, at runtime, TSIS-CORSIM™ ([version >= 6.2][]).

## TSIS-CORSIM™

TSIS™ is an IDE that enables users to conduct traffic operations analysis. Built using a component architecture, TSIS™ is a toolbox that contains tools that allow the user to define and manage traffic analysis projects, define traffic networks and create inputs for traffic simulation analysis, execute traffic simulation models, and interpret the results of those models.

CORSIM™ is a comprehensive microscopic traffic simulation, applicable to surface streets, freeways, and integrated networks with a complete selection of control devices. It simulates traffic and traffic control systems using commonly accepted vehicle and driver behavior models. CORSIM™ combines and extends two of the most widely used traffic simulation models, NETSIM for surface streets, and FRESIM for freeways.

## GOAL

To extend TSIS-CORSIM™ in order to provide the **tracking** and the **logging** of **fine-grained data of the detectors of a simulation model** created and simulated with the aforementioned toolbox.

### NOTES

To generate the (correct) temporal log of all the transitions of the vehicles on the detectors of the road network it is necessary that the detectors are of the type "presence".

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
[version >= 6.2]: http://mctrans.ce.ufl.edu/featured/tsis/
[TSIS-CORSIM]: http://ops.fhwa.dot.gov/trafficanalysistools/corsim.htm
