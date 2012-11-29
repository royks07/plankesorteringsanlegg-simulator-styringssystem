/*
 * Bane.h
 *
 *  Created on: 24. nov. 2012
 *      Author: roy
 */

#ifndef CONVEYOR_H_
#define CONVEYOR_H_

//#include <list>
#include <vector>
#include "Sensors.h"

class Wheel{
public:
	Wheel(b2Vec2 p,float32 r,int cmBits,b2World* world){
		m_speed=0;
		b2BodyDef bd;
		bd.position=p;
		bd.type=b2_dynamicBody;
		b2FixtureDef fd;
		fd.density=100;
		fd.friction=100.0f;
		fd.restitution=0.0f;
		b2CircleShape shape;
		shape.m_p=b2Vec2(0,0);
		shape.m_radius=r;
		fd.shape=&shape;
		fd.filter.groupIndex=-1;
		fd.filter.categoryBits=cmBits;
		fd.filter.maskBits=cmBits;
		m_body = world->CreateBody(&bd);
		m_body->CreateFixture(&fd);
	}
	void setSpeed(float32 speed){
		m_speed=speed;
	}
	void run(){
		m_body->SetAngularVelocity(m_speed);
		//m_body->SetAngularDamping(0.0f);
	}
	b2Body* getBody(){
		return m_body;
	}
private:
	b2Body* m_body;
	float32 m_speed;
};


class Beam{
public:
	Beam(b2Vec2 p1,b2Vec2 p2,float32 width,int cmBits,b2World* world){
		//center
		b2Vec2 m = p2+p1;
		m*=0.5f;

		//angle
		b2Vec2 direction = p2-p1;
		direction.Normalize();
		float32 angle=atan(direction.y/direction.x);

		//hx
		b2Vec2 vecX=p2-p1;
		float32 hx=vecX.Length()/2;

		//local anchors
		vecX*=0.5f;
		m_localAnchor2=vecX;
		vecX*=-1;
		m_localAnchor1=vecX;

		//hy
		float32 hy=width/2;

		b2BodyDef bd;
		bd.position=m;
		bd.type=b2_staticBody;
		b2FixtureDef fd;
		fd.density=0.1;
		fd.friction=0.0f;
		fd.restitution=0.0f;
		b2PolygonShape shape;
		shape.SetAsBox(hx,hy,b2Vec2(0,0),angle);
		fd.shape=&shape;
		fd.filter.groupIndex=-1;
		fd.filter.categoryBits=cmBits;
		fd.filter.maskBits=cmBits;

		m_body = world->CreateBody(&bd);
		m_body->CreateFixture(&fd);

	}
	b2Vec2& getLocalAnchor1(){
		return m_localAnchor1;
	}
	b2Vec2& getLocalAnchor2(){
		return m_localAnchor2;
	}
	b2Body* getBody(){
		return m_body;
	}
private:
	b2Body* m_body;
	b2Vec2 m_localAnchor1;
	b2Vec2 m_localAnchor2;
};

typedef enum{ingen,knott,krok} t_medbringerType;


