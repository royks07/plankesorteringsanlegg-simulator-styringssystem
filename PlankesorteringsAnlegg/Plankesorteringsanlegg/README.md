README                         {#mainpage}
======

This software, the "Educational simulator for control-system development" is written as part of a programming-project 
in the course DAT220 at the University of Agder (UiA), Norway.

The aim of the project is to create simulation-environments that can be used in educating students in control-
system development.


###The project contains 3 main logical components:

*	A simulator 
*	A communicator
*	An "Arduino Mega 2560"-based API


####Simulator

For the time being the simulator contains only one SimulatorPage. This SimulatorPage contains a
simulated PlankSortingPlant.

The simulator component is built on top of Erin Cattos Box2D 2.2.1.


####Communicator

The communicator is a class that is able to read and write simulated sensor and actuator data over a serial-
port.

The communicator component is built on top of the Boost.Asio-library.


####API

The simulator-API lets students write <b>control-systems</b> for simulated environments.

Tha API is based on Paul Bjørn Andersens(UiA) RTOS-kernel, TSS, and his usart-library. 


File manifest
-------------
*	doc/							-	This folder contains the project documentation.
*	Main.cpp						-	Create the GUI
*	Render.cpp
*	Render.h
*	SimulatorPage.cpp
*	SimulatorPage.h
*	PlankSortingPlant.cpp
*	PlankSortingPlant.h
*	SimulatorPageEntries.cpp
*	Actuators.cpp
*	Actuators.h
*	Communicator.cpp
*	Communicator.h
*	CommandSequenceInterpreter.cpp
*	CommandSequenceInterpreter.h
*	Conveyor.cpp
*	Conveyor.h
*	ConveyorSynchronizer.cpp
*	ConveyorSynchronizer.h
*	Package.cpp
*	Package.h
*	Packaging.cpp
*	Packaging.h
*	Sensors.cpp
*	Sensors.h
*	Storages.cpp
*	Storages.h
*	Text.h
*	Text.cpp
*	UserData.cpp
*	UserData.h


Configuration instructions
--------------------------


Installation instructions
-------------------------


Operating instructions
----------------------

###Example:


Copyright and licensing information
-----------------------------------


Contact information for the distributor or programmer
-----------------------------------------------------

Known bugs[1]
-------------

Troubleshooting[1]
------------------


Credits and acknowledgments
---------------------------

Changelog
---------

...


News
----

...
