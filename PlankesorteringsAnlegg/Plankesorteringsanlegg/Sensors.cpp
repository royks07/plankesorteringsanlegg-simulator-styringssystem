/*
 * Sensors.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "Sensors.h"

ContactSensor::ContactSensor(){
	m_color=b2Color(1.0f,0.5f,0.5f);
	m_activated=false;
	m_sensorType=e_unknownSensor;
}

ContactSensor_FromFixture::ContactSensor_FromFixture(int id,b2Fixture* fixture) : ContactSensor(){
	m_id=id;
	m_fixture=fixture;
	m_body=m_fixture->GetBody();
	m_world=m_body->GetWorld();

	stringstream ss;
	ss.str("");
	ss<<"S"<<m_id;
	m_name=(char*)ss.str().c_str();

	m_fixture->SetSensor(true);
	m_position=m_body->GetPosition();
	m_sensorType=e_normal;
}

signed int ContactSensor_FromFixture::get(){
	if(m_activated) return 1;
	else return 0;
}

void ContactSensor_FromFixture::drawLabel(){
	m_position=m_body->GetPosition();
	drawStrokeText(m_name ,m_position,1, m_color);
}


ContactSensor_Sync::ContactSensor_Sync(b2Vec2 position,float32 radius, b2Filter filter,b2World* world) : ContactSensor(){
	stringstream ss;
	ss.str("");
	ss<<"sync";//<<m_id;
	m_name=(char*)ss.str().c_str();


	m_position=position;
	m_world=world;
	m_bd.position=m_position;
	m_bd.type=b2_staticBody;
	m_fd.isSensor=true;

	m_radius=radius;
	b2CircleShape shape;
	shape.m_radius=m_radius;
	m_fd.shape=&shape;
	m_fd.filter=filter;

	m_body=world->CreateBody(&m_bd);
	m_fixture=m_body->CreateFixture(&m_fd);
	m_sensorType=e_sync;
}

void ContactSensor_Sync::drawLabel(){
	//m_position=m_body->GetPosition();
	drawStrokeText(m_name ,m_position,0.5f, m_color);
}


ContactSensorBinary::ContactSensorBinary(int id,b2Vec2 position,float32 radius,b2World* world) : ContactSensor(){
	m_id=id;
	stringstream ss;
	ss.str("");
	ss<<"S"<<m_id;
	m_name=(char*)ss.str().c_str();

	m_position=position;
	m_world=world;
	m_bd.position=m_position;
	m_bd.type=b2_staticBody;
	m_fd.isSensor=true;

	m_radius=radius;
	b2CircleShape shape;
	shape.m_radius=m_radius;
	m_fd.shape=&shape;
	//m_fd.filter=filter;

	m_body=world->CreateBody(&m_bd);
	m_fixture=m_body->CreateFixture(&m_fd);
	m_sensorType=e_normal;
}

void ContactSensorBinary::drawLabel(){
	//m_position=m_body->GetPosition();
	drawStrokeText(m_name ,m_position,1.0f, m_color);
}

signed int ContactSensorBinary::get(){
	if(m_activated) return 1;
	else return 0;
}


LengthSensor::LengthSensor(int id,b2Vec2 position,float32 radius,b2World* world) : ContactSensorBinary(id,position,radius,world){
	m_sensorType=e_lengthSensor;
}

void LengthSensor::meassureLength(float32 length){
	if(length>=30 && length<33){
		m_meassuredLenght=30;
	}else if(length>=33 && length<36){
		m_meassuredLenght=33;
	}else if(length>=36 && length<40){
		m_meassuredLenght=36;
	}else{
		m_meassuredLenght=-1;
	}
}


QualitySensor::QualitySensor(int id,b2Vec2 position,float32 radius,b2World* world) : ContactSensorBinary(id,position,radius,world){
	m_sensorType=e_qualitySensor;
}

void QualitySensor::meassureQuality(float32 quality){
	if(quality>=0 && quality<0.3){
		m_meassuredQuality=1;
	}else if(quality>=0.3 && quality<0.6){
		m_meassuredQuality=2;
	}else if(quality>=0.6 && quality<1){
		m_meassuredQuality=3;
	}else{
		m_meassuredQuality=-1;
	}
}


CounterSensor::CounterSensor(int id,b2Vec2 position,float32 radius,b2Filter filter,b2World* world) : ContactSensor_Sync(position,radius,filter,world){
	m_sensorType=e_counterSensor;
	m_counter=0;
	m_id=id;
	stringstream ss;
	ss.str("");
	ss<<"S"<<m_id;
	m_name=(char*)ss.str().c_str();
}

void CounterSensor::updateCounter(){
	m_counter++;
}

void CounterSensor::drawLabel(){
	//m_position=m_body->GetPosition();
	drawStrokeText(m_name ,m_position,1.0f, m_color);
}


SensorSet::SensorSet(Communicator* communicator){
	m_communicator=communicator;
}

void SensorSet::BeginContact(b2Contact* contact){
	vector<ContactSensor*>::iterator it;
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	b2Fixture* sensorFixture;
	for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
		sensorFixture=(*it)->m_fixture;
		if(sensorFixture==fixtureA || sensorFixture==fixtureB){
			b2Fixture* colliderFixture;
			if(sensorFixture==fixtureA) colliderFixture=fixtureB;
			else colliderFixture=fixtureA;
			UserData* userData=static_cast<UserData*>(colliderFixture->GetUserData());
			if(userData!=NULL){
				t_userDataType colliderType=userData->m_type;
				t_sensorType sensorType=(*it)->getType();
				if(sensorType==e_normal){
					(*it)->m_activated=true;
					m_communicator->print("s%d,%d.",(*it)->getId(),(*it)->m_activated);
				}else if(colliderType==e_medbringer && sensorType==e_sync){
					(*it)->m_activated=true;
				}else if(colliderType==e_plank && sensorType==e_lengthSensor){
					(*it)->m_activated=true;
					((LengthSensor*)(*it))->meassureLength(((PlankUserData*)userData)->m_length);
					m_communicator->print("s%d,%d.",(*it)->getId(),(*it)->get());
				}else if(colliderType==e_plank && sensorType==e_qualitySensor){
					(*it)->m_activated=true;
					((QualitySensor*)(*it))->meassureQuality(((PlankUserData*)userData)->m_quality);
					m_communicator->print("s%d,%d.",(*it)->getId(),(*it)->get());
				}else if(colliderType==e_medbringer && sensorType==e_counterSensor){
					(*it)->m_activated=true;
					((CounterSensor*)(*it))->updateCounter();
					m_communicator->print("s%d,%d.",(*it)->getId(),(*it)->get());
				}else if(colliderType==e_storageLift){
					(*it)->m_activated=true;
				}
			}
		}
	}
}

void SensorSet::EndContact(b2Contact* contact){
	vector<ContactSensor*>::iterator it;
	b2Fixture* fixtureA=contact->GetFixtureA();
	b2Fixture* fixtureB=contact->GetFixtureB();
	b2Fixture* sensorFixture;
	for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
		sensorFixture=(*it)->m_fixture;
		if(sensorFixture==fixtureA || sensorFixture==fixtureB){
			b2Fixture* colliderFixture;
			if(sensorFixture==fixtureA) colliderFixture=fixtureB;
			else colliderFixture=fixtureA;
			UserData* userData=static_cast<UserData*>(colliderFixture->GetUserData());
			if(userData!=NULL){
				t_userDataType colliderType=userData->m_type;
				t_sensorType sensorType=(*it)->getType();
				if(colliderType==e_plank && sensorType==e_normal){
					(*it)->m_activated=false;
					m_communicator->print("s%d,%d.",(*it)->getId(),(*it)->m_activated);
				}else if(colliderType==e_medbringer && sensorType==e_sync){
					(*it)->m_activated=false;
				}else if(colliderType==e_plank && sensorType==e_lengthSensor){
					(*it)->m_activated=false;
				}else if(colliderType==e_plank && sensorType==e_qualitySensor){
					(*it)->m_activated=false;
				}else if(colliderType==e_medbringer && sensorType==e_counterSensor){
					(*it)->m_activated=false;
				}else if(colliderType==e_storageLift){
					(*it)->m_activated=false;
				}
			}
		}
	}
}

void SensorSet::drawLabels(){
	vector<ContactSensor*>::iterator it;
	for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
		(*it)->drawLabel();
	}
}

signed int SensorSet::readSensor(signed int id){
	vector<ContactSensor*>::iterator it;
	for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
		if((*it)->getId()==id){
			return (*it)->get();
		}
	}
	return -1;
}

void SensorSet::add(ContactSensor* contactSensor){
	m_contactSensorVector.push_back(contactSensor);
}