class ConveyorBelt{
public:
	ConveyorBelt(b2Vec2 p1,b2Vec2 p2,float32 beamThickness,float32 beltThickness,int N,int M,t_medbringerType medbringerType,int sensorPos,float32 sensorAdjustment,int cmBits,b2World* world){
		m_medbringerType=medbringerType;
		m_paused=false;
		m_p1=p1;
		m_p2=p2;
		b2Vec2 A, B ,C ,D; //points where circlesegments and linesegments meet
		b2Vec2 beamDirection=m_p2-m_p1;
		float32 beamAngle=atan(beamDirection.y/beamDirection.x);
		beamDirection.Normalize();
		b2Vec2 beamNormal=b2Vec2(-beamDirection.y,beamDirection.x);
		float32 length_px_to_X=beamThickness/2+beltThickness/2;
		A=beamNormal;
		A*=length_px_to_X;
		A+=m_p1;
		D=-beamNormal;
		D*=length_px_to_X;
		D+=m_p1;
		B=beamNormal;
		B*=length_px_to_X;
		B+=m_p2;
		C=-beamNormal;
		C*=length_px_to_X;
		C+=m_p2;
		float32 lineSegmentLength=(m_p2-m_p1).Length();
		float32 cirleSegmentLength=0.5f*b2_pi*(beamThickness+beltThickness);
		float32 curveLength=2*cirleSegmentLength+2*lineSegmentLength;
		float32 stepLength=curveLength/N;
		b2Vec2 p=A;
		float32 tmpAngle;
		b2Body* lastBody=NULL;
		b2Body* firstBody=NULL;
		for(int i=0;i<N;i++){
			float32 length=i*stepLength;
			if(length==0){
				//do nothing
			}
			else if(length>0 && length<lineSegmentLength){
				b2Vec2 tmpVec = beamDirection;
				tmpVec*=stepLength;
				p+=tmpVec;
			}else if(length>=lineSegmentLength && length<lineSegmentLength+stepLength){
				float32 firstSectorLength=length-lineSegmentLength;
				tmpAngle=b2_pi/2+beamAngle-firstSectorLength/length_px_to_X;
				b2Vec2 tmpVec=b2Vec2(cos(tmpAngle),sin(tmpAngle));
				tmpVec*=length_px_to_X;
				p=m_p2+tmpVec;
			}else if(length>=lineSegmentLength+stepLength && length<lineSegmentLength+cirleSegmentLength){
				tmpAngle-=(stepLength/length_px_to_X);
				b2Vec2 tmpVec=b2Vec2(cos(tmpAngle),sin(tmpAngle));
				tmpVec*=length_px_to_X;
				p=m_p2+tmpVec;
			}else if(length>=lineSegmentLength+cirleSegmentLength && length<lineSegmentLength+cirleSegmentLength+stepLength){
				float32 firstSegmentLength=length-lineSegmentLength-cirleSegmentLength;
				p=C;
				b2Vec2 tmpVec=-beamDirection;
				tmpVec*=firstSegmentLength;
				p+=tmpVec;
			}else if(length>=(lineSegmentLength+cirleSegmentLength) && length<(2*lineSegmentLength+cirleSegmentLength)){
				b2Vec2 tmpVec=-beamDirection;
				tmpVec*=stepLength;
				p+=tmpVec;
			}else if(length>=(2*lineSegmentLength+cirleSegmentLength) && length-stepLength<(lineSegmentLength+2*cirleSegmentLength)){
				float32 firstSectorLength=length-(lineSegmentLength*2+cirleSegmentLength);
				tmpAngle=3*b2_pi/2+beamAngle-(firstSectorLength/length_px_to_X);
				b2Vec2 tmpVec=b2Vec2(cos(tmpAngle),sin(tmpAngle));
				tmpVec*=length_px_to_X;
				p=m_p1+tmpVec;
			}else{
				tmpAngle-=(stepLength/length_px_to_X);
				b2Vec2 tmpVec=b2Vec2(cos(tmpAngle),sin(tmpAngle));
				tmpVec*=length_px_to_X;
				p=m_p1+tmpVec;
			}
			b2BodyDef bd;
			bd.position=p;
			bd.type=b2_dynamicBody;
			bd.gravityScale=1.0f;
			bd.angle=getAngleAt(p);
			b2Body* body=world->CreateBody(&bd);
			b2FixtureDef fd;
			fd.density=200;//0.1f;
			fd.filter.categoryBits=cmBits;
			fd.filter.maskBits=cmBits;
			fd.filter.groupIndex=-2;
			b2PolygonShape shape;
			shape.SetAsBox(stepLength/2,beltThickness/2);
			fd.shape=&shape;
			body->CreateFixture(&fd);

			//add "medbringer"
			if(i%M==0){
				b2CircleShape cShape;
				b2PolygonShape pShape;

				// 'M' identifiserer en medbringer
				unsigned char* idChar=new unsigned char('M');
				fd.userData=(void*)(idChar);

				fd.density=100.0f;

				switch(m_medbringerType){
					case ingen:
						break;
					case knott:
						cShape.m_p=b2Vec2(0,beltThickness);
						cShape.m_radius=beltThickness/2;
						fd.shape=&cShape;
						fd.filter.groupIndex=-1;
						body->CreateFixture(&fd);
						break;
					case krok:
						float32 armLength=1.40f;
						float32 hookWidth=1.3f;
						float32 hookThickness=0.125f;
						b2Vec2 armPos=b2Vec2(0,beltThickness/2+armLength/2);
						b2Vec2 hookPos=b2Vec2(hookWidth/2-hookThickness/2,armLength+beltThickness/2+hookThickness/2);
						pShape.SetAsBox(hookThickness/2,armLength/2,armPos,0);
						fd.shape=&pShape;
						fd.filter.groupIndex=-1;
						body->CreateFixture(&fd);

						pShape.SetAsBox(hookWidth/2,hookThickness/2,hookPos,0);
						fd.shape=&pShape;
						fd.filter.groupIndex=-1;
						body->CreateFixture(&fd);
						break;
				}
			}

			//set position of conveyorbelt-sensor -- registers state of conveyorbelt
			if(i==sensorPos){
				float32 angle=getAngleAt(p);
				b2Vec2 vecTangent = b2Vec2(cos(angle),sin(angle));
				b2Vec2 vecNormal=b2Vec2(-vecTangent.y,vecTangent.x);
				vecTangent*=sensorAdjustment;
				vecNormal*=(beltThickness*1.5);
				b2Vec2 vecSensorPos=vecNormal+vecTangent;
				vecSensorPos+=p;
				b2Filter filter;
				filter.categoryBits=cmBits;
				filter.maskBits=cmBits;
				m_contactSensor=new ContactSensor("convayorbelt sensor X",vecSensorPos,beltThickness/4,filter,world);
			}


			if(lastBody==NULL){
				firstBody=body;
			}else{
				b2RevoluteJointDef rejd;
				rejd.bodyA=lastBody;
				rejd.bodyB=body;
				rejd.localAnchorA=b2Vec2(stepLength/2,0);
				rejd.localAnchorB=b2Vec2(-stepLength/2,0);
				rejd.collideConnected=false;
				world->CreateJoint(&rejd);
				if(i==N-1){//last body
					rejd.bodyA=body;
					rejd.bodyB=firstBody;
					rejd.localAnchorA=b2Vec2(stepLength/2,0);
					rejd.localAnchorB=b2Vec2(-stepLength/2,0);
					world->CreateJoint(&rejd);

				}
			}
			link.push_back(body);
			lastBody=body;
		}
	}

