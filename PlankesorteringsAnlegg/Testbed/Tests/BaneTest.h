/*
 * BaneTest.h
 *
 *  Created on: 24. nov. 2012
 *      Author: roy
 */

#ifndef BANETEST_H_
#define BANETEST_H_

#include "PlankesorteringsAnlegg/Conveyor.h"
#include "PlankesorteringsAnlegg/ConveyorSynchronizer.h"
#include "PlankesorteringsAnlegg/Storages.h"
#include "PlankesorteringsAnlegg/Actuators.h"
#include "PlankesorteringsAnlegg/Pakke.h"
#include "PlankesorteringsAnlegg/Packaging.h"

class BaneTest : public Test
{
public:
	BaneTest(){

		m_actuatorSet=new ActuatorSet;

		//StorageArea(numberOfStorages,upperLeftCorner,width,height,wallThickness,world){
		StorageArea* storageArea = new StorageArea(10,b2Vec2(57.3f,24.8f),9,5,0.5f,m_world);
		for(int i=10;i<=19;i++){
			// add stoppers to actuatorset
			JointActuatorPrismaticBinary* stopperActuator = new JointActuatorPrismaticBinary(i,storageArea->getStopperJoint(i-10));
			m_actuatorSet->add(stopperActuator);
		}
		for(int i=20;i<=29;i++){
			//add lifts to actuatorset
			JointActuatorPrismaticStep* liftActuator = new JointActuatorPrismaticStep(i,storageArea->getLiftJoint(i-20));
			m_actuatorSet->add(liftActuator);
		}

		m_commandSequenceInterpreter=new CommandSequenceInterpreter(m_actuatorSet);


		// Conveyor(p1,p2,thickness,beltThickness,N,M,medbringerType,sensorPos,sensorAdjustment,cmBits,world)

		//CONVEYOR1
		b2Vec2 p1(23.4f,17.8f);
		b2Vec2 p2(29.9f,24.2f);
		m_conveyor1=new Conveyor(p1,p2,2.0f,0.125f,49,7,knott,5,0.25f,1,m_world);
		m_conveyor1->setSpeed(-2.0f);

		//CONVEYOR2
		b2Vec2 p3(29.9f,24.2f);
		b2Vec2 p4(49.4f,24.2f);
		m_conveyor2=new Conveyor(p3,p4,2.0f,0.125f,91,7,knott,7,0,2,m_world);
		m_conveyor2->setSpeed(-2.0f);

		//CONVEYOR3
		b2Vec2 p5(49.6f,24.9f);
		b2Vec2 p6(56.3f,24.9f);
		m_conveyor3=new Conveyor(p5,p6,0.7f,0.0625f,56,14,ingen,3,0,1,m_world);
		m_conveyor3->setSpeed(-6.0f);

		//CONVEYOR4
		b2Vec2 p7(55.0f,27.9f);
		b2Vec2 p8(150/*74.3f*/,27.9f);
		m_conveyor4=new Conveyor(p7,p8,2.0f,0.125f,350,7,krok,10,0,2,m_world);
		m_conveyor4->setSpeed(2.0f);

		//CONVEYOR5
		b2Vec2 p9(41.5f,11.9f);
		b2Vec2 p10(150,11.9f);
		m_conveyor5=new Conveyor(p9,p10,2.0f,0.125f,200,7,ingen,3,0,2,m_world);
		m_conveyor5->setSpeed(2.0f);

		//CONVEYOR6
		b2Vec2 p11(26.6f,11.6f);
		b2Vec2 p12(33.2f,5.2f);
		m_conveyor6=new Conveyor(p11,p12,2.0f,0.125f,49,7,knott,5,0,1,m_world);
		m_conveyor6->setSpeed(2.0f);

		//CONVEYOR7
		b2Vec2 p13(17.1f,9.9f);
		b2Vec2 p14(26.5f,12.3f);
		m_conveyor7=new Conveyor(p13,p14,0.7f,0.0625f,56,14,ingen,3,0,2,m_world);
		m_conveyor7->setSpeed(2.0f);

		//CONVEYOR8
		b2Vec2 p15(9.2f,9.1f);
		b2Vec2 p16(16.0f,9.1f);
		m_conveyor8=new Conveyor(p15,p16,0.7f,0.0625f,56,14,ingen,3,0,2,m_world);
		m_conveyor8->setSpeed(2.0f);


		//SLIDE1
		b2Vec2 p17(16.7f,25.0f);
		//b2Vec2 p18(22.3f,19.4f);
		Beam(p17,p1,2.0f,4,m_world);

		//SLIDE2
		/*b2Vec2 p19(9.2f,9.1f);
		b2Vec2 p20(16.0f,9.1f);*/
		Beam(p9,p12,2.0f,4,m_world);

		//PACKAGE
		b2Vec2 pakkePos = b2Vec2(4.6f,22.2f);
		m_pakke = new Pakke(pakkePos,m_world);

		//PACKAGE-INPUT:
		PackageInput* packageInput=new PackageInput(b2Vec2(11.9f,20.6f),8.6f,1.17f,7.0f,0.37f,m_world);

		JointActuatorPrismaticStep* liftPackageActuator = new JointActuatorPrismaticStep(1,packageInput->getLiftJoint());
		m_actuatorSet->add(liftPackageActuator);

		JointActuatorRevoluteStep* rotatePackageActuator = new JointActuatorRevoluteStep(2,packageInput->getRotateJoint());
		m_actuatorSet->add(rotatePackageActuator);



		SensorField* sensorField = new SensorField();
		sensorField->add(m_conveyor1->getSensor());
		sensorField->add(m_conveyor2->getSensor());
		//sensorField->add(m_conveyor3->getSensor());
		sensorField->add(m_conveyor4->getSensor());
		//sensorField->add(m_conveyor5->getSensor());

		m_world->SetContactListener(sensorField);

		m_conveyorSynchronizer=new ConveyorSynchronizer();
		m_conveyorSynchronizer->add(m_conveyor1);
		m_conveyorSynchronizer->add(m_conveyor2);
		//m_conveyorSynchronizer->add(m_conveyor3);
		m_conveyorSynchronizer->add(m_conveyor4);
		//m_conveyorSynchronizer->add(m_conveyor5);

	}


