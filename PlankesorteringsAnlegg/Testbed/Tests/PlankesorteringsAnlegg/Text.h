/*
 * Text.h
 *
 *  Created on: 28. nov. 2012
 *      Author: roy
 */

#ifndef TEXT_H_
#define TEXT_H_

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "freeglut/freeglut.h"
#endif

void drawStrokeText(string str,b2Vec2 position,float32 size,b2Color color)
	{
		glPushMatrix();
		glTranslatef(position.x,position.y,0);
		glScalef(0.01f*size,0.01f*size,0);
		glLineWidth(3);
		glColor3f(color.r,color.g,color.b);

		string::iterator it;
		for(it=str.begin();it!=str.end();it++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN , (*it));
		}

		glPopMatrix();
		glLineWidth(1);
	}

#endif /* TEXT_H_ */