	float32 getAngleAt(b2Vec2 p){
		float32 angle;
		b2Vec2 u=m_p2-m_p1;
		float32 length_p1_to_p2=u.Length();
		u.Normalize();
		b2Vec2 v=b2Vec2(-u.y,u.x);
		b2Vec2 puv=p-m_p1;
		float pu_length=puv.x*u.x+puv.y*u.y;
		float pv_length=puv.x*v.x+puv.y*v.y;
		b2Vec2 vecTangent;
		if(pu_length>=0 && pu_length<=length_p1_to_p2 && pv_length>=0){
			vecTangent=u;
			angle=atan(u.y/u.x);
		}else if(pu_length>=0 && pu_length<=length_p1_to_p2 && pv_length<0){
			vecTangent=u;
			angle=atan(u.y/u.x)+b2_pi;
		}else if(pu_length>length_p1_to_p2 && pv_length>=0){
			vecTangent=p-m_p2;
			vecTangent.Normalize();
			vecTangent=b2Vec2(-vecTangent.y,vecTangent.x);
			angle=atan(vecTangent.y/vecTangent.x);
		}else if(pu_length>length_p1_to_p2 && pv_length<0){
			vecTangent=p-m_p2;
			vecTangent.Normalize();
			vecTangent=b2Vec2(-vecTangent.y,vecTangent.x);
			angle=atan(vecTangent.y/vecTangent.x)+b2_pi;
		}else if(pu_length<0 && pv_length>=0){
			vecTangent=p-m_p1;
			vecTangent.Normalize();
			vecTangent=b2Vec2(-vecTangent.y,vecTangent.x);
			angle=atan(vecTangent.y/vecTangent.x);
		}else if(pu_length<0 && pv_length<0){
			vecTangent=p-m_p1;
			vecTangent.Normalize();
			vecTangent=b2Vec2(-vecTangent.y,vecTangent.x);
			angle=atan(vecTangent.y/vecTangent.x)+b2_pi;
		}
		return angle;
	}
	void pause(){
		if(!m_paused){
			vector<b2Body*>::iterator it;
			linkSpeed.clear();
			for(it=link.begin();it!=link.end();it++){
				linkSpeed.push_back((*it)->GetLinearVelocity());
				(*it)->SetLinearVelocity(b2Vec2(0,0));
			}
			m_paused=true;
		}
	}
	void play(){
		if(m_paused){
			vector<b2Body*>::iterator it;
			int i=0;
			for(it=link.begin();it!=link.end();it++){
				(*it)->SetLinearVelocity(linkSpeed.at(i));
				i++;
			}
			m_paused=false;
		}
	}
	ContactSensor* getSensor(){
		return m_contactSensor;
	}
	b2Vec2 m_p1;
	b2Vec2 m_p2;
	vector<b2Body*> link;
	vector<b2Vec2> linkSpeed;
	bool m_paused;
	ContactSensor* m_contactSensor;
	t_medbringerType m_medbringerType;
};

