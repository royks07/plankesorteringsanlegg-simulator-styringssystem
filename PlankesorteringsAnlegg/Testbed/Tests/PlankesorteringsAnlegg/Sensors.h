/*
 * Sensors.h
 *
 *  Created on: 26. nov. 2012
 *      Author: roy
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <string>
#include <vector>

#include "UserData.h"

#include "Text.h"

#include "Kommunikator.h"

typedef enum{e_unknownSensor,e_normal,e_sync,e_lengthSensor,e_qualitySensor,e_counterSensor} t_sensorType;

class ContactSensor{
public:
	ContactSensor(){
		m_color=b2Color(1.0f,0.5f,0.5f);
		m_activated=false;
		m_sensorType=e_unknownSensor;
	}
	virtual ~ContactSensor(){
		}
	virtual int getId(){
		return m_id;
	}
	virtual string getName(){
		return m_name;
	}
	virtual void drawLabel(){

	}
	virtual signed int get(){

	}
	virtual t_sensorType getType(){
		return m_sensorType;
	}
	int32 m_id;
	b2Color m_color;
	string m_name;
	b2Vec2 m_position;
	float32 m_radius;
	b2World* m_world;
	b2Body* m_body;
	b2Fixture* m_fixture;
	bool m_activated;
	b2BodyDef m_bd;
	b2FixtureDef m_fd;
	t_sensorType m_sensorType;
};

class ContactSensor_FromFixture : public ContactSensor{
public:
	ContactSensor_FromFixture(int id,b2Fixture* fixture) : ContactSensor(){
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
	signed int get(){
		if(m_activated) return 1;
		else return 0;
	}
	void drawLabel(){
		m_position=m_body->GetPosition();
		drawStrokeText(m_name ,m_position,1, m_color);
	}
	float32 m_radius;
};


class ContactSensor_Sync : public ContactSensor{
public:
	ContactSensor_Sync(b2Vec2 position,float32 radius, b2Filter filter,b2World* world) : ContactSensor(){

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
	virtual void drawLabel(){
		//m_position=m_body->GetPosition();
		drawStrokeText(m_name ,m_position,0.5f, m_color);
	}
	float32 m_radius;
};

class ContactSensorBinary : public ContactSensor{
public:
	ContactSensorBinary(int id,b2Vec2 position,float32 radius,b2World* world) : ContactSensor(){
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
	virtual void drawLabel(){
		//m_position=m_body->GetPosition();
		drawStrokeText(m_name ,m_position,1.0f, m_color);
	}
	virtual signed int get(){
		if(m_activated) return 1;
		else return 0;
	}
	float32 m_radius;
};

class LengthSensor : public ContactSensorBinary{
public:
	LengthSensor(int id,b2Vec2 position,float32 radius,b2World* world) : ContactSensorBinary(id,position,radius,world){
		m_sensorType=e_lengthSensor;
	}

	void meassureLength(float32 length){
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

	signed int get(){
		return m_meassuredLenght;
	}
	signed int m_meassuredLenght;
};

class QualitySensor : public ContactSensorBinary{
public:
	QualitySensor(int id,b2Vec2 position,float32 radius,b2World* world) : ContactSensorBinary(id,position,radius,world){
		m_sensorType=e_qualitySensor;
	}

	void meassureQuality(float32 quality){
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

	signed int get(){
		return m_meassuredQuality;
	}
	signed int m_meassuredQuality;
};

class CounterSensor : public ContactSensor_Sync{
public:
	CounterSensor(int id,b2Vec2 position,float32 radius,b2Filter filter,b2World* world) : ContactSensor_Sync(position,radius,filter,world){
		m_sensorType=e_counterSensor;
		m_counter=0;
		m_id=id;
		stringstream ss;
		ss.str("");
		ss<<"S"<<m_id;
		m_name=(char*)ss.str().c_str();
	}
	void updateCounter(){
		m_counter++;
	}
	void drawLabel(){
		//m_position=m_body->GetPosition();
		drawStrokeText(m_name ,m_position,1.0f, m_color);
	}
	signed int get(){
		return m_counter;
	}
	signed int m_counter;
};



class SensorSet : public b2ContactListener{
public:
	SensorSet(Communicator* communicator){
		m_communicator=communicator;
	}
	void BeginContact(b2Contact* contact){
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
	void EndContact(b2Contact* contact){
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

	void drawLabels(){
		vector<ContactSensor*>::iterator it;
		for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
			(*it)->drawLabel();
		}
	}
	signed int readSensor(signed int id){
		vector<ContactSensor*>::iterator it;
		for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
			if((*it)->getId()==id){
				return (*it)->get();
			}
		}
		return -1;
	}
	void add(ContactSensor* contactSensor){
		m_contactSensorVector.push_back(contactSensor);
	}
	vector<ContactSensor*> m_contactSensorVector;
	Communicator* m_communicator;
};

#endif /* SENSORS_H_ */
