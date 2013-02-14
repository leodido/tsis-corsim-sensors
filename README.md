# Sensors

ciao

## Dependecies

This code depends on, as well as `corsim.dll` and `corwin.lib` (included in the source), from the **MFC** libraries (because some classes inherit from its `CObject` class).

## Microsof Visual Studio C++ 2010

This project is written in Visual C++ and contained in a Microsoft Visual Studio C++ 2010 solution project.

### Configuration

To install and use **MFC** classes adapt this [guide][].

Then in the project general settings:

+ set _Use of MFC_ to _Use MFC in a shared DLL_
+ set _Use of ATL_ to _Dynamic linking to ATL_

While in the project VC++ directory settings:

+ add to _Inclusion directories_ the paths of installed **WinDDK**:
++ %WinDDK%\inc\atl71
++ %WinDDK%\inc\mfc42
+ add to _LIbrary directories_ this paths:
++ %winDDK%\lib\ATL\i386
++ %winDDK%\lib\Mfc\i386

Note that %winDDK% is the path of your local **WinDDK** installation (i.e. mine is: C:\WinDDK\7600.16385.1).

Finally in the linker input settings:

+ add to _additional dependecies_ two rows, containing `corsim.lib` and `corwin.lib`

[guide]: http://bit.ly/mfc-in-visual-c-express