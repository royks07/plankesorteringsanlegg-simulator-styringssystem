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

		world->CreateJoint(&pjd);

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
		rejd.lowerAngle=0;

		world->CreateJoint(&rejd);




	}
};


#endif /* PACKAGING_H_ */
