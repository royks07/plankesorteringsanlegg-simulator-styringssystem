/** \file Main.h,
 * 	\brief Main.h and Main.cpp contains the main-function, and code to create user-interface and run the simulation-loop.
 *
 *	Based on Erin Cattos Box2D 2.2.1 Testbed Main.cpp
 * */

#ifndef MAIN_H_
#define MAIN_H_

#include "glui/glui.h"
#include <cstdio>
#include <iostream>

#include "Communicator.h"
#include "Render.h"
#include "SimulatorPage.h"

using namespace std;

static void Resize(int32 w, int32 h);/**< Set the dimensions of the view-port around the viewcenter*/
static b2Vec2 ConvertScreenToWorld(int32 x, int32 y);/**< Convert screen coordinates to world coordinates*/
static void Timer(int);/**< Control the frame-rate*/
static void SimulationLoop();/**< Run the simulation loop*/
static void Keyboard(unsigned char key, int x, int y);
static void KeyboardSpecial(int key, int x, int y);
static void KeyboardUp(unsigned char key, int x, int y);
static void Mouse(int32 button, int32 state, int32 x, int32 y);
static void MouseMotion(int32 x, int32 y);
static void MouseWheel(int wheel, int direction, int x, int y);
static void Restart(int);/**< Restart SimulatorPage*/
static void Pause(int);/**< Toggle pause*/
static void Exit(int code);/**< Exit the program*/
static void SingleStep(int);/**< Run the simulation loop one step at the time*/
static void SetSerialPort();/**< Update serialport and restart SimulatorPage*/
static void CloseAbout();/**< Close the About window*/
static void About();/**< Open About window*/
static void CloseHelp();/**< Close the Help window*/
static void Help();/**< Open the About window*/
int main(int argc, char** argv);/**< Start program execution*/

namespace{
	int32 simulatorPageIndex = 0;
	int32 simulatorPageSelection = 0;
	int32 simulatorPageCount = 0;
	SimulatorPageEntry* entry;
	SimulatorPage* simulatorPage;
	Settings settings;
	int32 width = 1200;
	int32 height = 600;
	int32 framePeriod = 16;
	int32 mainWindow;
	float settingsHz = 60.0;
	GLUI *glui;
	float32 viewZoom = 1.0f;
	int tx, ty, tw, th;
	bool rMouseDown;
	b2Vec2 lastp;
	char serialport[32]="COM1";
	Communicator* communicator;
	GLUI* aboutWindow;
	GLUI* helpWindow;
}

#endif /* MAIN_H_ */
