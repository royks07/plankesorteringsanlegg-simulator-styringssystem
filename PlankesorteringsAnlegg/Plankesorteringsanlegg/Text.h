/** \file Text.h
 * \brief Contains a function to draw text on SimulatorPage.
 *
 * \author Roy Kollen Svendsen
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <Box2D/Box2D.h>
#include <string>

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "freeglut/freeglut.h"
#endif

using namespace std;

/** \brief Draw string on current SimulatorPage.
 *
 * @param str The string you want to display.
 * @param position The lower left corner of the string.
 * @param size A factor that changes the dimensions of the string.
 * @param color The text-color.
 */
void drawStrokeText(string str,b2Vec2 position,float32 size,b2Color color);

#endif /* TEXT_H_ */
