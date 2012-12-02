/*
 * Packaging.h
 *
 *  Created on: 30. nov. 2012
 *      Author: roy
 */

#ifndef PACKAGING_H_
#define PACKAGING_H_

class PackageInput{
public:
	PackageInput(b2Vec2 p,float32 lengthLyingBeam,float32 thicknessLyingBeam,float32 heightStandingBeam,float32 thicknessStandingBeam,b2World* world){
		b2BodyDef bd;
		bd.type=b2_dynamicBody;
		bd.position=p;

		b2Body* bodyBeams=world->CreateBody(&bd);

		//LYING BEAM
		b2PolygonShape shape;
		shape.SetAsBox(lengthLyingBeam/2,thicknessLyingBeam/2,b2Vec2(-lengthLyingBeam/2+thicknessLyingBeam/2,0),0);

		b2FixtureDef fd;
		fd.density=1.0f;
		fd.shape=&shape;

		bodyBeams->CreateFixture(&fd);

		//STANDING BEAM
		shape.SetAsBox(thicknessStandingBeam/2,heightStandingBeam/2,b2Vec2(0,heightStandingBeam/2+thicknessLyingBeam/2),0);

		fd.shape=&shape;

		bodyBeams->CreateFixture(&fd);

		//LIFT
		bd.position=p+b2Vec2(-(lengthLyingBeam-thicknessLyingBeam/2-thicknessStandingBeam/2)/2-thicknessStandingBeam/2,thicknessLyingBeam/2+0.5f);
		b2Body* bodyLift=world->CreateBody(&bd);
		shape.SetAsBox((lengthLyingBeam-thicknessLyingBeam/2-thicknessStandingBeam/2)/2,0.5f);
		fd.shape=&shape;
		bodyLift->CreateFixture(&fd);

		//JOIN LIFT and BEAMS(b2PrismaticJoint)
		b2PrismaticJointDef pjd;
		pjd.bodyA=bodyLift;
		pjd.bodyB=bodyBeams;
		pjd.localAxisA=b2Vec2(0,1);
		pjd.localAnchorB=b2Vec2(-(lengthLyingBeam-thicknessLyingBeam/2-thicknessStandingBeam/2)/2-thicknessStandingBeam/2,thicknessLyingBeam/2+0.5f);
		pjd.collideConnected=true;
		pjd.enableLimit=true;
		pjd.upperTranslation=0;
		pjd.lowerTranslation=-(heightStandingBeam-1.0f);
		pjd.enableMotor=true;
		pjd.maxMotorForce=1000;


		m_jointLift=(b2PrismaticJoint*)world->CreateJoint(&pjd);

		//AXIS
		bd.type=b2_staticBody;
		bd.position=p;
		b2Body* bodyAxis=world->CreateBody(&bd);

		//JOIN AXIS and BEAMS(b2RevoluteJoint)
		b2RevoluteJointDef rejd;
		rejd.bodyA=bodyBeams;
		rejd.bodyB=bodyAxis;
		rejd.enableLimit=true;
		rejd.upperAngle=b2_pi/4;
		rejd.lowerAngle=b2_pi/360.0f*4;
		rejd.enableMotor=true;
		rejd.maxMotorTorque=100000;

		m_jointRotate=(b2RevoluteJoint*)world->CreateJoint(&rejd);
	}

	b2PrismaticJoint* getLiftJoint(){
		return m_jointLift;
	}
	b2RevoluteJoint* getRotateJoint(){
		return m_jointRotate;
	}

	b2PrismaticJoint* m_jointLift;
	b2RevoluteJoint* m_jointRotate;
};