	void Step(Settings *settings){
		Test::Step(settings);
		m_conveyor1->run();
		m_conveyor2->run();
		m_conveyor3->run();
		m_conveyor4->run();
		m_conveyor5->run();
		m_conveyor6->run();
		m_conveyor7->run();
		m_conveyor8->run();
		m_conveyorSynchronizer->run();

		m_actuatorSet->drawLabels();
		m_actuatorSet->run();

		m_pakke->runStroFjerner();

	}

	void Keyboard(unsigned char key){
		Test::Keyboard(key);
		m_commandSequenceInterpreter->interpret(key);

		if(key=='1'){
			m_conveyor1->play();
		}else if(key=='q'){
			m_conveyor1->pause();
		}

		if(key=='2'){
			m_conveyor2->play();
		}else if(key=='w'){
			m_conveyor2->pause();
		}
	}

	static Test* Create()
	{
		return new BaneTest;
	}

private:
	Conveyor* m_conveyor1;
	Conveyor* m_conveyor2;
	Conveyor* m_conveyor3;
	Conveyor* m_conveyor4;
	Conveyor* m_conveyor5;
	Conveyor* m_conveyor6;
	Conveyor* m_conveyor7;
	Conveyor* m_conveyor8;
	Wheel* m_wheel1;
	Wheel* m_wheel2;
	ConveyorSynchronizer* m_conveyorSynchronizer;
	CommandSequenceInterpreter* m_commandSequenceInterpreter;
	ActuatorSet* m_actuatorSet;
	Pakke* m_pakke;
};

#endif /* BANETEST_H_ */
