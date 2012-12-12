/*
 * Text.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */


#include "Text.h"

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