class ConveyorBeltShield{
public:
	ConveyorBeltShield(b2Vec2 p1,b2Vec2 p2,float32 beamThickness,float32 beltThickness,int cmBits,b2World* world){
		m_p1=p1;
		m_p2=p2;
		m_beamThickness=beamThickness;
		m_beltThickness=beltThickness;

		b2EdgeShape eshape;
		b2ChainShape shape;
		b2Vec2 vertices[22];

		b2Vec2 u_axis = m_p2-m_p1;

		float32 beamAngle=atan(u_axis.y/u_axis.x);
		float32 beamLength=u_axis.Length();
		b2Vec2 vecBeamLocal=b2Vec2(beamLength,0);

/*
		b2Vec2 v_axis=b2Vec2(u_axis.y,-u_axis.x);//pointing downward
		v_axis.Normalize();
*/

		float32 radius=m_beamThickness/2+beltThickness*2.0f;

		float32 angle_180=b2_pi;
		float32 angle_270=b2_pi/2*3;

		float32 angle;
		float32 angle_delta=b2_pi/2/10; //approximate 1/4 circle by 10 linesegments

		b2Vec2 p_local;

		//circlesegment 1:
		angle=angle_180;
		for(int i=0;i<=10;i++){
			p_local = b2Vec2(cos(angle),sin(angle));
			p_local*=radius;
			vertices[i]=p_local;
			angle+=angle_delta;
		}

		//circlesegment 2
		angle=angle_270;
		for(int i=11;i<=21;i++){
			p_local = b2Vec2(cos(angle),sin(angle));
			p_local*=radius;
			p_local+=vecBeamLocal;
			vertices[i]=p_local;
			angle+=angle_delta;
		}

		shape.CreateChain(vertices,22);

		b2BodyDef bd;
		bd.type=b2_staticBody;
		bd.position=m_p1;
		bd.angle=beamAngle;

		b2FixtureDef fd;
		fd.friction=0.0f;
		fd.restitution=0.0f;
		fd.filter.groupIndex=-1;
		fd.filter.categoryBits=cmBits;
		fd.filter.maskBits=cmBits;
		fd.shape=&shape;

		b2Body* body=world->CreateBody(&bd);
		body->CreateFixture(&fd);
	}
	b2Vec2 m_p1;
	b2Vec2 m_p2;
	float32 m_beamThickness;
	float32 m_beltThickness;
};


class Conveyor{
public:
	Conveyor(b2Vec2 p1, b2Vec2 p2, float32 beamThickness,float32 beltThickness,int N,int M,t_medbringerType medbringerType,int sensorPos,float32 sensorAdjustment,int cmBits, b2World* world){
		m_speed=0;
		m_bPaused=false;
		m_beamThickness=beamThickness;
		m_beltThickness=beltThickness;
		if(p1.x<p2.x){
			m_p1=p1;
			m_p2=p2;
		}else{
			m_p2=p1;
			m_p1=p2;
		}
		Beam beam(m_p1,m_p2,beamThickness,cmBits,world);

		m_wheel1=new Wheel(m_p1,beamThickness/2,cmBits,world);
		m_wheel2=new Wheel(m_p2,beamThickness/2,cmBits,world);
		setSpeed(0);
		//connect wheel1 to beam
		b2RevoluteJointDef rejd;
		rejd.bodyA=m_wheel1->getBody();
		rejd.bodyB=beam.getBody();
		rejd.localAnchorA=b2Vec2(0,0);
		rejd.localAnchorB=beam.getLocalAnchor1();
		world->CreateJoint(&rejd);
		//connect wheel2 to beam
		rejd.bodyA=m_wheel2->getBody();
		rejd.localAnchorB=beam.getLocalAnchor2();
		world->CreateJoint(&rejd);

		m_conveyorBelt=new ConveyorBelt(m_p1,m_p2,m_beamThickness,m_beltThickness,N,M,medbringerType,sensorPos,sensorAdjustment,cmBits,world);
		m_conveyorBeltShield = new ConveyorBeltShield(m_p1,m_p2,m_beamThickness,m_beltThickness,cmBits,world);
	}
	void setSpeed(float32 speed){
		m_speed=speed/(m_beamThickness/2+m_beltThickness);
		m_wheel1->setSpeed(m_speed);
		m_wheel2->setSpeed(m_speed);
	}
	void pause(){
		m_wheel1->setSpeed(0);
		m_wheel2->setSpeed(0);
		m_conveyorBelt->pause();
		m_bPaused=true;

	}
	void play(){
		m_wheel1->setSpeed(m_speed);
		m_wheel2->setSpeed(m_speed);
		m_conveyorBelt->play();
		m_bPaused=false;
	}
	bool isPaused(){
		return m_bPaused;
	}
	void run(){
		m_wheel1->run();
		m_wheel2->run();
		//m_wheelSynchronizer->run();
	}
	Wheel* getWheel1() const {
		return m_wheel1;
	}
	Wheel* getWheel2() const {
		return m_wheel2;
	}
	ContactSensor* getSensor(){
		return m_conveyorBelt->getSensor();
	}
	bool isSensorActivated(){
		return m_conveyorBelt->getSensor()->m_activated;
	}
	Wheel* m_wheel1;
	Wheel* m_wheel2;
	b2Vec2 m_p1;
	b2Vec2 m_p2;
	float32 m_beltThickness;
	float32 m_beamThickness;
	float32 m_speed;
	ConveyorBelt* m_conveyorBelt;
	ConveyorBeltShield* m_conveyorBeltShield;
	bool m_bPaused;
};



#endif /* CONVEYOR_H_ */
