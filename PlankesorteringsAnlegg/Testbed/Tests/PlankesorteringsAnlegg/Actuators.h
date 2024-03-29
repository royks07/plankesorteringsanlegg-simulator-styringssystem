/*
 * Actuators.h
 *
 *  Created on: 28. nov. 2012
 *      Author: roy
 */

#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include <vector>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Text.h"

//midlertidig:
#include "Sensors.h"

#include "Package.h"

using namespace std;

class Actuator{
public:
	Actuator(int id){
		m_id=id;
		stringstream ss;
		ss.str("");
		ss<<"A"<<m_id;
		m_name=(char*)ss.str().c_str();
	}
	virtual ~Actuator(){
	}
	virtual int getId(){
		return m_id;
	}
	virtual string getName(){
		return m_name;
	}
	virtual void drawLabel(){

	}
	virtual void set(signed int value){

	}
	virtual void run(){

	}

	int m_id;

	b2Color m_color;
	b2Vec2 m_position;
	string m_name;
};

class ConveyorActuator : public Actuator{
public:
	ConveyorActuator(int id,Conveyor* conveyor):Actuator(id){
		m_conveyor=conveyor;
		m_color=b2Color(0.5f,0.5f,1);
	}
	virtual void drawLabel(){
		m_position=m_conveyor->m_p1;
		drawStrokeText(m_name ,m_position,1, m_color);

	}
	virtual void set(signed int value){

	}
	virtual void run(){

	}
	Conveyor* m_conveyor;
};

class ConveyorActuatorBinary : public ConveyorActuator{
public:
	ConveyorActuatorBinary(int id,Conveyor* conveyor):ConveyorActuator(id,conveyor){
	}
	void set(signed int value){
		if(value==0 && !m_conveyor->isPaused()) m_conveyor->pause();
		else if(value==1 && m_conveyor->isPaused()) m_conveyor->play();
	}
};

class ConveyorActuatorStep : public ConveyorActuator{
public:
	ConveyorActuatorStep(int id,Conveyor* conveyor):ConveyorActuator(id,conveyor){
	}
	void set(signed int value){
		float32 radius=m_conveyor->m_beamThickness/2+m_conveyor->m_beltThickness/2;
		float32 stepLength=0.1f;
		float32 stepAngle=stepLength/radius;
		m_maxRotation=abs(stepLength*value);
		int f=value/abs(value);
		m_conveyor->setSpeed(2.0f*f);//conveyor speed 2.0f
		m_startAngle=m_conveyor->getWheel1()->getBody()->GetAngle();
	}
	void run(){
		float32 currentAngle=m_conveyor->getWheel1()->getBody()->GetAngle();
		if(abs(currentAngle-m_startAngle)>=m_maxRotation) m_conveyor->setSpeed(0.0f);
	}
	float32 m_startAngle;
	float32 m_maxRotation;
};


class JointActuatorPrismaticBinary : public Actuator{
public:

	JointActuatorPrismaticBinary(int id,b2Joint* joint):Actuator(id){
		m_joint=joint;
		m_color=b2Color(0.5,0.5,1);
		m_speed=50.0f;
	}
	void set(signed int value){
		if(value==1)((b2PrismaticJoint*)m_joint)->SetMotorSpeed(m_speed);
		else if(value==0)((b2PrismaticJoint*)m_joint)->SetMotorSpeed(-m_speed);
	}
	void setSpeed(float32 speed){
		m_speed=speed;
	}
	void drawLabel(){
		m_position=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
		drawStrokeText(m_name ,m_position,1, m_color);
	}
	b2Joint* m_joint;
	float32 m_speed;
};

class JointActuatorPrismaticRange : public Actuator{
public:
	JointActuatorPrismaticRange(int id,b2Joint* joint):Actuator(id){
		m_joint=joint;
		m_color=b2Color(0.5,0.5,1);
	}
	void set(signed int value){
		((b2PrismaticJoint*)m_joint)->SetMotorSpeed(value);
	}
	void drawLabel(){
		m_position=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
		drawStrokeText(m_name ,m_position,1, m_color);
	}
	b2Joint* m_joint;
};

