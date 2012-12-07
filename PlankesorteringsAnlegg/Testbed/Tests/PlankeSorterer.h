
#ifndef PLANKESORTERER_H
#define PLANKESORTERER_H

#include <vector>
#include "../Framework/b2dJson.h"

#include <Box2D\Common\b2Math.h>

#include <list>


class PlankeSorterer : public SimulatorPage
{
public:
	enum vehicleType {
		e_bike,
		e_car,
		e_truck,
		e_tank,
		e_walker,
		e_vehicleMax,
	};

	enum {
		k_left = 	0x01,
		k_stop = 	0x02,
		k_right = 	0x04,
	};


	typedef b2Vec2 pt;



	class PunktListe{
	public:

		void add(b2Vec2 punkt){
			m_punktliste.push_back(punkt);
		}
		void addLinjeStykke(b2Vec2 p1,b2Vec2 p2,int point_count){
			b2Vec2 retningsVektor=p2-p1;
			
			float32 d=retningsVektor.Length()/point_count;
			
			retningsVektor.Normalize();
			
			for(int i=0;i<=point_count;i++){
				b2Vec2 tmpVektor=retningsVektor;
				tmpVektor*=(i*d);
				add(p1+tmpVektor);
			}
		}
		void addSirkelBue(b2Vec2 s,float32 v1,float32 v2,float32 r,int point_count){
			
			float32 dv=(v2-v1)/(point_count-1);
			
			float32 v;
			
			for(int i=0;i<point_count;i++){
				v=v1+dv*i;
				b2Vec2 tmpVektor=s+b2Vec2(r*cos(v),r*sin(v));
				add(tmpVektor);
			}
		
		}

	void addTransportband(float32 lenkeLengde,
						  float32 lenkeBredde,
						  b2Vec2  s1,
						  float32 r1,
						  b2Vec2  s2,
						  float32 r2){

		b2Vec2 s1_til_s2=s2-s1;

		float32 gamma=atan(s1_til_s2.y/s1_til_s2.x);

		float32 d=s1_til_s2.Length();
		float32 dr=abs(r2-r1);
		float32 beta = acos(dr/d);
		float32 alfa = b2_pi - beta;
		
		//Tangeringspunkter mellom sirkler og linjer
		b2Vec2 A;
		b2Vec2 B;
		b2Vec2 C;
		b2Vec2 D;
		
		b2Vec2 s1_til_A=b2Vec2(r1*cos(alfa+gamma),r1*sin(alfa+gamma));
		A=s1+s1_til_A;

		b2Vec2 s2_til_B=s1_til_A;
		s2_til_B*=r2/r1;
		B=s2+s2_til_B;

		b2Vec2 s2_til_C=b2Vec2(r2*cos(-alfa+gamma),r2*sin(-alfa+gamma));
		C=s2+s2_til_C;

		b2Vec2 s1_til_D=s2_til_C;
		s1_til_D*=r1/r2;
		D=s1+s1_til_D;

		addSirkelBue(s1,gamma+alfa+2*beta,gamma+alfa,r1,10);
		addLinjeStykke(A,B,10);
		addSirkelBue(s2,gamma+alfa,gamma-alfa,r2,10);
		addLinjeStykke(C,D,10);
	}


		list<b2Vec2> getList(){
			return m_punktliste;
		}

		list<b2Vec2> m_punktliste;
	
	};


	class Lenke{
	public:
		Lenke(b2Vec2 festepunkt1,
			  b2Vec2 festepunkt2,
			  float32 lengde,
			  float32 bredde,
			  b2World* world):m_festepunkt1(festepunkt1),
							  m_festepunkt2(festepunkt2),
							  m_lengde(lengde),
							  m_bredde(bredde)
		{
			b2BodyDef bd;
			bd.position.Set(getMidtpunkt().x,getMidtpunkt().y);
			/*bd.bullet=true;
			bd.gravityScale=1.0f;*/
			//bd.type=b2_dynamicBody;
			bd.type=b2_staticBody;
			
			m_body=world->CreateBody(&bd);

			b2Vec2 tmpVec;
			tmpVec=m_festepunkt2-m_festepunkt1;
			m_lengde=tmpVec.Length();
			
			b2PolygonShape ps;
			ps.SetAsBox(m_lengde/2,m_bredde/2,b2Vec2_zero,getRotasjonsvinkel());

			b2FixtureDef fd;
			//fd.density=1.0f;
			fd.filter.categoryBits=1;
			fd.filter.groupIndex=8;
			fd.filter.maskBits=2;
			//fd.isSensor=false;
			fd.restitution=0.2f;
			fd.friction=0.2f;
			fd.shape=&ps;

			m_body->CreateFixture(&fd);
			
		}

