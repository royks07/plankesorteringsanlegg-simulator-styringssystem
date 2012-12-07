/*
* Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef JSONDEMO_CARS_H
#define JSONDEMO_CARS_H

#include <vector>
#include "../Framework/b2dJson.h"

//A collection of different vehicles to play around with.
//Yes, I know the walker don't walk so good and the tank
//tracks fall off almost immediately... but it's still fun
//even without tracks :p
//
//This demo shows how to load a Box2D world from a JSON file
//and access named joints in the file to manipulate later.
//See the class constructor for details.

class jsonDemoCars : public SimulatorPage
{
public:
	enum vehicleType {
		e_bike,
		e_car,
		e_truck,
		e_tank,
		e_walker,
		e_vehicleMax,
	};

	enum {
		k_left = 	0x01,
		k_stop = 	0x02,
		k_right = 	0x04,
	};

	jsonDemoCars()
	{		
		m_pCurrentJoints = NULL;

		//load world from JSON file
		b2dJson json;
		b2World* world = json.readFromFile("cars.json");
		if ( world ) {
			//replace testbed's world
			delete m_world;
			m_world = world;

			//re-set standard testbed stuff
			m_world->SetContactListener(this);
			m_world->SetDebugDraw(&m_debugDraw);
			b2BodyDef bodyDef;
			m_groundBody = m_world->CreateBody(&bodyDef);//needed for testbed mousejoint

			//get named drive joints from the JSON info
			json.getJointsByName("bike", m_bikeJoints);
			json.getJointsByName("car", m_carJoints);
			json.getJointsByName("truck", m_truckJoints);
			json.getJointsByName("tank", m_tankJoints);
			json.getJointsByName("walker", m_walkerJoints);
		}

		m_vehicle = e_bike;
		m_keyState = 0;
		m_pCurrentJoints = &m_bikeJoints;
	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'a': m_keyState += k_left; break;
		case 's': m_keyState += k_stop; break;
		case 'd': m_keyState += k_right; break;
		case 'c':
			m_vehicle = (vehicleType)(m_vehicle+1);
			if ( m_vehicle == e_vehicleMax )
				m_vehicle = e_bike;
			switch ( m_vehicle )
			{
			case e_bike: m_pCurrentJoints = &m_bikeJoints; break;
			case e_car: m_pCurrentJoints = &m_carJoints; break;
			case e_truck: m_pCurrentJoints = &m_truckJoints; break;
			case e_tank: m_pCurrentJoints = &m_tankJoints; break;
			case e_walker: m_pCurrentJoints = &m_walkerJoints; break;
			}

			break;
		}
	}

	void KeyboardUp(unsigned char key)
	{
		switch (key)
		{
		case 'a': m_keyState -= k_left; break;
		case 's': m_keyState -= k_stop; break;
		case 'd': m_keyState -= k_right; break;
		}
	}

	void Step(Settings* settings)
	{
		SimulatorPage::Step(settings);

		m_debugDraw.DrawString(5, m_textLine, "Keys: left = a, brake = s, right = d, change vehicle = c");
		m_textLine += 15;
		m_debugDraw.DrawString(5, m_textLine, "Currently controlling: %s", m_vehicle==e_bike?"bike": m_vehicle==e_car?"car": m_vehicle==e_truck?"monster truck": m_vehicle==e_tank?"tank":"walker");
		m_textLine += 15;

		//set motor speeds for current vehicle
		float32 motorSpeed = m_keyState == k_left ? 1 : m_keyState == k_right ? -1 : 0;
		switch ( m_vehicle )
		{
		case e_bike : motorSpeed *= 30; break;
		case e_car : motorSpeed *= 30; break;
		case e_truck : motorSpeed *= 15; break;
		case e_tank : motorSpeed *= 10; break;
		case e_walker : motorSpeed *= -3; break;
		}
		for (int i = 0; i < m_pCurrentJoints->size(); i++) {
			switch ( m_vehicle )
			{
			case e_bike :
			case e_walker :
				((b2RevoluteJoint*)m_pCurrentJoints->at(i))->SetMotorSpeed(motorSpeed);
				break;
			default:
				((b2WheelJoint*)m_pCurrentJoints->at(i))->SetMotorSpeed(motorSpeed);
			}
		}

		//move viewpoint to current vehicle
		if ( !m_pCurrentJoints->empty() ) {
			float32 desiredViewCenterX = m_pCurrentJoints->at(0)->GetBodyA()->GetWorldCenter().x;
			settings->viewCenter.x = 0.25f * settings->viewCenter.x + 0.75f * desiredViewCenterX;//avoid 'snapping'
		}
	}

	static SimulatorPage* Create()
	{
		return new jsonDemoCars;
	}
	
	//control state
	vehicleType m_vehicle;
	int m_keyState;

	//drive joints for available vehicles
	vector<b2Joint*> m_bikeJoints;
	vector<b2Joint*> m_carJoints;
	vector<b2Joint*> m_truckJoints;
	vector<b2Joint*> m_tankJoints;
	vector<b2Joint*> m_walkerJoints;

	//reference to drive joints of currently controlled vehicle
	vector<b2Joint*>* m_pCurrentJoints;
};

#endif
