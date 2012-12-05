/*
 * Package.h
 *
 *  Created on: 4. des. 2012
 *      Author: roy
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include "UserData.h"

class Sprinkle{
public:
	Sprinkle(b2World* world){
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
		m_width=7.62f;
		m_thickness=0.1f;
		shape->SetAsBox(m_width/2,m_thickness/2);
		m_fd->shape=shape;
	}
	Sprinkle* CreateAt(b2Vec2* pos){
		m_bd->position=*pos;
		m_body = m_world->CreateBody(m_bd);
		m_bHasBody=true;
		m_fixture = m_body->CreateFixture(m_fd);
		return this;
	}

	float32 getWidth(){
		return m_width;
	}

	float32 getThickness(){
		return m_thickness;
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
	float32 m_width;
	float32 m_thickness;
	b2Body* m_body;
	bool m_bHasBody;
	b2Fixture* m_fixture;
};

class Plank{
public:
	Plank(b2World* world){
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
		m_width=1.73f;
		m_thickness=0.57f;
		shape->SetAsBox(m_width/2,m_thickness/2);
		m_fd->shape=shape;

		float32 length=randomFloat(30.0f,40.0f);
		float32 quality=randomFloat(0.0f,1.0f);
		cout<<"Length:"<<length<<" Quality:"<<quality<<endl;

		PlankUserData* plankUserData = new PlankUserData(length,quality);
		m_fd->userData=plankUserData;

	}
	float randomFloat(float a, float b) {
	    float random = ((float) rand()) / (float) RAND_MAX;
	    float diff = b - a;
	    float r = random * diff;
	    return a + r;
	}
	void CreateAt(b2Vec2* pos){
		m_bd->position=*pos;
		m_body = m_world->CreateBody(m_bd);
		m_body->CreateFixture(m_fd);
	}

	float32 getWidth(){
		return m_width;
	}

	float32 getThickness(){
		return m_thickness;
	}
private:
	b2BodyDef* m_bd;
	b2FixtureDef* m_fd;
	b2World* m_world;
	float32 m_width;
	float32 m_thickness;
	b2Body* m_body;
};


class Package{
public:
	Package(b2Vec2 position, b2World* world){

		for(int i=0;i<7;i++){
			m_sprinkle[i] = new Sprinkle(world);
		}
		float32 sprinkleWidth=m_sprinkle[0]->getWidth();
		m_sprinkleThickness=m_sprinkle[0]->getThickness();

		for(int r=0;r<8;r++){
			if(r>0&&r<8){
				b2Vec2 posSprinkle;
				posSprinkle.SetZero();
				posSprinkle+=position;
				posSprinkle+=b2Vec2(sprinkleWidth/2-0.5,r*m_plankThickness+r*m_sprinkleThickness-m_sprinkleThickness/2);
				m_sprinkle[r-1]->CreateAt(&posSprinkle);
			}

			for(int c=0;c<4;c++){
				Plank* plank = new Plank(world);
				float32 plankWidth=plank->getWidth();
				if(c==0 && r==0)m_plankThickness=plank->getThickness();
				b2Vec2 posPlank;
				posPlank.SetZero();
				posPlank+=position;
				posPlank+=b2Vec2(c*plankWidth+plankWidth/2,r*m_plankThickness+m_plankThickness/2+r*m_sprinkleThickness);
				plank->CreateAt(&posPlank);

			}
		}

	}
	float32 getPlankThickness(){
		return m_plankThickness;
	}
	float32 getSprinkleThickness(){
		return m_sprinkleThickness;
	}
	void runSprinkleRemover(){
		b2Vec2 center=b2Vec2(11.0f,25.0f);
		float32 distance=9.0f;
		int sprinkleCount=7;
		for(int i=0;i<sprinkleCount;i++){
			m_sprinkle[i]->removeWhenOutOfBounds(center,distance);
//BUG: Krasjer når strø flyttes outofbounds + 100 enheter med musa...
		}
	}
	float32 m_plankThickness;
	float32 m_sprinkleThickness;
	Sprinkle* m_sprinkle[7];
};







#endif /* PACKAGE_H_ */
