/*
 * AktuatorFlate.h
 *
 *  Created on: 14. nov. 2012
 *      Author: roy
 */

#ifndef AKTUATORFLATE_H_
#define AKTUATORFLATE_H_

#include <iostream>
#include <list>

#include "Pakke.h"

class Aktuator{
public:
	Aktuator(b2Joint* joint,char activationKey,float32 speed){

		m_activationKey=activationKey;
		m_speed=speed;
		m_joint=joint;
		m_type=m_joint->GetType();
		m_bCreator=false;
		cout<<"Aktuator-type: "<<m_type<<endl;
		cout<<"Aktuator::Aktuator(b2Joint*): created..";

	}

	Aktuator(b2Body* body,char activationKey){

		m_body=body;
		m_activationKey=activationKey;
		m_bCreator=true;
		cout<<"Aktuator-type: "<<"Creator"<<endl;
		cout<<"Aktuator::Aktuator(activationKey,posisjon): created..";
	}

private:
	void swtichAktuator(char key){
		cout <<"test"<<endl;
		if(key==m_activationKey){
			if(m_bCreator){
				m_posisjon=m_body->GetPosition();
				Stro stro = Stro(m_body->GetWorld());
				stro.CreateAt(&m_posisjon);
			}else{
				if(m_type==e_revoluteJoint){
					cout<<"e_revoluteJoint"<<endl;
					((b2RevoluteJoint*)m_joint)->SetMotorSpeed(m_speed);
				}else if(m_type==e_prismaticJoint){
					cout<<"e_prismaticJoint"<<endl;
					((b2PrismaticJoint*)m_joint)->SetMotorSpeed(m_speed);
				}
			}
		}
	}
	friend class AktuatorFlate;
	b2JointType m_type;
	b2Joint* m_joint;
	char m_activationKey;
	float32 m_speed;

	b2Vec2 m_posisjon;
	b2Body* m_body;
	bool m_bCreator;


};

class AktuatorFlate{
public:
	AktuatorFlate(){
		m_aktuator_liste = list<Aktuator>();
	}
	void addAktuator(Aktuator aktuator){
		m_aktuator_liste.push_back(aktuator);
	}
	void switchActuators(char key){
		list<Aktuator>::iterator it;
		for(it=m_aktuator_liste.begin();it!=m_aktuator_liste.end();it++){
			(*it).swtichAktuator(key);
		}
	}
	list <Aktuator> m_aktuator_liste;

};


#endif /* AKTUATORFLATE_H_ */
