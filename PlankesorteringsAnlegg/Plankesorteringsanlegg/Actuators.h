/**
 *	\file Actuators.h
 *	\brief Contains actuator-hierarchy and actuator-set.
 *
 *	\author Roy Kollen Svendsen
 */

#ifndef ACTUATORS_H_
#define ACTUATORS_H_


#include <vector>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Text.h"
#include "Conveyor.h"

#include "Sensors.h"
#include "Package.h"

using namespace std;

/** \brief Abstract class from which other actuator-classes inherit.
 */
class Actuator{
public:
	Actuator(int id);
	virtual ~Actuator(){}
	virtual int getId(){return m_id;}
	virtual string getName(){return m_name;}
	virtual void drawLabel(){	}
	virtual void set(signed int value){}
	virtual void run(){}

	int m_id;
	b2Color m_color;
	b2Vec2 m_position;
	string m_name;
};

/** \brief Abstact class from which conveyor-actuators inherit.
 */
class ConveyorActuator : public Actuator{
public:
	ConveyorActuator(int id,Conveyor* conveyor);
	virtual void drawLabel();
	virtual void set(signed int value){};
	virtual void run(){};
	Conveyor* m_conveyor;
};

/** \brief Turn a conveyor ON or OFF.
 */
class ConveyorActuatorBinary : public ConveyorActuator{
public:
	ConveyorActuatorBinary(int id,Conveyor* conveyor);
	void set(signed int value);
};

/** \brief Run a conveyor an arbitrary number of steps.
 */
class ConveyorActuatorStep : public ConveyorActuator{
public:
	ConveyorActuatorStep(int id,Conveyor* conveyor);
	void set(signed int value);
	void run();

	float32 m_startAngle;
	float32 m_maxRotation;
};

/** \brief Switch a prismatic joint between the two extremal states.
 */
class JointActuatorPrismaticBinary : public Actuator{
public:
	JointActuatorPrismaticBinary(int id,b2Joint* joint);
	void set(signed int value);
	inline void setSpeed(float32 speed){m_speed=speed;}
	void drawLabel();

	b2Joint* m_joint;
	float32 m_speed;
};

/** \brief Run a prismatic joint at an arbitrary constant speed.
 */
class JointActuatorPrismaticRange : public Actuator{
public:
	JointActuatorPrismaticRange(int id,b2Joint* joint);
	void set(signed int value);
	void drawLabel();

	b2Joint* m_joint;
};

/** \brief Run a prismatic joint an arbitrary number of steps.
 */
class JointActuatorPrismaticStep : public Actuator{
public:
	JointActuatorPrismaticStep(int id,b2Joint* joint);
	void set(signed int value);
	void drawLabel();
	void run();

	b2Joint* m_joint;
	b2Vec2 m_startPos;
	b2Vec2 m_currentPos;
	float32 m_maxDisplacement;
	signed int m_value;
};

/** \brief Turn a revolute joint an arbitrary angle.
 */
class JointActuatorRevoluteStep : public Actuator{
public:
	JointActuatorRevoluteStep(int id,b2Joint* joint);
	void set(signed int value);
	void drawLabel();
	void run();

	b2Joint* m_joint;
	float32 m_startAngle;
	float32 m_currentAngle;
	float32 m_maxRotation;
	signed int m_value;
};

/** \brief Produce packages at a specific position, by controlling a package-source.
 */
class PackageSourceActuator : public Actuator{
public:
	PackageSourceActuator(int id,PackageSource* packageSource);
	virtual void drawLabel();
	void set(signed int value);
	void run();

	PackageSource* m_packageSource;
};

/** \brief Produce "sprinkles" at a specific position, by controlling a sprinkle-source.
 */
class SprinkleSourceActuator : public Actuator{
public:
	SprinkleSourceActuator(int id,SprinkleSource* sprinkleSource);
	virtual void drawLabel();
	void set(signed int value);
	void run();

	SprinkleSource* m_sprinkleSource;
};

/** \brief Draw actuator-labels, run actuators and change the state of actuators by id.
 */
class ActuatorSet{
public:
	ActuatorSet(){}
	void add(Actuator* actuator);/**< \brief Add any actuator to the actuator-set.*/
	void drawLabels();/**< \brief Draw labels of all registered actuators.*/
	void writeActuator(signed int id,signed int value);/**< \brief Change state of actuator by actuator-id.*/
	void run();/**< \brief Run all registered actuators.*/

	vector<Actuator*> m_actuator;
};

#endif /* ACTUATORS_H_ */
