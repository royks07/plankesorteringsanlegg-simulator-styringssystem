/*
 * Actuators.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "Actuators.h"

Actuator::Actuator(int id){
	m_id=id;
	stringstream ss;
	ss.str("");
	ss<<"A"<<m_id;
	m_name=(char*)ss.str().c_str();
}


 ConveyorActuator::ConveyorActuator(int id,Conveyor* conveyor):Actuator(id){
	m_conveyor=conveyor;
	m_color=b2Color(0.5f,0.5f,1);
}

 void ConveyorActuator::drawLabel(){
	m_position=m_conveyor->m_p1;
	drawStrokeText(m_name ,m_position,1, m_color);
}


ConveyorActuatorBinary::ConveyorActuatorBinary(int id,Conveyor* conveyor):ConveyorActuator(id,conveyor){}

void ConveyorActuatorBinary::set(signed int value){
	if(value==0 && !m_conveyor->isPaused()) m_conveyor->pause();
	else if(value==1 && m_conveyor->isPaused()) m_conveyor->play();
}


ConveyorActuatorStep::ConveyorActuatorStep(int id,Conveyor* conveyor):ConveyorActuator(id,conveyor){}

void ConveyorActuatorStep::set(signed int value){
	float32 radius=m_conveyor->m_beamThickness/2+m_conveyor->m_beltThickness/2;
	float32 stepLength=0.1f;
	float32 stepAngle=stepLength/radius;
	m_maxRotation=abs(stepLength*value);
	int f=value/abs(value);
	m_conveyor->setSpeed(2.0f*f);//conveyor speed 2.0f
	m_startAngle=m_conveyor->getWheel1()->getBody()->GetAngle();
}
void ConveyorActuatorStep::run(){
	float32 currentAngle=m_conveyor->getWheel1()->getBody()->GetAngle();
	if(abs(currentAngle-m_startAngle)>=m_maxRotation) m_conveyor->setSpeed(0.0f);
}


JointActuatorPrismaticBinary::JointActuatorPrismaticBinary(int id,b2Joint* joint):Actuator(id){
	m_joint=joint;
	m_color=b2Color(0.5,0.5,1);
	m_speed=50.0f;
}

void JointActuatorPrismaticBinary::set(signed int value){
	if(value==1)((b2PrismaticJoint*)m_joint)->SetMotorSpeed(m_speed);
	else if(value==0)((b2PrismaticJoint*)m_joint)->SetMotorSpeed(-m_speed);
}
void JointActuatorPrismaticBinary::drawLabel(){
	m_position=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
	drawStrokeText(m_name ,m_position,1, m_color);
}


JointActuatorPrismaticRange::JointActuatorPrismaticRange(int id,b2Joint* joint):Actuator(id){
	m_joint=joint;
	m_color=b2Color(0.5,0.5,1);
}
void JointActuatorPrismaticRange::set(signed int value){
	((b2PrismaticJoint*)m_joint)->SetMotorSpeed(value);
}
void JointActuatorPrismaticRange::drawLabel(){
	m_position=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
	drawStrokeText(m_name ,m_position,1, m_color);
}


JointActuatorPrismaticStep::JointActuatorPrismaticStep(int id,b2Joint* joint):Actuator(id){
	m_joint=joint;
	m_color=b2Color(0.5,0.5,1);
}

void JointActuatorPrismaticStep::set(signed int value){
	m_value+=value;
	float32 stepLength=0.1f;
	m_maxDisplacement=abs(stepLength*value);
	int f=value/abs(value);
	((b2PrismaticJoint*)m_joint)->SetMotorSpeed(f*1);
	m_startPos=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
}

void JointActuatorPrismaticStep::drawLabel(){
	m_position=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
	drawStrokeText(m_name ,m_position,1, m_color);
}
void JointActuatorPrismaticStep::run(){
	m_currentPos=((b2PrismaticJoint*)m_joint)->GetBodyA()->GetPosition();
	b2Vec2 tmpVec=m_currentPos-m_startPos;
	float32 displacement=tmpVec.Length();
	if(displacement>=m_maxDisplacement){
		((b2PrismaticJoint*)m_joint)->SetMotorSpeed(0);
		m_value=0;
	}
}


JointActuatorRevoluteStep::JointActuatorRevoluteStep(int id,b2Joint* joint):Actuator(id){
	m_joint=joint;
	m_color=b2Color(0.5,0.5,1);
}

void JointActuatorRevoluteStep::set(signed int value){
	m_value+=value;
	float32 stepAngle=b2_pi/180.0f;
	m_maxRotation=abs(stepAngle*value);
	int f=value/abs(value);
	((b2RevoluteJoint*)m_joint)->SetMotorSpeed(f*0.3f);
	m_startAngle=((b2RevoluteJoint*)m_joint)->GetBodyA()->GetAngle();
}

void JointActuatorRevoluteStep::drawLabel(){
	m_position=((b2RevoluteJoint*)m_joint)->GetBodyA()->GetPosition();
	drawStrokeText(m_name ,m_position,1, m_color);
}
void JointActuatorRevoluteStep::run(){
	m_currentAngle=((b2RevoluteJoint*)m_joint)->GetBodyA()->GetAngle();
	float32 rotation=abs(m_currentAngle-m_startAngle);
	if(rotation>=m_maxRotation){
		((b2RevoluteJoint*)m_joint)->SetMotorSpeed(0);
		m_value=0;
	}
}


PackageSourceActuator::PackageSourceActuator(int id,PackageSource* packageSource):Actuator(id){
	m_packageSource=packageSource;
	m_position=m_packageSource->getPosition();
	m_color=b2Color(0.5f,1.0f,0.5f);
}

void PackageSourceActuator::drawLabel(){
	drawStrokeText(m_name ,m_position,0.5f, m_color);
}
void PackageSourceActuator::set(signed int value){
	//the same value shouldn't be used twice
	signed int positive=value/abs(value)+1;
	if(positive)m_packageSource->create(value);
	else m_packageSource->destruct(abs(value));
}
void PackageSourceActuator::run(){
	m_packageSource->runSprinkleRemover();
}


SprinkleSourceActuator::SprinkleSourceActuator(int id,SprinkleSource* sprinkleSource):Actuator(id){
	m_sprinkleSource=sprinkleSource;
	m_color=b2Color(0.5f,1.0f,0.5f);
	m_position=m_sprinkleSource->getPosition();
}

void SprinkleSourceActuator::drawLabel(){
	drawStrokeText(m_name ,m_position,0.5f, m_color);
}
void SprinkleSourceActuator::set(signed int value){
	if(value==1)m_sprinkleSource->create(1);
	if(value==-1)m_sprinkleSource->destruct(1);
}
void SprinkleSourceActuator::run(){
	m_sprinkleSource->updatePosition();
	m_position=m_sprinkleSource->getPosition();
}


void ActuatorSet::add(Actuator* actuator){
	m_actuator.push_back(actuator);
}
void ActuatorSet::drawLabels(){
	vector<Actuator*>::iterator it;
	for(it=m_actuator.begin();it!=m_actuator.end();it++){
		(*it)->drawLabel();
	}
}

void ActuatorSet::writeActuator(signed int id,signed int value){
	vector<Actuator*>::iterator it;
	for(it=m_actuator.begin();it!=m_actuator.end();it++){
		if((*it)->getId()==id){
			cout<<"writing to actuator "<<(*it)->getName()<<endl;
			(*it)->set(value);
		}
	}
}
void ActuatorSet::run(){
	vector<Actuator*>::iterator it;
	for(it=m_actuator.begin();it!=m_actuator.end();it++){
		(*it)->run();
	}
}


