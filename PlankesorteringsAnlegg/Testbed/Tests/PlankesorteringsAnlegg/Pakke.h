/*
 * Pakke.h
 *
 *  Created on: 13. nov. 2012
 *      Author: roy
 */

#ifndef PAKKE_H_
#define PAKKE_H_

#include "UserData.h"

class Stro{
public:
	Stro(b2World* world){
		m_world = world;
		m_bd = new b2BodyDef;
		m_bd->position=b2Vec2(0,0);
		m_bd->gravityScale=1.0f;
		m_bd->type=b2_dynamicBody;
		m_fd = new b2FixtureDef;
		m_fd->density=1.0f;
		b2Filter filter;
		filter.categoryBits=15;
		filter.maskBits=15;
		filter.groupIndex=0;
		m_fd->filter=filter;
		m_fd->friction=0.1f;
		m_fd->restitution=0.0f;
		b2PolygonShape* shape = new b2PolygonShape;
		m_bredde=7.62f;
		m_tykkelse=0.1f;
		shape->SetAsBox(m_bredde/2,m_tykkelse/2);
		m_fd->shape=shape;
	}
	Stro* CreateAt(b2Vec2* pos){
		m_bd->position=*pos;
		m_body = m_world->CreateBody(m_bd);
		m_bHasBody=true;
		m_fixture = m_body->CreateFixture(m_fd);
		return this;
	}

	float32 getBredde(){
		return m_bredde;
	}

	float32 getTykkelse(){
		return m_tykkelse;
	}

	void removeWhenOutOfBounds(b2Vec2 senter,float32 avstand){
		if(m_bHasBody){
			if((m_body->GetPosition()-senter).Length()>avstand){
				cout<<"outofbounds..."<<endl;
				b2Filter filter;
				filter.categoryBits=0;
				filter.maskBits=0;
				filter.groupIndex=0;
				m_fixture->SetFilterData(filter);

			}
			if((m_body->GetPosition()-senter).Length()>(avstand+100)){
				m_world->DestroyBody(m_body);
//BUG: krasjer når flyttes med musa...mus-rutinene mister objektet som pekes på..
				m_bHasBody=false;;
			}
		}

	}


private:
	b2BodyDef* m_bd;
	b2FixtureDef* m_fd;
	b2World* m_world;
	float32 m_bredde;
	float32 m_tykkelse;
	b2Body* m_body;
	bool m_bHasBody;
	b2Fixture* m_fixture;
};

class Planke{
public:
	Planke(b2World* world){
		m_world = world;
		m_bd = new b2BodyDef;
		m_bd->position=b2Vec2(0,0);
		m_bd->gravityScale=1.0f;
		m_bd->type=b2_dynamicBody;
		m_fd = new b2FixtureDef;
		m_fd->density=1.0f;
		b2Filter filter;
		filter.categoryBits=15;
		filter.maskBits=15;
		filter.groupIndex=0;
		m_fd->filter=filter;
		m_fd->friction=0.5f;
		m_fd->restitution=0.0f;
		b2PolygonShape* shape = new b2PolygonShape;
		m_bredde=1.73f;
		m_tykkelse=0.57f;
		shape->SetAsBox(m_bredde/2,m_tykkelse/2);
		m_fd->shape=shape;

		PlankUserData* plankUserData = new PlankUserData(3.4f,0.7f);
		m_fd->userData=plankUserData;

	}
	void CreateAt(b2Vec2* pos){
		m_bd->position=*pos;
		m_body = m_world->CreateBody(m_bd);
		m_body->CreateFixture(m_fd);
	}

	float32 getBredde(){
		return m_bredde;
	}

	float32 getTykkelse(){
		return m_tykkelse;
	}
private:
	b2BodyDef* m_bd;
	b2FixtureDef* m_fd;
	b2World* m_world;
	float32 m_bredde;
	float32 m_tykkelse;
	b2Body* m_body;
};


class Pakke{
public:
	Pakke(b2Vec2 posisjon, b2World* world){

		Planke planke(world);
		float32 plankeBredde=planke.getBredde();
		float32 plankeTykkelse=planke.getTykkelse();
		for(int i=0;i<7;i++){
			m_stro[i] = new Stro(world);
		}
		float32 stroBredde=m_stro[0]->getBredde();
		float32 stroTykkelse=m_stro[0]->getTykkelse();

		for(int r=0;r<8;r++){
			if(r>0&&r<8){
				b2Vec2 posStro;
				posStro.SetZero();
				posStro+=posisjon;
				posStro+=b2Vec2(stroBredde/2-0.5,r*plankeTykkelse+r*stroTykkelse-stroTykkelse/2);
				m_stro[r-1]->CreateAt(&posStro);
			}

			for(int c=0;c<4;c++){
				b2Vec2 posPlanke;
				posPlanke.SetZero();
				posPlanke+=posisjon;
				posPlanke+=b2Vec2(c*plankeBredde+plankeBredde/2,r*plankeTykkelse+plankeTykkelse/2+r*stroTykkelse);
				planke.CreateAt(&posPlanke);

			}
		}

	}
	void runStroFjerner(){
		b2Vec2 senter=b2Vec2(11.0f,25.0f);
		float32 avstand=9.0f;
		int antallStro=7;
		for(int i=0;i<antallStro;i++){
			m_stro[i]->removeWhenOutOfBounds(senter,avstand);
//BUG: Krasjer når strø flyttes outofbounds + 100 enheter med musa...
		}
	}
	Stro* m_stro[7];
};




#endif /* PAKKE_H_ */