class PackageOutput{
public:
	PackageOutput(b2Vec2 position,b2World* world){
		float32 floorWidth=19.0f;
		float32 floorThickness=1.0f;
		float32 stopperBeamLength=6.8f;
		float32 stopperBeamThickness=0.4f;
		float32 forkBeamThickness=1.0f;
		float32 forkBaseLength=1.6f;
		float32 forkBeamLength=3.4f;
		float32 forkThickness=0.25f;
		float32 liftThickness=1.0f;
		float32 x=(floorWidth-forkBeamThickness)/2;

		//FLOOR-BEAM
		b2BodyDef bd;
		bd.position=position;
		bd.type=b2_staticBody;
		bd.fixedRotation=true;

		b2PolygonShape shape;
		shape.SetAsBox(floorWidth/2,floorThickness/2);

		b2FixtureDef fd;
		fd.restitution=0;
		fd.friction=100;
		fd.density=100;
		fd.shape=&shape;

		b2Body* body=world->CreateBody(&bd);
		body->CreateFixture(&fd);

		//STANDING BEAM
		shape.SetAsBox(stopperBeamThickness/2,stopperBeamLength/2,b2Vec2(-floorWidth/2+x-stopperBeamThickness/2,floorThickness/2+stopperBeamLength/2),0);
		fd.shape=&shape;
		fd.filter.groupIndex=-3;
		body->CreateFixture(&fd);

		//LIFT
		bd.position=position+b2Vec2(-floorWidth/2+(x-stopperBeamThickness)/2,floorThickness/2+liftThickness/2);
		bd.type=b2_dynamicBody;
		b2Body* bodyLift=world->CreateBody(&bd);
		shape.SetAsBox((x-stopperBeamThickness)/2,liftThickness/2);
		fd.shape=&shape;
		fd.filter.groupIndex=0;

		bodyLift->CreateFixture(&fd);

		//FORK
		bd.position=position+b2Vec2(floorWidth/2-forkBeamThickness/2,floorThickness/2+stopperBeamLength-forkThickness-forkBeamLength/2);
		bd.type=b2_dynamicBody;

		b2Body* bodyFork=world->CreateBody(&bd);
		shape.SetAsBox(forkBeamThickness/2,forkBeamLength/2);
		fd.shape=&shape;
		fd.filter.groupIndex=-3;
		bodyFork->CreateFixture(&fd);

		shape.SetAsBox((x+forkBeamThickness)/2,forkThickness/2,b2Vec2(-(x+forkBeamThickness)/2+forkBeamThickness/2,forkBeamLength/2+forkThickness/2),0);
		fd.shape=&shape;
		bodyFork->CreateFixture(&fd);

		//FORKBASE
		bd.position=position+b2Vec2(floorWidth/2-forkBeamThickness/2,floorThickness/2+forkBaseLength/2);
		bd.type=b2_dynamicBody;

		shape.SetAsBox(forkBeamThickness/2,forkBaseLength/2);
		fd.shape=&shape;
		fd.filter.groupIndex=0;

		b2Body* bodyForkBase=world->CreateBody(&bd);
		bodyForkBase->CreateFixture(&fd);

		//JOINT FORKBASE - "FRAME"
		b2PrismaticJointDef pjd;
		pjd.bodyA=bodyForkBase;
		pjd.bodyB=body;
		pjd.localAxisA=b2Vec2(1,0);
		pjd.localAnchorB=b2Vec2(floorWidth/2-forkBeamThickness/2,floorThickness/2+forkBaseLength/2);
		pjd.enableLimit=true;
		pjd.upperTranslation=x;
		pjd.lowerTranslation=0;
		pjd.enableMotor=true;
		pjd.maxMotorForce=10000;
		m_jointForkBase=(b2PrismaticJoint*)world->CreateJoint(&pjd);

		//JOINT FORK - FORKBASE
		pjd.bodyA=bodyFork;
		pjd.bodyB=bodyForkBase;
		pjd.localAxisA=b2Vec2(0,1);
		pjd.localAnchorB=b2Vec2(0,stopperBeamLength-forkThickness-forkBeamLength/2-forkBaseLength/2);

		pjd.upperTranslation=1;
		pjd.lowerTranslation=-forkThickness;

		//pjd.collideConnected=true;
		m_jointFork=(b2PrismaticJoint*)world->CreateJoint(&pjd);

		//JOINT LIFT - "FRAME"
		pjd.bodyA=bodyLift;
		pjd.bodyB=body;
		pjd.localAxisA=b2Vec2(0,1);
		pjd.localAnchorB=b2Vec2(-floorWidth/2+(x-stopperBeamThickness)/2,floorThickness/2+liftThickness/2);

		pjd.upperTranslation=0;
		pjd.lowerTranslation=-(stopperBeamLength-liftThickness);

		//pjd.collideConnected=true;
		m_jointLift=(b2PrismaticJoint*)world->CreateJoint(&pjd);
	}
	b2PrismaticJoint* getForkJoint(){
		return m_jointFork;
	}
	b2PrismaticJoint* getForkBaseJoint(){
		return m_jointForkBase;
	}
	b2PrismaticJoint* getLiftJoint(){
		return m_jointLift;
	}

	b2PrismaticJoint* m_jointForkBase;
	b2PrismaticJoint* m_jointFork;
	b2PrismaticJoint* m_jointLift;
};

#endif /* PACKAGING_H_ */
