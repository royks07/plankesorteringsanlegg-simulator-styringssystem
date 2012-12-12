
#include "Main.h"

void Resize(int32 w, int32 h){
	width = w;
	height = h;

	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float32 ratio = float32(tw) / float32(th);

	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	// L/R/B/T
	gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}

b2Vec2 ConvertScreenToWorld(int32 x, int32 y){
	float32 u = x / float32(tw);
	float32 v = (th - y) / float32(th);

	float32 ratio = float32(tw) / float32(th);
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	b2Vec2 p;
	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
	return p;
}

// This is used to control the frame rate (60Hz).
void Timer(int){
	glutSetWindow(mainWindow);
	glutPostRedisplay();
	glutTimerFunc(framePeriod, Timer, 0);
}

void SimulationLoop(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	simulatorPage->SetTextLine(30);
	b2Vec2 oldCenter = settings.viewCenter;
	settings.hz = settingsHz;
	simulatorPage->Step(&settings);
	if (oldCenter.x != settings.viewCenter.x || oldCenter.y != settings.viewCenter.y)
	{
		Resize(width, height);
	}

	simulatorPage->DrawTitle(5, 15, entry->name);

	glutSwapBuffers();

	if (simulatorPageSelection != simulatorPageIndex)
	{
		simulatorPageIndex = simulatorPageSelection;
		delete simulatorPage;
		entry = g_simulatorPageEntries + simulatorPageIndex;
		simulatorPage = entry->createFcn(communicator);
		viewZoom = 1.0f;
		settings.viewCenter.Set(0.0f, 20.0f);
		Resize(width, height);
	}
}

void Keyboard(unsigned char key, int x, int y){
	B2_NOT_USED(x);
	B2_NOT_USED(y);

	switch (key)
	{
	case 27:
#ifndef __APPLE__
		// freeglut specific function
		glutLeaveMainLoop();
#endif
		exit(0);
		break;

		// Press 'z' to zoom out.
	case 'z':
		viewZoom = b2Min(1.1f * viewZoom, 20.0f);
		Resize(width, height);
		break;

		// Press 'x' to zoom in.
	case 'x':
		viewZoom = b2Max(0.9f * viewZoom, 0.02f);
		Resize(width, height);
		break;

		// Press 'r' to reset.
	case 'r':
		delete simulatorPage;
		simulatorPage = entry->createFcn(communicator);
		break;

		// Press space to launch a bomb.
	case ' ':
		if (simulatorPage)
		{
			simulatorPage->LaunchBomb();
		}
		break;

	case 'p':
		settings.pause = !settings.pause;
		break;

		// Press [ to prev simulatorPage.
	case '[':
		--simulatorPageSelection;
		if (simulatorPageSelection < 0)
		{
			simulatorPageSelection = simulatorPageCount - 1;
		}
		glui->sync_live();
		break;

		// Press ] to next simulatorPage.
	case ']':
		++simulatorPageSelection;
		if (simulatorPageSelection == simulatorPageCount)
		{
			simulatorPageSelection = 0;
		}
		glui->sync_live();
		break;

	default:
		if (simulatorPage)
		{
			simulatorPage->Keyboard(key);
		}
	}
}

void KeyboardSpecial(int key, int x, int y){
	B2_NOT_USED(x);
	B2_NOT_USED(y);

	switch (key)
	{
	case GLUT_ACTIVE_SHIFT:
		// Press left to pan left.
	case GLUT_KEY_LEFT:
		settings.viewCenter.x -= 0.5f;
		Resize(width, height);
		break;

		// Press right to pan right.
	case GLUT_KEY_RIGHT:
		settings.viewCenter.x += 0.5f;
		Resize(width, height);
		break;

		// Press down to pan down.
	case GLUT_KEY_DOWN:
		settings.viewCenter.y -= 0.5f;
		Resize(width, height);
		break;

		// Press up to pan up.
	case GLUT_KEY_UP:
		settings.viewCenter.y += 0.5f;
		Resize(width, height);
		break;

		// Press home to reset the view.
	case GLUT_KEY_HOME:
		viewZoom = 1.0f;
		settings.viewCenter.Set(0.0f, 20.0f);
		Resize(width, height);
		break;
	}
}

void KeyboardUp(unsigned char key, int x, int y){
	B2_NOT_USED(x);
	B2_NOT_USED(y);

	if (simulatorPage)
	{
		simulatorPage->KeyboardUp(key);
	}
}