		b2Vec2 getMidtpunkt(){
			b2Vec2 tmpVec= m_festepunkt1+m_festepunkt2;
			tmpVec*=0.5f;
			return tmpVec;
		}

		float32 getRotasjonsvinkel(){
			b2Vec2 tmpVec=m_festepunkt2-m_festepunkt1;
			return atan(tmpVec.y/tmpVec.x);
		}

		b2Body* getBodyPtr(){
			return m_body;
		}

		b2Vec2 getLokaltFestepunkt1(){
			return m_festepunkt1-getMidtpunkt();
		}

		b2Vec2 getLokaltFestepunkt2(){
			return m_festepunkt2-getMidtpunkt();
		}

		b2Vec2 m_festepunkt1;
		b2Vec2 m_festepunkt2;
		float32 m_lengde;
		float32 m_bredde;

		b2Body* m_body;
	};

	class Kjede{
	public:
		Kjede(b2Vec2  senter1,
			  float32 radius1,
			  b2Vec2  senter2,
			  float32 radius2,
			  b2World *world):m_s1(senter1),
							  m_r1(radius1),
							  m_s2(senter2),
							  m_r2(radius2),
							  m_world(world)
		{

		}

		Kjede(PunktListe punktListe,float32 lengde,float32 tykkelse,b2World *world){
			m_world=world;
			list<b2Vec2> l = punktListe.getList();
			list<b2Vec2>::iterator it;
			b2Vec2 festepunkt1;
			b2Vec2 festepunkt2;
			it=l.begin();
			do{
				festepunkt1=*it;
				it++;
				if(it!=l.end()) {
					festepunkt2=*it;
					add(Lenke(festepunkt1,festepunkt2,lengde,tykkelse,m_world));
					//add(lenke);
				}				

			}while(it!=l.end());
		}
		
		void joinLenker(){
			list<Lenke>::iterator it1,it2;
			it1=it2=m_lenker.begin();
			it2++;
			while(it2!=m_lenker.end()){
				joinToLenker(*it1,*it2);
				//m_joints.push_back(joint);
				it1++;
				it2++;
			}

			joinToLenker(m_lenker.back(),m_lenker.front());
		
		}
	
	private:
		void add(Lenke lenke){
			m_lenker.push_back(lenke);
		}
		
		/*b2RevoluteJoint**/ void joinToLenker(Lenke lenke1,Lenke lenke2){
			b2RevoluteJointDef rejd;
			
			rejd.collideConnected=false;

			rejd.bodyA=lenke1.getBodyPtr();
			b2Vec2 lfa=lenke1.getLokaltFestepunkt2();
			rejd.localAnchorA.Set(lfa.x,lfa.y);
			
			rejd.bodyB=lenke2.getBodyPtr();
			b2Vec2 lfb=lenke2.getLokaltFestepunkt1();
			rejd.localAnchorB.Set(lfb.x,lfb.y);

			//rejd.Initialize(lenke1.getBodyPtr(),lenke2.getBodyPtr(),b2Vec2(lfa.x,lfa.y));
			/*b2RevoluteJoint* rej = (b2RevoluteJoint*)*/m_world->CreateJoint(&rejd);
			/*return rej;*/
		}
		
		b2Vec2 m_s1;
		b2Vec2 m_s2;
		float32 m_r1;
		float32 m_r2;
		b2World *m_world;
		list<Lenke> m_lenker;
		list<b2Joint*> m_joints;

	};

	void plasserLenke(b2Vec2 posisjon,
						  float32 lengde,
						  float32 bredde,
						  float32 rotVinkel,
						  b2World *world){
		b2BodyDef bd;
		bd.position.Set(posisjon.x,posisjon.y);
		bd.type=b2_staticBody;//forel�pig
		bd.gravityScale=1.0f;
		
		
		b2Body *b;
		b=world->CreateBody(&bd);

		b2Vec2 sentrum;
		sentrum.SetZero();
		b2PolygonShape ps;
		ps.SetAsBox(lengde/2,bredde/2,sentrum,rotVinkel);


		b2FixtureDef fd;
		fd.density=1.0f;
		fd.filter.categoryBits=1;
		fd.filter.groupIndex=8;
		fd.filter.maskBits=2;
		fd.friction=0.2f;
		fd.restitution=0.1f;
		fd.shape=&ps;
	
		b->CreateFixture(&fd);
	}

