/*
 * Kjede.h
 *
 *  Created on: 13. nov. 2012
 *      Author: roy
 */

#ifndef KJEDE_H_
#define KJEDE_H_

#include <list>

class PunktListe{
	public:
		void add(b2Vec2 punkt){
				m_punktliste.push_back(punkt);
		}
		void start(b2Vec2 startPunkt){
			add(startPunkt);
		}
		void addLinjeStykke(b2Vec2 p1,b2Vec2 p2,int point_count){
			if(p1.x!=p2.x||p1.y!=p2.y){
				b2Vec2 retningsVektor=p2-p1;
				float32 d=retningsVektor.Length()/point_count;
				retningsVektor.Normalize();
				for(int i=1;i<=point_count;i++){
					b2Vec2 tmpVektor=retningsVektor;
					tmpVektor*=(i*d);
					add(p1+tmpVektor);
				}
			}
		}
		void addSirkelBue(b2Vec2 s,float32 v1,float32 v2,float32 r,int point_count){
			float32 dv=(v2-v1)/point_count;
			float32 v;
			for(int i=1;i<=point_count;i++){
				v=v1+dv*i;
				b2Vec2 tmpVektor=s+b2Vec2(r*cos(v),r*sin(v));
				add(tmpVektor);
			}
		}

	void makeTransportband(float32 lenkeLengde,
						  float32 lenkeBredde,
						  b2Vec2  s1,
						  float32 r1,
						  b2Vec2  s2,
						  float32 r2){

		b2Vec2 s1_til_s2=s2-s1;

		float32 gamma=atan(s1_til_s2.y/s1_til_s2.x);//vinkelen til vektoren mellom senterene

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


		int N=2;//antall lenker per lengdeenhet
		float32 lengdeSirkelbue1 = 2*beta*r1;
		float32 lengdeSirkelbue2 = 2*alfa*r2;
		float32 lengdeAB = (A-B).Length();
		float32 lengdeCD = (C-D).Length();

		addSirkelBue(s1,gamma+alfa+2*beta,gamma+alfa,r1,(int)(N*lengdeSirkelbue1));
		addLinjeStykke(A,B,(int)(N*lengdeAB));
		addSirkelBue(s2,gamma+alfa,gamma-alfa,r2,(int)(N*lengdeSirkelbue2));
		addLinjeStykke(C,D,(int)(N*lengdeCD));
	}


		list<b2Vec2> getList(){
			return m_punktliste;
		}

		list<b2Vec2> m_punktliste;