void Mouse(int32 button, int32 state, int32 x, int32 y){
	// Use the mouse to move things around.
	if (button == GLUT_LEFT_BUTTON)
	{
		int mod = glutGetModifiers();
		b2Vec2 p = ConvertScreenToWorld(x, y);
		if (state == GLUT_DOWN)
		{
			b2Vec2 p = ConvertScreenToWorld(x, y);
			if (mod == GLUT_ACTIVE_SHIFT)
			{
				simulatorPage->ShiftMouseDown(p);
			}
			else
			{
				simulatorPage->MouseDown(p);
			}
		}

		if (state == GLUT_UP)
		{
			simulatorPage->MouseUp(p);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			lastp = ConvertScreenToWorld(x, y);
			rMouseDown = true;
		}

		if (state == GLUT_UP)
		{
			rMouseDown = false;
		}
	}
}

void MouseMotion(int32 x, int32 y){
	b2Vec2 p = ConvertScreenToWorld(x, y);
	simulatorPage->MouseMove(p);

	if (rMouseDown)
	{
		b2Vec2 diff = p - lastp;
		settings.viewCenter.x -= diff.x;
		settings.viewCenter.y -= diff.y;
		Resize(width, height);
		lastp = ConvertScreenToWorld(x, y);
	}
}

void MouseWheel(int wheel, int direction, int x, int y){
	B2_NOT_USED(wheel);
	B2_NOT_USED(x);
	B2_NOT_USED(y);
	if (direction > 0)
	{
		viewZoom /= 1.1f;
	}
	else
	{
		viewZoom *= 1.1f;
	}
	Resize(width, height);
}

void Restart(int){
	delete simulatorPage;
	entry = g_simulatorPageEntries + simulatorPageIndex;
	simulatorPage = entry->createFcn(communicator);
    Resize(width, height);
}

void Pause(int){
	settings.pause = !settings.pause;
}

static void Exit(int code){
	// TODO: freeglut is not building on OSX
#ifdef FREEGLUT
	glutLeaveMainLoop();
#endif
	exit(code);
}

void SingleStep(int){
	settings.pause = 1;
	settings.singleStep = 1;
}

void SetSerialPort(){
	cout<<serialport<<endl;
	communicator->close();
	communicator = new Communicator(serialport);
	Restart(0);
}

static void CloseAbout(){
	aboutWindow->close();
}

static void About(){
	//if(aboutWindow!=NULL) aboutWindow->close();
	aboutWindow=GLUI_Master.create_glui("About",0,100,100);
	GLUI_Panel* aboutPanel=aboutWindow->add_panel("About",GLUI_PANEL_NAME_DROP);
	aboutWindow->add_statictext_to_panel(aboutPanel,"This program was written as part of a project in ");
	aboutWindow->add_statictext_to_panel(aboutPanel,"the course DAT220 at the University of Agder, Norway.");
	aboutWindow->add_statictext_to_panel(aboutPanel,"You can contact the author, Roy Kollen Svendsen,");
	aboutWindow->add_statictext_to_panel(aboutPanel,"through his e-mail address <<royks07@student.uia.no>>");
	aboutWindow->add_statictext_to_panel(aboutPanel,"");
	aboutWindow->add_separator_to_panel(aboutPanel);
	aboutWindow->add_statictext_to_panel(aboutPanel,"");
	aboutWindow->add_statictext_to_panel(aboutPanel,"Libraries used:");
	aboutWindow->add_statictext_to_panel(aboutPanel,"-Box2D");
	aboutWindow->add_statictext_to_panel(aboutPanel,"-Boost.Asio");
	aboutWindow->add_statictext_to_panel(aboutPanel,"-FreeGLUT");
	aboutWindow->add_statictext_to_panel(aboutPanel,"-GLUI");
	aboutWindow->add_statictext_to_panel(aboutPanel,"");
	aboutWindow->add_separator_to_panel(aboutPanel);
	aboutWindow->add_statictext_to_panel(aboutPanel,"");
	aboutWindow->add_statictext_to_panel(aboutPanel,"Goal:");
	aboutWindow->add_statictext_to_panel(aboutPanel,"The aim of the project was to create a simulation");
	aboutWindow->add_statictext_to_panel(aboutPanel,"environment that can be used to educate students");
	aboutWindow->add_statictext_to_panel(aboutPanel,"in control-system development.");
	aboutWindow->add_statictext_to_panel(aboutPanel,"");
	aboutWindow->add_button("Close",1,(GLUI_Update_CB)(CloseAbout));

}

