/*
 * Package.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "Package.h"

Sprinkle::Sprinkle(b2Vec2 pos,b2World* world){
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

	m_bd->position=pos;
	m_body = m_world->CreateBody(m_bd);
	m_bHasBody=true;
	m_fixture = m_body->CreateFixture(m_fd);
}

Sprinkle::~Sprinkle(){
	m_world->DestroyBody(m_body);
}

void Sprinkle::removeWhenOutOfBounds(b2Vec2 senter,float32 avstand){
	if(m_bHasBody){
		if((m_body->GetPosition()-senter).Length()>avstand){
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


Plank::Plank(b2Vec2 pos,b2World* world){
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

	m_bd->position=pos;
	m_body = m_world->CreateBody(m_bd);
	m_body->CreateFixture(m_fd);
}

Plank::~Plank(){
	m_world->DestroyBody(m_body);
}

float Plank::randomFloat(float a, float b) {
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


Package::Package(b2Vec2 position, b2World* world){

	float32 sprinkleWidth=7.62f;
	m_sprinkleThickness=0.1f;

	float32 plankWidth=1.73f;
	m_plankThickness=0.57f;

	for(int r=0;r<8;r++){
		if(r>0&&r<8){
			b2Vec2 posSprinkle;
			posSprinkle.SetZero();
			posSprinkle+=position;
			posSprinkle+=b2Vec2(sprinkleWidth/2-0.5,r*m_plankThickness+r*m_sprinkleThickness-m_sprinkleThickness/2);
			m_sprinkle[r-1] = new Sprinkle(posSprinkle,world);
		}

		for(int c=0;c<4;c++){
			b2Vec2 posPlank;
			posPlank.SetZero();
			posPlank+=position;
			posPlank+=b2Vec2(c*plankWidth+plankWidth/2,r*m_plankThickness+m_plankThickness/2+r*m_sprinkleThickness);
			m_plank[r*4+c] = new Plank(posPlank,world);

		}
	}
}

Package::~Package(){
	for(int i=0;i<32;i++) delete m_plank[i];
	for(int i=0;i<7;i++) delete m_sprinkle[i];

}

void Package::runSprinkleRemover(){
	b2Vec2 center=b2Vec2(11.0f,25.0f);
	float32 distance=9.0f;
	int sprinkleCount=7;
	for(int i=0;i<sprinkleCount;i++){
		m_sprinkle[i]->removeWhenOutOfBounds(center,distance);
		//BUG: Krasjer når strø flyttes outofbounds + 100 enheter med musa...
	}
}


SprinkleSource::SprinkleSource(b2Body* body, b2World* world){
	m_body=body;
	m_position=m_body->GetPosition();
	m_world=world;
}

void SprinkleSource::create(int id){
	m_position=m_body->GetPosition();
	Sprinkle* sprinkle=new Sprinkle(m_position,m_world);
	m_sprinkle.insert(pair<int,Sprinkle*>(id,sprinkle));
}

void SprinkleSource::destruct(int id){
	//delete (m_sprinkle[id]);
	//m_sprinkle.erase(id);
	typedef multimap<int,Sprinkle*>::iterator t_iterator;
	t_iterator it;
	pair<t_iterator, t_iterator> iterpair = m_sprinkle.equal_range(id);
	for(it=iterpair.first;it!=iterpair.second;it++){
		if((*it).first==id){
			delete ((*it).second);
			m_sprinkle.erase(it);
		}
	}
}

void SprinkleSource::updatePosition(){
	m_position=m_body->GetPosition();
}


PackageSource::PackageSource(b2Vec2 position, b2World* world){
	m_position=position;
	m_world=world;
}

void PackageSource::create(int id){
	Package* package=new Package(m_position,m_world);
	m_package.insert(pair<int,Package*>(id,package));
}

void PackageSource::destruct(int id){
	//delete (m_package[id]);
	//m_package.erase(id);
	typedef multimap<int,Package*>::iterator t_iterator;
	t_iterator it;
	pair<t_iterator, t_iterator> iterpair = m_package.equal_range(id);
	for(it=iterpair.first;it!=iterpair.second;it++){
		if((*it).first==id){
			delete ((*it).second);
			m_package.erase(it);
		}
	}
}

void PackageSource::runSprinkleRemover(){
	multimap<int,Package*>::iterator it;
	for(it=m_package.begin();it!=m_package.end();it++){
		(*it).second->runSprinkleRemover();
	}
}
