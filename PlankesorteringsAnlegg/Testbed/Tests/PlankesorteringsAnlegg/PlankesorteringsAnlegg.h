
#ifndef _PLANKESORTERINGSANLEGG_H
#define _PLANKESORTERINGSANLEGG_H

//doxygen - eclox eclipse plugin:
//http://mcuoneclipse.com/2012/06/25/5-best-eclipse-plugins-1-eclox-with-doxygen-graphviz-and-mscgen/

#include <string>
#include <sstream>

#include <vector>
#include "../../Framework/b2dJson.h"
#include "Kjede.h"
#include "Pakke.h"
#include "AktuatorFlate.h"
#include "SensorFlate.h"
#include "BaneSynkronisator.h"

#include "Kommunikator.h"



class PlankesorteringsAnlegg : public SimulatorPage
{
public:

	/**
	 * \breif Oppretter et PlankesorteringsAnlegg
	 */
	PlankesorteringsAnlegg():m_communicator("/dev/pts/8"){

		//load world from JSON file
		b2dJson json;
		b2World* world = json.readFromFile("rube.json");
		if ( world ) {
			//replace testbed's world
			delete m_world;
			m_world = world;

			//re-set standard testbed stuff
			//m_world->SetContactListener(this);
			m_sensorFlateTilstand = new SensorFlateTilstand();
			//m_sensorFlateTilstand->setBit(3);
			m_sensorFlate = new SensorFlate(m_sensorFlateTilstand);

			m_world->SetContactListener(m_sensorFlate);

			m_world->SetDebugDraw(&m_debugDraw);
			b2BodyDef bodyDef;

			m_groundBody = m_world->CreateBody(&bodyDef);//needed for testbed mousejoint

			//get named drive joints from the JSON info

			/*json.getJointsByName("bike", m_bikeJoints);
			json.getJointsByName("car", m_carJoints);
			json.getJointsByName("truck", m_truckJoints);
			json.getJointsByName("tank", m_tankJoints);
			json.getJointsByName("walker", m_walkerJoints);
*/

			//HEIS:
			b2RevoluteJoint* jointPakkeVender = (b2RevoluteJoint*)json.getJointByName("jointPakkeVender");
			jointPakkeVender->EnableMotor(true);
			jointPakkeVender->SetMotorSpeed(0.0f);
			jointPakkeVender->SetMaxMotorTorque(2000);
			jointPakkeVender->EnableLimit(false);

			Aktuator pakkeVenderFram = Aktuator(jointPakkeVender,'1',0.2);
			Aktuator pakkeVenderTilbake = Aktuator(jointPakkeVender,'q',-0.2f);

			m_aktuatorFlate.addAktuator(pakkeVenderFram);
			m_aktuatorFlate.addAktuator(pakkeVenderTilbake);


			b2PrismaticJoint* jointPakkeLofter = (b2PrismaticJoint*)json.getJointByName("jointPakkeLofter");
			jointPakkeLofter->EnableMotor(true);
			jointPakkeLofter->SetMotorSpeed(0.0);
			jointPakkeLofter->SetMaxMotorForce(1000);
			jointPakkeLofter->EnableLimit(true);
			jointPakkeLofter->SetLimits(-4.3f,1.5f);

			Aktuator pakkeLofterOpp = Aktuator(jointPakkeLofter,'2',-1);
			Aktuator pakkeLofterNed = Aktuator(jointPakkeLofter,'w',1);

			m_aktuatorFlate.addAktuator(pakkeLofterOpp);
			m_aktuatorFlate.addAktuator(pakkeLofterNed);


			//STOPPERE
			//1
			b2PrismaticJoint* jointStopper1 = (b2PrismaticJoint*)json.getJointByName("jointStopper1");
			jointStopper1->EnableMotor(true);
			jointStopper1->SetMotorSpeed(0.0);
			jointStopper1->SetMaxMotorForce(1000);
			jointStopper1->EnableLimit(true);
			jointStopper1->SetLimits(2.3f,3.63f);

			Aktuator stopper1Opp = Aktuator(jointStopper1,'7',50);
			Aktuator stopper1Ned = Aktuator(jointStopper1,'u',-50);

			m_aktuatorFlate.addAktuator(stopper1Opp);
			m_aktuatorFlate.addAktuator(stopper1Ned);

			//2
			b2PrismaticJoint* jointStopper2 = (b2PrismaticJoint*)json.getJointByName("jointStopper2");
			jointStopper2->EnableMotor(true);
			jointStopper2->SetMotorSpeed(0.0);
			jointStopper2->SetMaxMotorForce(1000);
			jointStopper2->EnableLimit(true);
			jointStopper2->SetLimits(2.3f,3.63f);

			Aktuator stopper2Opp = Aktuator(jointStopper2,'8',50);
			Aktuator stopper2Ned = Aktuator(jointStopper2,'i',-50);

			m_aktuatorFlate.addAktuator(stopper2Opp);
			m_aktuatorFlate.addAktuator(stopper2Ned);

			//3
			b2PrismaticJoint* jointStopper3 = (b2PrismaticJoint*)json.getJointByName("jointStopper3");
			jointStopper3->EnableMotor(true);
			jointStopper3->SetMotorSpeed(0.0);
			jointStopper3->SetMaxMotorForce(1000);
			jointStopper3->EnableLimit(true);
			jointStopper3->SetLimits(2.3f,3.63f);

			Aktuator stopper3Opp = Aktuator(jointStopper3,'9',50);
			Aktuator stopper3Ned = Aktuator(jointStopper3,'o',-50);

			m_aktuatorFlate.addAktuator(stopper3Opp);
			m_aktuatorFlate.addAktuator(stopper3Ned);


			//LOMMEHEISER
			//1
			b2PrismaticJoint* jointLommeHeis1 = (b2PrismaticJoint*)json.getJointByName("jointLommeHeis1");
			jointLommeHeis1->EnableMotor(true);
			jointLommeHeis1->SetMotorSpeed(0.0);
			jointLommeHeis1->SetMaxMotorForce(1000);
			jointLommeHeis1->EnableLimit(true);
			jointLommeHeis1->SetLimits(5.72f,19.0f);

			Aktuator lommeHeis1Opp = Aktuator(jointLommeHeis1,'j',-50);
			Aktuator lommeHeis1Ned = Aktuator(jointLommeHeis1,'m',50);

			m_aktuatorFlate.addAktuator(lommeHeis1Opp);
			m_aktuatorFlate.addAktuator(lommeHeis1Ned);

			//2
			b2PrismaticJoint* jointLommeHeis2 = (b2PrismaticJoint*)json.getJointByName("jointLommeHeis2");
			jointLommeHeis2->EnableMotor(true);
			jointLommeHeis2->SetMotorSpeed(0.0);
			jointLommeHeis2->SetMaxMotorForce(1000);
			jointLommeHeis2->EnableLimit(true);
			jointLommeHeis2->SetLimits(5.72f,19.0f);

			Aktuator lommeHeis2Opp = Aktuator(jointLommeHeis2,'k',-50);
			Aktuator lommeHeis2Ned = Aktuator(jointLommeHeis2,'\,',50);

			m_aktuatorFlate.addAktuator(lommeHeis2Opp);
			m_aktuatorFlate.addAktuator(lommeHeis2Ned);

			//3
			b2PrismaticJoint* jointLommeHeis3 = (b2PrismaticJoint*)json.getJointByName("jointLommeHeis3");
			jointLommeHeis3->EnableMotor(true);
			jointLommeHeis3->SetMotorSpeed(0.0);
			jointLommeHeis3->SetMaxMotorForce(1000);
			jointLommeHeis3->EnableLimit(true);
			jointLommeHeis3->SetLimits(5.72f,19.0f);

			Aktuator lommeHeis3Opp = Aktuator(jointLommeHeis3,'l',-50);
			Aktuator lommeHeis3Ned = Aktuator(jointLommeHeis3,'\.',50);

			m_aktuatorFlate.addAktuator(lommeHeis3Opp);
			m_aktuatorFlate.addAktuator(lommeHeis3Ned);


			//GAFFEL
			//gaffelsokkel
			b2PrismaticJoint* jointGaffelSokkel = (b2PrismaticJoint*)json.getJointByName("jointGaffelSokkel");
			jointGaffelSokkel->EnableMotor(true);
			jointGaffelSokkel->SetMotorSpeed(0.0);
			jointGaffelSokkel->SetMaxMotorForce(1000);
			jointGaffelSokkel->EnableLimit(true);
			jointGaffelSokkel->SetLimits(0.9f,10.0f);

			Aktuator gaffelSokkelFram = Aktuator(jointGaffelSokkel,'a',1);
			Aktuator gaffelSokkelTilbake = Aktuator(jointGaffelSokkel,'s',-1);

			m_aktuatorFlate.addAktuator(gaffelSokkelFram);
			m_aktuatorFlate.addAktuator(gaffelSokkelTilbake);

			//gaffel
			b2PrismaticJoint* jointGaffel = (b2PrismaticJoint*)json.getJointByName("jointGaffel");
			jointGaffel->EnableMotor(true);
			jointGaffel->SetMotorSpeed(0.0);
			jointGaffel->SetMaxMotorForce(1000);
			jointGaffel->EnableLimit(true);
			jointGaffel->SetLimits(5.1f,5.7f);

			Aktuator gaffelOpp = Aktuator(jointGaffel,'d',-1);
			Aktuator gaffelNed = Aktuator(jointGaffel,'c',1);

			m_aktuatorFlate.addAktuator(gaffelOpp);
			m_aktuatorFlate.addAktuator(gaffelNed);

			//STRØKASSETT
			b2PrismaticJoint* jointStrokassett = (b2PrismaticJoint*)json.getJointByName("jointStrokassett");
			jointStrokassett->EnableMotor(true);
			jointStrokassett->SetMotorSpeed(0.0);
			jointStrokassett->SetMaxMotorForce(1000);
			jointStrokassett->EnableLimit(true);
			jointStrokassett->SetLimits(10.84f,10.84f*2);

			Aktuator strokassettOpp = Aktuator(jointStrokassett,'f',-3);
			Aktuator strokassettNed = Aktuator(jointStrokassett,'v',3);

			m_aktuatorFlate.addAktuator(strokassettOpp);
			m_aktuatorFlate.addAktuator(strokassettNed);

			//PAKKELØFTER VED SLUTT
			b2PrismaticJoint* jointPakkeLofterSlutt = (b2PrismaticJoint*)json.getJointByName("jointPakkeLofterSlutt");
			jointPakkeLofterSlutt->EnableMotor(true);
			jointPakkeLofterSlutt->SetMotorSpeed(0.0);
			jointPakkeLofterSlutt->SetMaxMotorForce(1000);
			jointPakkeLofterSlutt->EnableLimit(true);
			jointPakkeLofterSlutt->SetLimits(-2.3f+3.77f,3.77f*2);

			Aktuator pakkeLofterSluttOpp = Aktuator(jointPakkeLofterSlutt,'g',-3);
			Aktuator pakkeLofterSluttNed = Aktuator(jointPakkeLofterSlutt,'b',3);

			m_aktuatorFlate.addAktuator(pakkeLofterSluttOpp);
			m_aktuatorFlate.addAktuator(pakkeLofterSluttNed);


			//STRØKASSETT
			b2Body* bodyStrokassett = (b2Body*)json.getBodyByName("bodyStrokassett");

			Aktuator strokassett = Aktuator(bodyStrokassett,'n');

			m_aktuatorFlate.addAktuator(strokassett);


			//SENSOR_TEST

			b2Fixture* fixtureTestSensor = (b2Fixture*)json.getFixtureByName("sensorTest");
			fixtureTestSensor->SetSensor(true);
			m_testSensor = new TellendeSensor(fixtureTestSensor,"Test Sensor",60,63);
			m_sensorFlate->addSensor((Sensor*)m_testSensor);

			//testplanke for testsensor
			m_testPlanke = (b2Body*)json.getBodyByName("bodyTestPlanke");


			//SENSORER:
			b2Fixture* fixSensor[19];
			Sensor* sensor[19];
			stringstream ssSensor;
			string sSensor;
			for(int i=1;i<=19;i++){
				ssSensor.str("");
				ssSensor.clear();
				ssSensor<<"s"<<i;
				sSensor=ssSensor.str();
				fixSensor[i]= (b2Fixture*)json.getFixtureByName(sSensor);
				fixSensor[i]->SetSensor(true);
				sensor[i] = new Sensor(fixSensor[i],sSensor,i,i);
				m_sensorFlate->addSensor(sensor[i]);
			}
			//knottTeller

			b2Fixture* fixtureKnottTeller = (b2Fixture*)json.getFixtureByName("s6");
			fixtureKnottTeller->SetSensor(true);
			m_knottTeller = new TellendeSensor(fixtureKnottTeller,"Knott teller",20,24);
			m_sensorFlate->addSensor((Sensor*)m_knottTeller);

			b2Fixture* fixtureKrokTeller = (b2Fixture*)json.getFixtureByName("s7");
			fixtureKrokTeller->SetSensor(true);
			m_krokTeller = new TellendeSensor(fixtureKrokTeller,"Krok teller",25,29);
			m_sensorFlate->addSensor((Sensor*)m_krokTeller);



			m_testPlanke = (b2Body*)json.getBodyByName("bodyTestPlanke");




			//HJUL:
			string strJointHjul;
			string strBodyHjul;
			stringstream ss;
			b2Body* bodyHjul[16];

			for(int i=1;i<=15;i++){
				ss.str("");
				ss.clear();
				ss<<"jointHjul"<<i;
				strJointHjul=ss.str();


				cout<<strJointHjul<<endl;//test

				jointsHjul[i] = (b2RevoluteJoint*)json.getJointByName(strJointHjul);
				jointsHjul[i]->EnableMotor(true);
				jointsHjul[i]->EnableLimit(false);
				if(i==4 || i==5) jointsHjul[i]->SetMotorSpeed(10.0f);
				else if(i>5)	jointsHjul[i]->SetMotorSpeed(-1.0f);
				else jointsHjul[i]->SetMotorSpeed(1.0f);
				jointsHjul[i]->SetMaxMotorTorque(1000);

				ss.str("");
				ss<<"bodyHjul"<<i;
				strBodyHjul=ss.str();

				cout<<strBodyHjul<<endl;//test

				bodyHjul[i] = (b2Body*)json.getBodyByName(strBodyHjul);


			}

			/*float32 X = bodyHjul[1]->GetPosition().x;
			float32 Y = bodyHjul[1]->GetPosition().y;
			cout<<"bodyHjul"<<1<<": X="<<X<<" Y="<<Y<<endl;

			X = bodyHjul[2]->GetPosition().x;
			Y = bodyHjul[2]->GetPosition().y;
			cout<<"bodyHjul"<<2<<": X="<<X<<" Y="<<Y<<endl;
 	 	 	 */

			//KJEDER:


			b2Vec2 senter1A =b2Vec2(55.0f,45.0f);
			b2Vec2 senter2A =b2Vec2(75.0f,45.0f);
			BaneFilter baneFilter= BaneFilter();
			Bane* baneA = new Bane(senter1A,senter2A,2,baneFilter,world);

			b2Vec2 senter1B =b2Vec2(55.0f,40.0f);
			b2Vec2 senter2B =b2Vec2(75.0f,40.0f);
			Bane* baneB = new Bane(senter1B,senter2B,2,baneFilter,world);

			b2Vec2 senter1C =b2Vec2(55.0f,35.0f);
			b2Vec2 senter2C =b2Vec2(75.0f,35.0f);
			Bane* baneC = new Bane(senter1C,senter2C,2,baneFilter,world);


			//BaneSynkronisator
			b2Fixture* fixturefaseSensorA =json.getFixtureByName("faseSensorA");
			FaseSensor* faseSensorA = new FaseSensor(fixturefaseSensorA,"FaseSensor A");
			BaneSensorPar* baneSensorParA = new BaneSensorPar(baneA,faseSensorA);

			b2Fixture* fixturefaseSensorB =json.getFixtureByName("faseSensorB");
			FaseSensor* faseSensorB = new FaseSensor(fixturefaseSensorB,"FaseSensor B");
			BaneSensorPar* baneSensorParB = new BaneSensorPar(baneB,faseSensorB);

			b2Fixture* fixturefaseSensorC =json.getFixtureByName("faseSensorC");
			FaseSensor* faseSensorC = new FaseSensor(fixturefaseSensorC,"FaseSensor C");
			BaneSensorPar* baneSensorParC = new BaneSensorPar(baneC,faseSensorC);


			m_sensorFlate->addSensor((Sensor*)faseSensorA);
			m_sensorFlate->addSensor((Sensor*)faseSensorB);
			m_sensorFlate->addSensor((Sensor*)faseSensorC);

			m_baneSynkronisator=new BaneSynkronisator;
			m_baneSynkronisator->addBaneSensorPar(baneSensorParA);
			m_baneSynkronisator->addBaneSensorPar(baneSensorParB);
			m_baneSynkronisator->addBaneSensorPar(baneSensorParC);



/*			m_baneSynkronisatorAB=new BaneSynkronisator2(baneA,baneB,faseSensorA,faseSensorB);
			m_baneSynkronisatorBC=new BaneSynkronisator2(baneB,baneC,faseSensorB,faseSensorC);
*/

			b2Filter kjede1Filter;
			kjede1Filter.categoryBits=1;
			kjede1Filter.maskBits=1;
			Kjede kjede1 = Kjede(bodyHjul[1],bodyHjul[2],kjede1Filter,world);

			b2Filter kjede2Filter;
			kjede2Filter.categoryBits=2;
			kjede2Filter.maskBits=2;
			Kjede kjede2 = Kjede(bodyHjul[2],bodyHjul[3],kjede2Filter,world);

			b2Filter kjede3Filter;
			kjede3Filter.categoryBits=1;
			kjede3Filter.maskBits=1;
			Kjede kjede3 = Kjede(bodyHjul[4],bodyHjul[5],kjede3Filter,world,2);

			b2Filter kjede4Filter;
			kjede4Filter.categoryBits=2;
			kjede4Filter.maskBits=2;
			Kjede kjede4 = Kjede(bodyHjul[6],bodyHjul[7],kjede4Filter,world,1);

			b2Filter kjede5Filter;
			kjede5Filter.categoryBits=1;
			kjede5Filter.maskBits=1;
			Kjede kjede5 = Kjede(bodyHjul[9],bodyHjul[8],kjede5Filter,world,2);

			b2Filter kjede6Filter;
			kjede6Filter.categoryBits=1;
			kjede6Filter.maskBits=1;
			Kjede kjede6 = Kjede(bodyHjul[11],bodyHjul[10],kjede6Filter,world);

			b2Filter kjede7Filter;
			kjede7Filter.categoryBits=2;
			kjede7Filter.maskBits=2;
			Kjede kjede7 = Kjede(bodyHjul[13],bodyHjul[12],kjede7Filter,world,2);

			b2Filter kjede8Filter;
			kjede8Filter.categoryBits=1;
			kjede8Filter.maskBits=1;
			Kjede kjede8 = Kjede(bodyHjul[15],bodyHjul[14],kjede8Filter,world,2);


		}


//		m_sensorFlateTilstand.printBitSekvensAsString();

	}


