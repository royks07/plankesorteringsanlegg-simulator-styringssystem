/*
 * Storages.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "Storages.h"

Storage::Storage(b2Vec2 upperLeftCorner,float32 width,float32 height, float32 wallThickness,b2World* world){
	b2BodyDef bd;
	bd.type=b2_staticBody;
	bd.position=upperLeftCorner;
	b2FixtureDef fd;
	fd.filter.categoryBits=8;
	fd.filter.maskBits=8;

	b2PolygonShape shape;
	//left wall:
	shape.SetAsBox(wallThickness/2,height/2,b2Vec2(wallThickness/2,-height/2),0);
	fd.shape=&shape;
	b2Body* bodyStorage=world->CreateBody(&bd);
	bodyStorage->CreateFixture(&fd);
	//right wall:
	shape.SetAsBox(wallThickness/2,height/2,b2Vec2(width-wallThickness/2,-height/2),0);
	fd.shape=&shape;
	bodyStorage->CreateFixture(&fd);

	//lift:
	bd.type=b2_dynamicBody;
	bd.gravityScale=0.0f;
	bd.position=upperLeftCorner+b2Vec2(width/2,-wallThickness/2);
	b2Body* bodyLift=world->CreateBody(&bd);
	fd.density=0.1f;
	fd.restitution=0.0f;
	shape.SetAsBox(width/2-wallThickness*1.1,wallThickness/2);
	fd.shape=&shape;

	StorageLiftUserData* liftUserData = new StorageLiftUserData;
	fd.userData=liftUserData;

	bodyLift->CreateFixture(&fd);

	//lift joint:
	b2PrismaticJointDef pjd;
	pjd.bodyA=bodyLift;
	pjd.bodyB=bodyStorage;
	pjd.localAnchorA=b2Vec2(0,0);
	pjd.localAnchorB=b2Vec2(width/2,-wallThickness/2);
	pjd.localAxisA=b2Vec2(0,1);

	pjd.enableLimit=true;
	pjd.lowerTranslation=0.0f;
	pjd.upperTranslation=12.0f;

	pjd.enableMotor=true;
	pjd.motorSpeed=0;
	pjd.maxMotorForce=100;

	m_jointLift=(b2PrismaticJoint*)world->CreateJoint(&pjd);

	//stopper:
	bd.type=b2_dynamicBody;
	bd.gravityScale=0.0f;
	bd.position=upperLeftCorner+b2Vec2(width/2,3.0f);
	b2Body* m_bodyStopper=world->CreateBody(&bd);
	fd.density=0.1f;
	fd.restitution=0.0f;
	shape.SetAsBox(0.125f,1.0f);
	fd.shape=&shape;
	m_bodyStopper->CreateFixture(&fd);

	//stopper joint:
	pjd.bodyA=m_bodyStopper;
	pjd.bodyB=bodyStorage;
	pjd.localAnchorA=b2Vec2(0,0);
	pjd.localAnchorB=b2Vec2(width/2,3.0f);
	pjd.localAxisA=b2Vec2(0,1);

	pjd.lowerTranslation=0.0f;
	pjd.upperTranslation=1.5f;

	m_jointStopper=(b2PrismaticJoint*)world->CreateJoint(&pjd);

	//top-sensor-fixture:
	bd.type=b2_staticBody;
	bd.position=upperLeftCorner+b2Vec2(width/2,-wallThickness/16);
	b2Body* bodyTopSensor=world->CreateBody(&bd);

	fd.isSensor=true;
	shape.SetAsBox(width/2-wallThickness,wallThickness/16);
	fd.shape=&shape;
	m_fixtureTopSensor=bodyTopSensor->CreateFixture(&fd);

	//middle-sensor-fixture:
	bd.position=upperLeftCorner+b2Vec2(width/2,-height+wallThickness/16);
	b2Body* bodyMiddleSensor=world->CreateBody(&bd);

	fd.isSensor=true;
	shape.SetAsBox(width/2-wallThickness,wallThickness/16);
	fd.shape=&shape;
	m_fixtureMiddleSensor=bodyMiddleSensor->CreateFixture(&fd);

	//bottom-sensor-fixture:
	bd.position=upperLeftCorner+b2Vec2(width/2,-12.0f-wallThickness);
	b2Body* bodyBottomSensor=world->CreateBody(&bd);

	fd.isSensor=true;
	shape.SetAsBox(width/2-wallThickness,wallThickness/16);
	fd.shape=&shape;
	m_fixtureBottomSensor=bodyBottomSensor->CreateFixture(&fd);
}


StorageArea::StorageArea(int numberOfStorages,b2Vec2 upperLeftCorner,float32 width,float32 height, float32 wallThickness,b2World* world){
	b2Vec2 tmpPos=upperLeftCorner;
	b2Vec2 vecDisplace=b2Vec2(width,0);
	for(int i=0;i<numberOfStorages;i++){
		m_storage.push_back(new Storage(tmpPos,width,height,wallThickness,world));
		tmpPos+=vecDisplace;
	}
}
b2PrismaticJoint* StorageArea::getLiftJoint(int i){
	return m_storage.at(i)->getLiftJoint();
}
b2PrismaticJoint* StorageArea::getStopperJoint(int i){
	return m_storage.at(i)->getStopperJoint();
}
b2Fixture* StorageArea::getTopSensorFixture(int i){
	return m_storage.at(i)->m_fixtureTopSensor;
}
b2Fixture* StorageArea::getMiddleSensorFixture(int i){
	return m_storage.at(i)->m_fixtureMiddleSensor;
}
b2Fixture* StorageArea::getBottomSensorFixture(int i){
	return m_storage.at(i)->m_fixtureBottomSensor;
}