class JointActuatorPrismaticStep : public Actuator{
public:
	JointActuatorPrismaticStep(int id,b2Joint* joint):Actuator(id){
		m_joint=joint;
		m_color=b2Color(0.5,0.5,1);
	}
	void set(signed int value){
		m_value+=value;
		float32 stepLength=0.1f;
		m_maxDisplacement=abs(stepLength*value);
		int f=value/abs(value);
		((b2PrismaticJoint*)m_joint)->SetMotorSpeed(f*1);
		m_startPos=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();

	}
	void drawLabel(){
		m_position=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
		drawStrokeText(m_name ,m_position,1, m_color);
	}
	void run(){
		m_currentPos=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
		b2Vec2 tmpVec=m_currentPos-m_startPos;
		float32 displacement=tmpVec.Length();
		if(displacement>=m_maxDisplacement){
			((b2PrismaticJoint*)m_joint)->SetMotorSpeed(0);
			m_value=0;
		}
	}
	b2Joint* m_joint;
	b2Vec2 m_startPos;
	b2Vec2 m_currentPos;
	float32 m_maxDisplacement;
	signed int m_value;
};

class JointActuatorRevoluteStep : public Actuator{
public:
	JointActuatorRevoluteStep(int id,b2Joint* joint):Actuator(id){
		m_joint=joint;
		m_color=b2Color(0.5,0.5,1);
	}
	void set(signed int value){
		m_value+=value;
		float32 stepAngle=b2_pi/180.0f;
		m_maxRotation=abs(stepAngle*value);
		int f=value/abs(value);
		((b2RevoluteJoint*)m_joint)->SetMotorSpeed(f*0.3f);
		m_startAngle=((b2RevoluteJoint*)m_joint)->GetBodyA()->GetAngle();
	}
	void drawLabel(){
		m_position=((b2RevoluteJoint*)m_joint)->GetBodyA()->GetPosition();
		drawStrokeText(m_name ,m_position,1, m_color);
	}
	void run(){
		m_currentAngle=((b2RevoluteJoint*)m_joint)->GetBodyA()->GetAngle();
		float32 rotation=abs(m_currentAngle-m_startAngle);
		if(rotation>=m_maxRotation){
			((b2RevoluteJoint*)m_joint)->SetMotorSpeed(0);
			m_value=0;
		}
	}
	b2Joint* m_joint;
	float32 m_startAngle;
	float32 m_currentAngle;
	float32 m_maxRotation;
	signed int m_value;
};

class PackageSourceActuator : public Actuator{
public:
	PackageSourceActuator(int id,PackageSource* packageSource):Actuator(id){
		m_packageSource=packageSource;
		m_position=m_packageSource->getPosition();
		m_color=b2Color(0.5f,1.0f,0.5f);
	}

	virtual void drawLabel(){
		drawStrokeText(m_name ,m_position,0.5f, m_color);
	}
	void set(signed int value){
		//the same value shouldn't be used twice
		signed int positive=value/abs(value)+1;
		if(positive)m_packageSource->create(value);
		else m_packageSource->destruct(abs(value));
	}
	void run(){
		m_packageSource->runSprinkleRemover();
	}
	PackageSource* m_packageSource;
};

class SprinkleSourceActuator : public Actuator{
public:
	SprinkleSourceActuator(int id,SprinkleSource* sprinkleSource):Actuator(id){
		m_sprinkleSource=sprinkleSource;
		m_color=b2Color(0.5f,1.0f,0.5f);
		m_position=m_sprinkleSource->getPosition();
	}

	virtual void drawLabel(){
		drawStrokeText(m_name ,m_position,0.5f, m_color);
	}
	void set(signed int value){
		if(value==1)m_sprinkleSource->create(1);
		if(value==-1)m_sprinkleSource->destruct(1);
	}
	void run(){
		m_sprinkleSource->updatePosition();
		m_position=m_sprinkleSource->getPosition();
	}
	SprinkleSource* m_sprinkleSource;
};