	void plasserKrumKjede(float32 lenkeLengde,
				   float32 lenkeBredde,
				   b2Vec2 sentrum,
				   float32 radius,
				   float32 startvinkel,
				   float32 stopvinkel,
				   b2World *world){

	    //finn dreievinkelen som tilsvarer lengden av en lenke
		float32 deltaVinkel;
		deltaVinkel=2.0f*asin((lenkeLengde*0.5f)/radius);

		b2Vec2 startPunkt;
		startPunkt=sentrum+b2Vec2(radius*cos(startvinkel),radius*sin(startvinkel));

		b2Vec2 sluttPunkt;
		sluttPunkt=sentrum+b2Vec2(radius*cos(stopvinkel),radius*sin(stopvinkel));

		b2Vec2 endepunkt1;
		b2Vec2 endepunkt2;
		b2Vec2 midtpunkt;

		if(startvinkel>stopvinkel) deltaVinkel=-deltaVinkel;


		endepunkt2=startPunkt;

		for(float32 vinkel=startvinkel;abs(vinkel-stopvinkel)>=abs(deltaVinkel);vinkel+=deltaVinkel){
			endepunkt1=endepunkt2;
			endepunkt2=sentrum+b2Vec2(radius*cos(vinkel+deltaVinkel),radius*sin(vinkel+deltaVinkel));
			midtpunkt=(endepunkt1+endepunkt2);
			midtpunkt*=0.5f;
			
			b2Vec2 radiusVektor = midtpunkt-sentrum;

			float32 rotasjon=atan(radiusVektor.y/radiusVektor.x)-b2_pi/2;

			plasserLenke(midtpunkt,lenkeLengde,lenkeBredde,rotasjon,world);
		}


	}

	void plasserRettKjede(float32 lenkeLengde,
				   float32 lenkeTykkelse,
				   b2Vec2 p1,
				   b2Vec2 p2,
				   b2World *world){
		
		b2Vec2 p1_til_p2;
		p1_til_p2=p2-p1;

		b2Vec2 retningsVektor = p1_til_p2;
		retningsVektor.Normalize();

		b2Vec2 p;
		
		float32 rotVinkel = atan(retningsVektor.y/retningsVektor.x);

		float32 halvLenkeLengde=lenkeLengde/2;

		float32 d=p1_til_p2.Length();

		b2Vec2 p1_til_p;
		
		for(float32 f=halvLenkeLengde;f<=d-halvLenkeLengde;	f+=lenkeLengde){
			p1_til_p=retningsVektor;
			p1_til_p*=f;
			p=p1+p1_til_p;
			plasserLenke(p,lenkeLengde,lenkeTykkelse,rotVinkel,world);

		}

	}

	void plasserTransportband(float32 lenkeLengde,float32 lenkeBredde,b2Vec2 s1,float32 r1,b2Vec2 s2,float32 r2,b2World *world){

		b2Vec2 s1_til_s2;
		s1_til_s2=s2-s1;

		float32 gamma=atan(s1_til_s2.y/s1_til_s2.x);

		float32 d=s1_til_s2.Length();
		float32 dr=abs(r2-r1);
		float32 beta = acos(dr/d);
		float32 alfa = b2_pi - beta;
		
		//krysningspunkt mellom sirkelbuer og linjestykker
		b2Vec2 A;
		b2Vec2 B;
		b2Vec2 C;
		b2Vec2 D;
		
		b2Vec2 s1_til_A=b2Vec2(r1*cos(alfa+gamma),r1*sin(alfa+gamma));
		A=s1+s1_til_A;

		b2Vec2 s2_til_B=s1_til_A;
		s2_til_B*=r2/r1;
		B=s2+s2_til_B;

		b2Vec2 s2_til_C=b2Vec2(r2*cos(-alfa+gamma),r2*sin(-alfa+gamma));
		C=s2+s2_til_C;

		b2Vec2 s1_til_D=s2_til_C;
		s1_til_D*=r1/r2;
		D=s1+s1_til_D;


		plasserKrumKjede(lenkeLengde,lenkeBredde,s1,r1,gamma+alfa+2*beta,gamma+alfa,world);
		plasserKrumKjede(lenkeLengde,lenkeBredde,s2,r2,gamma+alfa,gamma-alfa,world);

		plasserRettKjede(lenkeLengde,lenkeBredde,A,B,world);
		plasserRettKjede(lenkeLengde,lenkeBredde,C,D,world);

	}