	    void testRender() {
	      glColor3f(1,1,1);

	      glPointSize(4);
	      glBegin(GL_POINTS);
	      glVertex2f( 0, 0 );
	      glVertex2f(-0.5, 0.5 );
	      glVertex2f( 0.5, 0.5 );
	      glEnd();

	      glBegin(GL_LINES);
	      glVertex2f(-0.5,  -0.5 );
	      glVertex2f(-0.16, -0.6 );
	      glVertex2f( 0.16, -0.6 );
	      glVertex2f( 0.5,  -0.5 );
	      glEnd();

	      glBegin(GL_LINE_LOOP);
	      for (float a = 0; a < 360.0f * b2_pi/180.0f; a += 30.0f * b2_pi/180.0f)
	        glVertex2f( sinf(a), cosf(a) );
	      glEnd();
	    }




	void Step(Settings* settings)
	{
		SimulatorPage::Step(settings);

		//m_sensorFlateTilstand.printBitSekvens();

		string str =m_sensorFlateTilstand->toString();
		m_debugDraw.DrawString(50,100,str.c_str());

/*		string strTeller =m_testSensor->toString();
		m_debugDraw.DrawString(50,120,strTeller.c_str());
*/
		string strKnottTeller =m_knottTeller->toString();
		m_debugDraw.DrawString(50,140,strKnottTeller.c_str());

		string strKrokTeller =m_krokTeller->toString();
		m_debugDraw.DrawString(50,160,strKrokTeller.c_str());



		/*		b2Transform t = b2Transform();
		t.SetIdentity();
		t.Set(b2Vec2(0,500),b2_pi/2);
		m_debugDraw.DrawTransform(t);
		m_debugDraw.DrawString(300,300,"Test-streng...");
*/
		testRender();

		/*m_baneSynkronisatorAB->run();
		m_baneSynkronisatorBC->run();*/
		//,
		m_baneSynkronisator->run();




		if(m_communicator.dataIsPresent()){
			unsigned char c = m_communicator.read();
			//m_communicator.write(c);
			Keyboard(c);
		}
	}





	void Keyboard(unsigned char key)
	{
		m_aktuatorFlate.switchActuators(key);


	}

	void KeyboardUp(unsigned char key)
	{

	}

	static SimulatorPage* Create()
	{
		return new PlankesorteringsAnlegg;
	}
	


	b2RevoluteJoint* jointsHjul[16];

	AktuatorFlate m_aktuatorFlate;


	b2Body* m_testPlanke;

	bool m_bTouching;

	SensorFlate* m_sensorFlate;
	SensorFlateTilstand* m_sensorFlateTilstand;
	//Sensor* m_testSensor;
	TellendeSensor* m_testSensor;

	TellendeSensor* m_knottTeller;
	TellendeSensor* m_krokTeller;



	/*BaneSynkronisator2* m_baneSynkronisatorAB;
	BaneSynkronisator2* m_baneSynkronisatorBC;*/
	BaneSynkronisator* m_baneSynkronisator;

	Communicator m_communicator;
};



#endif