class ActuatorSet{
public:
	ActuatorSet(){

	}
	void add(Actuator* actuator){
		m_actuator.push_back(actuator);
	}
	void drawLabels(){
		vector<Actuator*>::iterator it;
		for(it=m_actuator.begin();it!=m_actuator.end();it++){
			(*it)->drawLabel();
		}
	}

	void writeActuator(signed int id,signed int value){
		vector<Actuator*>::iterator it;
		for(it=m_actuator.begin();it!=m_actuator.end();it++){
			if((*it)->getId()==id){
				cout<<"writing to actuator "<<(*it)->getName()<<endl;
				(*it)->set(value);
			}
		}
	}
	void run(){
		vector<Actuator*>::iterator it;
		for(it=m_actuator.begin();it!=m_actuator.end();it++){
			(*it)->run();
		}
	}
	vector<Actuator*> m_actuator;
};

class CommandSequenceInterpreter{
public:
	CommandSequenceInterpreter(ActuatorSet* actuatorSet,SensorSet* sensorSet){
		m_actuatorSet=actuatorSet;
		m_sensorSet=sensorSet;
		m_readState=st_wait_for_new_command;
	}

	void interpret(unsigned char key){
			switch(m_readState){
			case st_wait_for_new_command:
				if(key=='a'){
					m_commandType=e_writeActuator;
					m_parameter.clear();
					m_ss.str("");
					cout<<"interpreter in actuator-state..."<<endl;
					m_readState=st_read_parameter;
				}else if (key=='s'){
					m_commandType=e_readSensor;
					m_parameter.clear();
					m_ss.str("");
					cout<<"interpreter in sensor-state..."<<endl;
					m_readState=st_read_parameter;
				}else{
				}
				break;
			case st_read_parameter:
				if(key==','){
					cout<<"finished reading parameter..."<<endl;
					m_readState=st_read_parameter;
					cout<<m_ss.str()<<endl;
					m_parameter.push_back(m_ss.str());
					m_ss.str("");
				//}else if(key=='.' && m_parameter.size()<=1){
					//do not end - need two parameters
				}else if(key=='.'){
					cout<<"finished reading command."<<endl;
					m_readState=st_wait_for_new_command;
					cout<<m_ss.str()<<endl;
					m_parameter.push_back(m_ss.str());
					m_ss.str("");
					//execute command:
					if(m_commandType==e_writeActuator){
						int id;
						int value;
						try {
						     id = boost::lexical_cast<int>(m_parameter.at(0).c_str());
						     value = boost::lexical_cast<int>(m_parameter.at(1).c_str());

						} catch( boost::bad_lexical_cast const& ) {
						    std::cout << "Error: parameter was not valid" << std::endl;
						}
						m_actuatorSet->writeActuator(id,value);
						cout<<"Writing "<<value<<" to a"<<id<<endl;
					}else if(m_commandType==e_readSensor){
						int id;
						try {
						     id = boost::lexical_cast<int>(m_parameter.at(0).c_str());
						} catch( boost::bad_lexical_cast const& ) {
						    std::cout << "Error: parameter was not valid" << std::endl;
						}
						signed int value=m_sensorSet->readSensor(id);
						cout<<"Value of S"<<id<<" is "<<value<<endl;

					}
				}else{
					cout<<"reading parameter..."<<endl;
					m_ss<<key;
				}
				break;
			}
	}
	typedef enum {st_wait_for_new_command,st_read_parameter} t_readState;
	typedef enum {e_writeActuator,e_readSensor} t_commandType;
	t_commandType m_commandType;
	t_readState m_readState;
	stringstream m_ss;
	vector<string> m_parameter;
	ActuatorSet* m_actuatorSet;
	SensorSet* m_sensorSet;
};

#endif /* ACTUATORS_H_ */