	PlankeSorterer()
	{		
		m_pCurrentJoints = NULL;

		//load world from JSON file
		b2dJson json;
		b2World* world = json.readFromFile("rube_bak2.json");
		
		b2Vec2 s1;
		s1.x=-8.0f;
		s1.y=4.0f;
		float32 r1=6.0f;

		b2Vec2 s2;
		s2.x=20.0f;
		s2.y=40.0f;
		float32 r2=2.0f;

		//plasserTransportband(0.5f,0.25f,s1,r1,s2,r2,world);

		PunktListe pl;
		//pl.addLinjeStykke(b2Vec2(-5,-5),b2Vec2(7,7),10);
		//pl.addSirkelBue(b2Vec2(-4,-4),b2_pi*0.75,0,3.0f,10);
		pl.addTransportband(0.5f,0.25f,s1,r1,s2,r2);
		//pl.addSirkelBue(s1,0.0f,b2_pi,r1,10);
		//pl.addSirkelBue(s2,b2_pi/2,b2_pi/4*3,r2,10);

		Kjede kjede(pl,0.5f,0.25f,world);
		//kjede.joinLenker();

		if ( world ) {
			//replace testbed's world
			delete m_world;
			m_world = world;

			//re-set standard testbed stuff
			m_world->SetContactListener(this);
			m_world->SetDebugDraw(&m_debugDraw);
			b2BodyDef bodyDef;
			m_groundBody = m_world->CreateBody(&bodyDef);//needed for testbed mousejoint

			//get named drive joints from the JSON info
			json.getJointsByName("bike", m_bikeJoints);
			json.getJointsByName("car", m_carJoints);
			json.getJointsByName("truck", m_truckJoints);
			json.getJointsByName("tank", m_tankJoints);
			json.getJointsByName("walker", m_walkerJoints);
		}

		


		b2BodyDef bd;
		float32 x=0.0f;
		float32 y=30.0f;

		b2Body *b;
		
		b2FixtureDef fd;

		b2PolygonShape ps;

		b2RevoluteJointDef rejd;

		b2Body *prevb;

		ps.SetAsBox((float32)0.25f,(float32)0.1f);


		for(int i=0;i<20;i++){
			
			bd.position.Set(x,y);
			bd.type=b2_dynamicBody;
			bd.gravityScale=1.0f;
			bd.angularDamping=(float32)0.4f;

			b = world->CreateBody(&bd);

			fd.density=(float32)1.0f;
			fd.friction=(float32)0.2f;
			fd.restitution=(float32)0.0f;
			fd.shape=&ps;
			fd.filter.categoryBits=1;
			fd.filter.maskBits=1;
			fd.filter.groupIndex=8;

			b->CreateFixture(&fd);

			if(i>0){
				b2Vec2 v(x-0.25f,y);
				rejd.Initialize(b,prevb,v);

				rejd.collideConnected=false;
				world->CreateJoint(&rejd);

			}

			prevb=b;
			x+=(float32)0.5f;

		}
		

		x-=0.25;
		y+=0.25;

		ps.SetAsBox((float32)0.1f,(float32)0.25f);


		for(int i=0;i<10;i++){
			
			bd.position.Set(x,y);
			bd.type=b2_dynamicBody;
			bd.gravityScale=1.0f;
			bd.angularDamping=(float32)0.4f;

			b = world->CreateBody(&bd);

			fd.density=(float32)1.0f;
			fd.friction=(float32)0.2f;
			fd.restitution=(float32)0.0f;
			fd.shape=&ps;
			fd.filter.categoryBits=1;
			fd.filter.maskBits=1;
			fd.filter.groupIndex=8;

			b->CreateFixture(&fd);

			/*if(i>0)*/{
				b2Vec2 v(x,y-0.25f);
				rejd.Initialize(b,prevb,v);

				rejd.collideConnected=false;
				world->CreateJoint(&rejd);

			}

			prevb=b;
			y+=(float32)0.5f;

		}

		x-=0.25;
		y-=0.25;

		ps.SetAsBox((float32)0.25f,(float32)0.1f);


		for(int i=0;i<20;i++){
			
			bd.position.Set(x,y);
			bd.type=b2_dynamicBody;
			bd.gravityScale=1.0f;
			bd.angularDamping=(float32)0.4f;

			b = world->CreateBody(&bd);

			fd.density=(float32)1.0f;
			fd.friction=(float32)0.2f;
			fd.restitution=(float32)0.0f;
			fd.shape=&ps;
			fd.filter.categoryBits=1;
			fd.filter.maskBits=1;
			fd.filter.groupIndex=8;

			b->CreateFixture(&fd);

			/*if(i>0)*/{
				b2Vec2 v(x+0.25f,y);
				rejd.Initialize(b,prevb,v);

				rejd.collideConnected=false;
				world->CreateJoint(&rejd);

			}

			prevb=b;
			x-=(float32)0.5f;

		}



		m_vehicle = e_bike;
		m_keyState = 0;
		m_pCurrentJoints = &m_bikeJoints;
	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'a': m_keyState += k_left; break;
		case 's': m_keyState += k_stop; break;
		case 'd': m_keyState += k_right; break;
		case 'c':
			m_vehicle = (vehicleType)(m_vehicle+1);
			if ( m_vehicle == e_vehicleMax )
				m_vehicle = e_bike;
			switch ( m_vehicle )
			{
			case e_bike: m_pCurrentJoints = &m_bikeJoints; break;
			case e_car: m_pCurrentJoints = &m_carJoints; break;
			case e_truck: m_pCurrentJoints = &m_truckJoints; break;
			case e_tank: m_pCurrentJoints = &m_tankJoints; break;
			case e_walker: m_pCurrentJoints = &m_walkerJoints; break;
			}

			break;
		}
	}

	void KeyboardUp(unsigned char key)
	{
		switch (key)
		{
		case 'a': m_keyState -= k_left; break;
		case 's': m_keyState -= k_stop; break;
		case 'd': m_keyState -= k_right; break;
		}
	}

	void Step(Settings* settings)
	{
		SimulatorPage::Step(settings);

		m_debugDraw.DrawString(5, m_textLine, "Keys: left = a, brake = s, right = d, change vehicle = c");
		m_textLine += 15;
		m_debugDraw.DrawString(5, m_textLine, "Currently controlling: %s", m_vehicle==e_bike?"bike": m_vehicle==e_car?"car": m_vehicle==e_truck?"monster truck": m_vehicle==e_tank?"tank":"walker");
		m_textLine += 15;

		//set motor speeds for current vehicle
		float32 motorSpeed = m_keyState == k_left ? 1 : m_keyState == k_right ? -1 : 0;
		switch ( m_vehicle )
		{
		case e_bike : motorSpeed *= 30; break;
		case e_car : motorSpeed *= 30; break;
		case e_truck : motorSpeed *= 15; break;
		case e_tank : motorSpeed *= 10; break;
		case e_walker : motorSpeed *= -3; break;
		}
		for (unsigned int i = 0; i < m_pCurrentJoints->size(); i++) {
			switch ( m_vehicle )
			{
			case e_bike :
			case e_walker :
				((b2RevoluteJoint*)m_pCurrentJoints->at(i))->SetMotorSpeed(motorSpeed);
				break;
			default:
				((b2WheelJoint*)m_pCurrentJoints->at(i))->SetMotorSpeed(motorSpeed);
			}
		}

		//move viewpoint to current vehicle
		if ( !m_pCurrentJoints->empty() ) {
			float32 desiredViewCenterX = m_pCurrentJoints->at(0)->GetBodyA()->GetWorldCenter().x;
			settings->viewCenter.x = 0.25f * settings->viewCenter.x + 0.75f * desiredViewCenterX;//avoid 'snapping'
		}
	}

	static SimulatorPage* Create()
	{
		return new PlankeSorterer;
	}
	
	//control state
	vehicleType m_vehicle;
	int m_keyState;

	//drive joints for available vehicles
	vector<b2Joint*> m_bikeJoints;
	vector<b2Joint*> m_carJoints;
	vector<b2Joint*> m_truckJoints;
	vector<b2Joint*> m_tankJoints;
	vector<b2Joint*> m_walkerJoints;

	//reference to drive joints of currently controlled vehicle
	vector<b2Joint*>* m_pCurrentJoints;
};

#endif
