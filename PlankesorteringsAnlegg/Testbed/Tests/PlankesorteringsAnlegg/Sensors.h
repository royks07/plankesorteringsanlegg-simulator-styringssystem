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

class ContactSensor{
public:
	ContactSensor(){
		m_color=b2Color(1.0f,0.5f,0.5f);
		m_activated=false;
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
	}
	void drawLabel(){
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
	}
	void drawLabel(){
		//m_position=m_body->GetPosition();
		drawStrokeText(m_name ,m_position,1.0f, m_color);
	}
	float32 m_radius;
};



class SensorSet : public b2ContactListener{
public:
	SensorSet(){

	}
	void BeginContact(b2Contact* contact){
		vector<ContactSensor*>::iterator it;
		b2Fixture* fixtureA=contact->GetFixtureA();
		b2Fixture* fixtureB=contact->GetFixtureB();
		b2Fixture* tmpFixture;
		for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
			tmpFixture=(*it)->m_fixture;
			if(tmpFixture==fixtureA){
				UserData* userData=static_cast<UserData*>(fixtureB->GetUserData());
				if(userData!=NULL){
					t_userDataType type=userData->m_type;
					if(type==e_unknown){
						cout<<"unknown"<<endl;
						(*it)->m_activated=true;
					}else if(type==e_plank){
						cout<<"plank(Lengde:"<<((PlankUserData*)userData)->m_length<<" "<<"Quality:"<<((PlankUserData*)userData)->m_quality<<")"<<endl;
						(*it)->m_activated=true;
					}else if(type==e_medbringer){
						cout<<"medbringer"<<endl;
						(*it)->m_activated=true;
					}else if(type==e_storageLift){
						cout<<"storageLift"<<endl;
						(*it)->m_activated=true;
					}
				}
			}else if(tmpFixture==fixtureB){
				UserData* userData=static_cast<UserData*>(fixtureA->GetUserData());
				if(userData!=NULL){
					t_userDataType type=userData->m_type;
					if(type==e_unknown){
						cout<<"unknown"<<endl;
						(*it)->m_activated=true;
					}else if(type==e_plank){
						cout<<"plank(Lengde:"<<((PlankUserData*)userData)->m_length<<" "<<"Quality:"<<((PlankUserData*)userData)->m_quality<<")"<<endl;
						(*it)->m_activated=true;
					}else if(type==e_medbringer){
						cout<<"medbringer"<<endl;
						(*it)->m_activated=true;
					}else if(type==e_storageLift){
						cout<<"storageLift"<<endl;
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
		b2Fixture* tmpFixture;
		for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
			tmpFixture=(*it)->m_fixture;
			if(tmpFixture==fixtureA){
				UserData* userData=static_cast<UserData*>(fixtureB->GetUserData());
				if(userData!=NULL){
					t_userDataType type=userData->m_type;
					if(type==e_unknown){
						cout<<"unknown"<<endl;
						(*it)->m_activated=false;
					}else if(type==e_plank){
						cout<<"plank(Lengde:"<<((PlankUserData*)userData)->m_length<<" "<<"Quality:"<<((PlankUserData*)userData)->m_quality<<")"<<endl;
						(*it)->m_activated=false;
					}else if(type==e_medbringer){
						cout<<"medbringer"<<endl;
						(*it)->m_activated=false;
					}else if(type==e_storageLift){
						cout<<"storageLift"<<endl;
						(*it)->m_activated=false;
					}
				}
			}else if(tmpFixture==fixtureB){
				UserData* userData=static_cast<UserData*>(fixtureA->GetUserData());
				if(userData!=NULL){
					t_userDataType type=userData->m_type;
					if(type==e_unknown){
						cout<<"unknown"<<endl;
						(*it)->m_activated=false;
					}else if(type==e_plank){
						cout<<"plank(Lengde:"<<((PlankUserData*)userData)->m_length<<" "<<"Quality:"<<((PlankUserData*)userData)->m_quality<<")"<<endl;
						(*it)->m_activated=false;
					}else if(type==e_medbringer){
						cout<<"medbringer"<<endl;
						(*it)->m_activated=false;
					}else if(type==e_storageLift){
						cout<<"storageLift"<<endl;
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
};

#endif /* SENSORS_H_ */