		b2Vec2 m_startPunkt;
		b2Vec2 m_sluttPunkt;
	};


	class Lenke{
	public:
		Lenke(b2Vec2 festepunkt1,
			  b2Vec2 festepunkt2,
			  float32 lengde,
			  float32 bredde,
			  b2Filter filter,
			  b2World* world):m_festepunkt1(festepunkt1),
							  m_festepunkt2(festepunkt2),
							  m_lengde(lengde),
							  m_bredde(bredde)
		{
			b2BodyDef bd;
			bd.position.Set(getMidtpunkt().x,getMidtpunkt().y);
			//bd.bullet=true;
			bd.gravityScale=1.0f;
			bd.type=b2_dynamicBody;
			bd.fixedRotation=false;
			//bd.type=b2_staticBody;

			m_body=world->CreateBody(&bd);

			b2Vec2 tmpVec;
			tmpVec=m_festepunkt2-m_festepunkt1;
			m_lengde=tmpVec.Length();

			b2PolygonShape ps;
			ps.SetAsBox(m_lengde/2,m_bredde/2,b2Vec2_zero,getRotasjonsvinkel());

			b2FixtureDef fd;
			//fd.density=1.0f;
			fd.filter=filter;
			//fd.isSensor=false;
			fd.density=1.0f;
			fd.restitution=0.0f;
			fd.friction=1.0f;
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

		b2Vec2 getUpVector(){
			b2Vec2 tmpVec=m_festepunkt2-m_festepunkt1;
			b2Vec2 normalVec = b2Vec2(-tmpVec.y,tmpVec.x);
			normalVec.Normalize();
			return normalVec;
		}

		b2Vec2 getTangent(){
			b2Vec2 tangentVec=m_festepunkt2-m_festepunkt1;
			tangentVec.Normalize();
			return tangentVec;
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

		float32 getBredde(){
			return m_bredde;
		}

		void setFilter(b2Filter filter){
			m_body->GetFixtureList()->SetFilterData(filter);
		}

		b2Vec2 m_festepunkt1;
		b2Vec2 m_festepunkt2;
		float32 m_lengde;
		float32 m_bredde;

		b2Body* m_body;


	};

	class Medbringer{
	public:
		Medbringer(Lenke* lenke){
			m_body = lenke->getBodyPtr();
			float32 lengde=0.22;//0.55f;
			float32 bredde=0.22;//0.22f;
			b2Vec2 anker(lenke->getUpVector());
			float32 radius=0.22f;
			anker*=lenke->getBredde()/2+radius/2;
			//anker*=lenke->getBredde()/2+bredde/2;
			//b2PolygonShape* shape = new b2PolygonShape;
			//shape->SetAsBox(lengde/2,bredde/2,anker,lenke->getRotasjonsvinkel());
			b2CircleShape* shape = new b2CircleShape;
			shape->m_radius=radius;
			shape->m_p=anker;
			b2FixtureDef fd;
			fd.density=0.1f;
			fd.shape=shape;
			fd.friction=0.0f;
			fd.restitution=0.0f;
			b2Filter filter;
			filter.categoryBits=8;
			filter.maskBits=8;
			filter.groupIndex=0;
			fd.filter=filter;
			m_fixture = m_body->CreateFixture(&fd);
		}
		void setFilter(b2Filter filter){
			//m_body->GetFixtureList()->SetFilterData(filter);
			m_fixture->SetFilterData(filter);
		}
	private:
		b2Fixture* m_fixture;
		b2Body* m_body;
	};



	class Krok{
	public:

		Krok(Lenke* lenke){
			b2Body* body = lenke->getBodyPtr();
			float32 lengde=0.22;//0.55f;
			float32 bredde=1.63;//0.22f;
			b2Vec2 anker(lenke->getUpVector());
			anker*=lenke->getBredde()/2+bredde/2;
			b2PolygonShape* shape = new b2PolygonShape;
			shape->SetAsBox(lengde/2,bredde/2,anker,lenke->getRotasjonsvinkel());
			b2FixtureDef fd;
			fd.density=0.1f;
			fd.shape=shape;
			fd.friction=1.0f;
			fd.restitution=0.0f;
			b2Filter filter;
			filter.categoryBits=8;
			filter.maskBits=8;
			filter.groupIndex=0;
			fd.filter=filter;
			body->CreateFixture(&fd);

			lengde=1.5;//0.55f;
			bredde=0.22;//0.22f;
			anker=lenke->getUpVector();
			anker*=lenke->getBredde()/2+bredde/2+lengde;
			b2Vec2 tangent = lenke->getTangent();
			tangent*=lengde/2-bredde/2;
			anker+=tangent;
			shape->SetAsBox(lengde/2,bredde/2,anker,lenke->getRotasjonsvinkel());
			fd.shape=shape;

			m_fixture=body->CreateFixture(&fd);
		}
		void setFilter(b2Filter filter){
			m_fixture->SetFilterData(filter);
		}
	private:
		b2Fixture* m_fixture;
	};




	class Bjelke{
	public:
		Bjelke(){

		}
		Bjelke(b2Vec2 endepunkt1,b2Vec2 endepunkt2,float32 tykkelse,b2World* world){

			m_endepunkt1=endepunkt1;
			m_endepunkt2=endepunkt2;
			m_retning=m_endepunkt2-m_endepunkt1;
			m_lengde=m_retning.Length();
			m_retning.Normalize();
			m_vinkel=atan(m_retning.y/m_retning.x);
			b2BodyDef bd;
			m_midtpunkt = m_endepunkt1+m_endepunkt2;
			m_midtpunkt*=0.5f;
			float32 lengde=(m_endepunkt2-m_endepunkt1).Length();
			bd.position=m_midtpunkt;
			bd.type=b2_staticBody;
			bd.angle=m_vinkel;
			m_body = world->CreateBody(&bd);
			b2FixtureDef fd;
			fd.density=1.0f;
			fd.friction=0.0f;
			fd.restitution=0.0f;
			b2PolygonShape shape;
			shape.SetAsBox(lengde/2,tykkelse/2);
			fd.shape=&shape;
			m_body->CreateFixture(&fd);
		}

		b2Vec2 getLokaltEndepunkt1(){
			return b2Vec2(-m_lengde/2,0);
		}
		b2Vec2 getLokaltEndepunkt2(){
			return b2Vec2(m_lengde/2,0);
		}
		b2Vec2 getNormal(){
			b2Vec2 tmpVec=getTangent();
			tmpVec=b2Vec2(-tmpVec.y,tmpVec.x);
		}
		b2Vec2 getTangent(){
			return m_retning;
		}

		b2Body* getBodyPtr(){
			return m_body;
		}
		void setFilter(b2Filter filter){
			m_body->GetFixtureList()->SetFilterData(filter);
		}
		float32 getVinkel(){
			return m_vinkel;
		}

		private:
		b2Body* m_body;
		b2Vec2 m_midtpunkt;
		b2Vec2 m_endepunkt1;
		b2Vec2 m_endepunkt2;
		b2Vec2 m_retning;
		float32 m_vinkel;
		float32 m_lengde;
	};

	class Hjul{
	public:
		Hjul(){

		}
		Hjul(b2Vec2 senter,float32 radius,b2World* world){
			m_world=world;
			m_bd.type=b2_dynamicBody;
			m_bd.position=senter;
			m_body = m_world->CreateBody(&m_bd);
			m_fd.density=0.1f;
			m_fd.friction=1.0f;
			m_fd.restitution=0.0f;
			b2CircleShape shape;
			shape.m_radius=radius;
			m_fd.shape=&shape;
			b2Filter filter;
			filter.categoryBits=-1;
			filter.maskBits=2;
			filter.categoryBits=2;
			m_fd.filter=filter;
			m_body->CreateFixture(&m_fd);
		}

		b2Body* getBodyPtr(){
			return m_body;
		}
		void setFilter(b2Filter filter){
			m_body->GetFixtureList()->SetFilterData(filter);
		}
	private:
		b2BodyDef m_bd ;
		b2FixtureDef m_fd;
		b2World* m_world;
		b2Body* m_body;
	};





	class Kjede{

	public:
		Kjede(){

		}

		Kjede(float32 x1,float32 y1,float32 x2,float32 y2,float32 radius,b2World* world){

		}

		Kjede(b2Body* hjul1, b2Body* hjul2,b2Filter filter,b2World* world){
			Kjede(hjul1,hjul2,filter,world,0);
		}


		Kjede(b2Body* hjul1, b2Body* hjul2,b2Filter filter,b2World* world,int krokType):m_lenkeTeller(0){

			float32 m_lenkeLengde=0.5;
			float32 m_lenkeTykkelse=0.25;


			m_hjul1=hjul1;
			m_hjul2=hjul2;
			m_world=world;

			m_krokType=krokType;

			b2Vec2 senterHjul1 = hjul1->GetPosition();
			float32 radiusHjul1 = hjul1->GetFixtureList()->GetShape()->m_radius;
			b2Vec2 senterHjul2 = hjul2->GetPosition();
			float32 radiusHjul2 = hjul2->GetFixtureList()->GetShape()->m_radius;
			PunktListe punktListe;
			punktListe.makeTransportband(0.5,0.25,senterHjul1,radiusHjul1,senterHjul2,radiusHjul2);
			//Kjede(punktListe,0.25f,0.25f,world);



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
					add(Lenke(festepunkt1,festepunkt2,m_lenkeLengde,m_lenkeTykkelse,filter,m_world));
					//add(lenke);
				}

			}while(it!=l.end());

			joinLenker();

		}

		Kjede(PunktListe punktListe,float32 lengde,float32 tykkelse,b2Filter filter,b2World *world){
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
					add(Lenke(festepunkt1,festepunkt2,lengde,tykkelse,filter,m_world));
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

		void setFilter(b2Filter medbringerFilter,b2Filter kjedeFilter){
			list<Lenke>::iterator it=m_lenker.begin();;
			for(;it!=m_lenker.end();it++){
				(*it).setFilter(kjedeFilter);
			}

			list<Medbringer>::iterator it2=medbringere.begin();;
			for(;it2!=medbringere.end();it2++){
				(*it2).setFilter(medbringerFilter);
			}

			list<Krok>::iterator it3=kroker.begin();;
			for(;it3!=kroker.end();it3++){
				(*it3).setFilter(medbringerFilter);
			}
		}

		float32 getLenkeTykkelse(){
			return m_lenkeLengde;
		}


	private:
		void add(Lenke lenke){
			if(m_lenkeTeller%7==0){
				if(m_krokType==0){
					Medbringer medbringer = Medbringer(&lenke);
					medbringere.push_back(medbringer);
				}
				else if(m_krokType==1){
					Krok krok = Krok(&lenke);
					kroker.push_back(krok);
				}
				else if(m_krokType==2){
				}
			}

			m_lenkeTeller++;
			m_lenker.push_back(lenke);
		}

		/*b2RevoluteJoint**/ void joinToLenker(Lenke lenke1,Lenke lenke2){
			b2RevoluteJointDef rejd;

			rejd.collideConnected=false;

			/*rejd.enableLimit=true;
			rejd.lowerAngle=-0.1;
			rejd.upperAngle=0.1;
			*/

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


		void joinToLenker_2(Lenke lenke1,Lenke lenke2){
			b2DistanceJointDef djd;

			djd.length=0.1f;

			djd.bodyA=lenke1.getBodyPtr();
			b2Vec2 lfa=lenke1.getLokaltFestepunkt2();
			djd.localAnchorA.Set(lfa.x,lfa.y);

			djd.bodyB=lenke2.getBodyPtr();
			b2Vec2 lfb=lenke2.getLokaltFestepunkt1();
			djd.localAnchorB.Set(lfb.x,lfb.y);


			//djd.Initialize(lenke1.getBodyPtr(),lenke2.getBodyPtr(),lfa,lfb);
			/*b2RevoluteJoint* rej = (b2RevoluteJoint*)*/m_world->CreateJoint(&djd);
			/*return rej;*/

		}


		float32 m_lenkeLengde;
		float32 m_lenkeTykkelse;
		int m_lenkeTeller;
		b2Vec2 m_s1;
		b2Vec2 m_s2;
		float32 m_r1;
		float32 m_r2;
		b2World *m_world;
		list<Medbringer> medbringere;
		list<Krok> kroker;
		list<Lenke> m_lenker;
		list<b2Joint*> m_joints;
		int m_krokType;

		b2Body* m_hjul1;
		b2Body* m_hjul2;
	};

	class BaneFilter{
	public:
		friend class Bane;
		BaneFilter(){

		}
		BaneFilter(b2Filter filterHjul1,b2Filter filterBjelke,b2Filter filterHjul2,b2Filter filterKjede,b2Filter filterMedbringer){
			m_filterHjul1=filterHjul1;
			m_filterBjelke=filterBjelke;
			m_filterHjul2=filterHjul2;
			m_filterKjede=filterKjede;
			m_filterMedbringer=filterMedbringer;
		}
	protected:
		b2Filter m_filterHjul1;
		b2Filter m_filterBjelke;
		b2Filter m_filterHjul2;
		b2Filter m_filterKjede;
		b2Filter m_filterMedbringer;
	};

	class Bane{
	public:
		Bane(){

		}
			Bane(b2Vec2 endepunkt1,b2Vec2 endepunkt2,float32 tykkelse,BaneFilter baneFilter,b2World* world){
				m_origSpeed=10;
				m_speed=m_origSpeed;
				m_bjelke=Bjelke(endepunkt1,endepunkt2,tykkelse,world);
				m_hjul1=Hjul(endepunkt1,tykkelse/2,world);
				m_hjul2=Hjul(endepunkt2,tykkelse/2,world);
				b2RevoluteJointDef rejd;
				rejd.bodyA=m_hjul1.getBodyPtr();
				rejd.bodyB=m_bjelke.getBodyPtr();
				rejd.enableMotor=true;
				rejd.localAnchorA=b2Vec2(0,0);
				rejd.localAnchorB=m_bjelke.getLokaltEndepunkt1();
				rejd.maxMotorTorque=1000;
				rejd.motorSpeed=m_speed;
				 m_joinHjul1=(b2RevoluteJoint*)world->CreateJoint(&rejd);

				rejd.bodyA=m_hjul2.getBodyPtr();
				//rejd.bodyB=m_bjelke.getBodyPtr();
				//rejd.enableMotor=true;
				rejd.localAnchorA=b2Vec2(0,0);
				rejd.localAnchorB=m_bjelke.getLokaltEndepunkt2();
				//rejd.maxMotorTorque=1000;
				//rejd.motorSpeed=10;
				 m_joinHjul2=(b2RevoluteJoint*)world->CreateJoint(&rejd);
				 b2Filter filter;
				 m_kjede=Kjede(m_hjul1.getBodyPtr(),m_hjul2.getBodyPtr(),filter,world);


				 m_hjul1.setFilter(baneFilter.m_filterHjul1);
				 m_bjelke.setFilter(baneFilter.m_filterBjelke);
				 m_hjul2.setFilter(baneFilter.m_filterHjul2);
				 m_kjede.setFilter(baneFilter.m_filterMedbringer,baneFilter.m_filterKjede);

				 //KjedeLøfter ...for at kjedet ikke skal sige og rives i stykker av tyngdekraften
				 b2Vec2 normal=b2Vec2(0,-1);
				 normal*=(tykkelse/2+m_kjede.getLenkeTykkelse());
				 normal*=1.5f;
				 b2Vec2 p1 = m_bjelke.getLokaltEndepunkt1()+normal;
				 b2Vec2 p2 = m_bjelke.getLokaltEndepunkt2()+normal;

				 b2BodyDef bd;
				 bd.position=m_bjelke.getBodyPtr()->GetPosition();
				 bd.type=b2_staticBody;
				 bd.angle=m_bjelke.getVinkel();
				 b2Body* body =world->CreateBody(&bd);
				 b2FixtureDef fd;
				 fd.density=1.0f;
				 fd.friction=0.0f;
				 fd.restitution=0.0f;
				 b2EdgeShape shape;
				 shape.Set(p1,p2);
				 fd.shape=&shape;
				 body->CreateFixture(&fd);
			}
			void setSpeed(float32 speed){
				m_speed=speed;
				m_joinHjul1->SetMotorSpeed(m_speed);
				m_joinHjul2->SetMotorSpeed(m_speed);
			}
			float32 getSpeed(){
				m_speed=m_joinHjul1->GetMotorSpeed();
				return m_speed;
			}
			float32 getOrigSpeed(){
				m_speed=m_joinHjul1->GetMotorSpeed();
				return m_speed;
			}
			void stop(){
				m_speed=0.0f;
				m_joinHjul1->SetMotorSpeed(m_speed);
				m_joinHjul2->SetMotorSpeed(m_speed);

			}
			void start(){
				m_speed=m_origSpeed;
				m_joinHjul1->SetMotorSpeed(m_origSpeed);
				m_joinHjul2->SetMotorSpeed(m_origSpeed);
			}

			void restart(){
				m_speed=m_origSpeed;
				m_joinHjul1->SetMotorSpeed(m_speed);
				m_joinHjul2->SetMotorSpeed(m_speed);
			}

	private:

			Bjelke m_bjelke;
			Hjul m_hjul1;
			Hjul m_hjul2;
			float32 m_speed;
			float32 m_origSpeed;
			b2RevoluteJoint* m_joinHjul1;
			b2RevoluteJoint* m_joinHjul2;
			Kjede m_kjede;

	};






#endif /* KJEDE_H_ */
