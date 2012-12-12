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

The simulator-API lets students write control-systems for simulated environments.

Tha API is based on Paul Bjørn Andersens(UiA) RTOS-kernel, TSS, and his usart-library. 


File manifest
-------------

* doc/ - This folder contains the project documentation.
* Actuators.cpp	
* Actuators.h - Contains actuator-hierarchy and actuator-set
* CommandSequenceInterpreter.cpp	
* CommandSequenceInterpreter.h - Contains the CommandSequanceInterpreter-class
* Communicator.cpp	
* Communicator.h - Contains the Communicator-class
* Conveyor.cpp	
* Conveyor.h - Contains the Conveyor-class and the classes needed to build it
* ConveyorSynchronizer.cpp	
* ConveyorSynchronizer.h - Contains the class which synchronize conveyors
* Main.cpp	
* Main.h - Main.h and Main.cpp contains the main-function, and code to create user-interface and run the simulation-loop
* Package.cpp	
* Package.h - Contains the classes related to planks, "sprinkles" and packages
* Packaging.cpp	
* Packaging.h - Contains the PackageInput- and PackageOutput-class
* PlankSortingPlant.cpp	
* PlankSortingPlant.h - Contains the SimulationPage to create a "plank sorting plant"
* Render.cpp	
* Render.h	
* Sensors.cpp	
* Sensors.h - Contains the sensor class-hierarchy and the sensor-set
* SimulatorPage.cpp	
* SimulatorPage.h	
* SimulatorPageEntries.cpp - Assign simulator-page entries to global array g_simulatorPageEntries
* Storages.cpp	
* Storages.h - Define classes to create individual storages and storage area
* Text.cpp	
* Text.h - Contains a function to draw text on SimulatorPage
* UserData.cpp	
* UserData.h - Contains the UserData class-hierarchy

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