static void CloseHelp(){
	helpWindow->close();
}

static void Help(){
	//if(helpWindow!=NULL) helpWindow->close();
	helpWindow=GLUI_Master.create_glui("Help",0,100,100);
	GLUI_Panel* helpPanel=helpWindow->add_panel("Help",GLUI_PANEL_NAME_DROP);
	helpWindow->add_statictext_to_panel(helpPanel,"Usage:");
	helpWindow->add_statictext_to_panel(helpPanel,"You can control the actuators directly through your keyboard:");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"To create a new package -> a55,3.");
	helpWindow->add_statictext_to_panel(helpPanel,"To remove the package -> a55,-3.");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"To lift the package 10 units -> a1,-10.");
	helpWindow->add_statictext_to_panel(helpPanel,"To drop the package 5 units -> a1,5.");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"To tip the package 45 degrees -> a2,45.");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"To activate the first stopper -> a10,1.");
	helpWindow->add_statictext_to_panel(helpPanel,"To de-activate the first stopper -> a10,0.");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"You can also control the actuators and watch the state of the ");
	helpWindow->add_statictext_to_panel(helpPanel,"sensors through a terminal-program(e.g. minicom or hyperterminal)");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"To read from sensors and write to actuators you can use");
	helpWindow->add_statictext_to_panel(helpPanel,"the accompanying API for the Arduino Mega 2560:");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"To get the state of sensor 10: signed int value=readSensor(10)");
	helpWindow->add_statictext_to_panel(helpPanel,"");
	helpWindow->add_statictext_to_panel(helpPanel,"Se the manual for further details.");

	helpWindow->add_button("Close",1,(GLUI_Update_CB)(CloseHelp));

}

int main(int argc, char** argv)
{
	// Place a communicator at serialport (Default COM1)
	communicator = new Communicator(serialport);

	// Hide joints
	settings.drawJoints=0;

	// Set startup view position
	settings.viewCenter=b2Vec2(35,16);

	simulatorPageCount = 0;
	while (g_simulatorPageEntries[simulatorPageCount].createFcn != NULL)
	{
		++simulatorPageCount;
	}

	simulatorPageIndex = b2Clamp(simulatorPageIndex, 0, simulatorPageCount-1);
	simulatorPageSelection = simulatorPageIndex;

	entry = g_simulatorPageEntries + simulatorPageIndex;
	simulatorPage = entry->createFcn(communicator);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width, height);

	char title[32];
	sprintf(title, "Educational simulator for control-system development");

	mainWindow = glutCreateWindow(title);
	//glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(SimulationLoop);
	GLUI_Master.set_glutReshapeFunc(Resize);
	GLUI_Master.set_glutKeyboardFunc(Keyboard);
	GLUI_Master.set_glutSpecialFunc(KeyboardSpecial);
	GLUI_Master.set_glutMouseFunc(Mouse);
#ifdef FREEGLUT
	glutMouseWheelFunc(MouseWheel);
#endif
	glutMotionFunc(MouseMotion);

	glutKeyboardUpFunc(KeyboardUp);


	glui = GLUI_Master.create_glui_subwindow( mainWindow,
		GLUI_SUBWINDOW_RIGHT );


	glui->add_statictext("Simulator-page:");
	GLUI_Listbox* simulatorPageList =
		glui->add_listbox("", &simulatorPageSelection);

	int32 simulatorPageCount = 0;
	SimulatorPageEntry* e = g_simulatorPageEntries;
	while (e->createFcn)
	{
		simulatorPageList->add_item(simulatorPageCount, e->name);
		++simulatorPageCount;
		++e;
	}
	glui->add_separator();
	glui->add_edittext( "Serialport:", GLUI_EDITTEXT_TEXT, serialport );
	glui->add_button("Apply", 0, (GLUI_Update_CB)SetSerialPort);

	glui->add_separator();

	glui->add_button("Pause", 0, Pause);
	glui->add_button("Single Step", 0, SingleStep);
	glui->add_button("Restart", 0, Restart);

	glui->add_button("Quit", 0,(GLUI_Update_CB)Exit);

	glui->add_separator();

	glui->add_button("Help", 0,(GLUI_Update_CB)Help);
	glui->add_button("About", 0,(GLUI_Update_CB)About);


	glui->set_main_gfx_window( mainWindow );


	// Use a timer to control the frame rate.
	glutTimerFunc(framePeriod, Timer, 0);

	glutMainLoop();

	return 0;
}

