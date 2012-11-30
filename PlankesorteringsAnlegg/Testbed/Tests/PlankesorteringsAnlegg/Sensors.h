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

class ContactSensor{
public:
	ContactSensor(string sensorName,b2Vec2 position, float32 radius,b2Filter filter,b2World* world){
		m_activated=false;
		m_sensorName=sensorName;
		m_position=position;
		m_radius=radius;
		m_world=world;

		b2BodyDef bd;
		bd.position=m_position;
		bd.type=b2_staticBody;

		b2FixtureDef fd;
		fd.isSensor=true;

		b2CircleShape shape;
		shape.m_radius=m_radius;
		fd.shape=&shape;
		fd.filter=filter;

		m_body=world->CreateBody(&bd);
		m_fixture=m_body->CreateFixture(&fd);
	}
	string m_sensorName;
	b2Vec2 m_position;
	float32 m_radius;
	b2World* m_world;
	b2Body* m_body;
	b2Fixture* m_fixture;
	bool m_activated;
};


class SensorField : public b2ContactListener{
public:
	SensorField(){

	}
	void BeginContact(b2Contact* contact){
		vector<ContactSensor*>::iterator it;
		b2Fixture* fixtureA=contact->GetFixtureA();
		b2Fixture* fixtureB=contact->GetFixtureB();
		b2Fixture* tmpFixture;
		for(it=m_contactSensorVector.begin();it!=m_contactSensorVector.end();it++){
			tmpFixture=(*it)->m_fixture;
			if(tmpFixture==fixtureA){
				cout<<"begiincontact.."<<endl;
				unsigned char* idChar=(unsigned char*)fixtureB->GetUserData();
				if(idChar!=NULL){
					if(*idChar=='M'){
						cout<<"idChar was 'M'"<<endl;
						(*it)->m_activated=true;
					}
					if(*idChar=='P')
						cout<<"idChar was 'P'"<<endl;
				}
			}else if(tmpFixture==fixtureB){
				cout<<"begincontact.."<<endl;
				unsigned char* idChar=(unsigned char*)fixtureA->GetUserData();
				if(idChar!=NULL){
					if(*idChar=='M'){
						cout<<"idChar was 'M'"<<endl;
						(*it)->m_activated=true;
					}
					if(*idChar=='P')
						cout<<"idChar was 'P'"<<endl;
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
				cout<<"endcontact.."<<endl;
				unsigned char* idChar=(unsigned char*)fixtureB->GetUserData();
				if(idChar!=NULL){
					if(*idChar=='M'){
						cout<<"idChar was 'M'"<<endl;
						(*it)->m_activated=false;
					}
					if(*idChar=='P')
						cout<<"idChar was 'P'"<<endl;
				}
			}else if(tmpFixture==fixtureB){
				cout<<"endcontact.."<<endl;
				unsigned char* idChar=(unsigned char*)fixtureA->GetUserData();
				if(idChar!=NULL){
					if(*idChar=='M'){
						cout<<"idChar was 'M'"<<endl;
						(*it)->m_activated=false;
					}
					if(*idChar=='P')
						cout<<"idChar was 'P'"<<endl;
				}
			}
		}
	}
	void add(ContactSensor* contactSensor){
		m_contactSensorVector.push_back(contactSensor);
	}
	vector<ContactSensor*> m_contactSensorVector;
};



#endif /* SENSORS_H_ */
