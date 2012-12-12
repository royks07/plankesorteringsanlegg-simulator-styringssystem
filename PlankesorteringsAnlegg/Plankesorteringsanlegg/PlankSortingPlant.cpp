/*
 * PlankSortingPlant.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "PlankSortingPlant.h"

PlankSortingPlant::PlankSortingPlant(Communicator* communicator):SimulatorPage(communicator){

	//ACTUATOR SET
	m_actuatorSet=new ActuatorSet;

	//SENSOR SET
	m_sensorSet = new SensorSet(m_communicator);


	//STORAGE AREA
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

	for(int i=10;i<=19;i++){
		//add upper sensor to sensorset
		ContactSensor_FromFixture* upperSensor = new ContactSensor_FromFixture(i,storageArea->getTopSensorFixture(i-10));
		m_sensorSet->add(upperSensor);
	}
	for(int i=20;i<=29;i++){
		//add middle sensor to sensorset
		ContactSensor_FromFixture* middleSensor = new ContactSensor_FromFixture(i,storageArea->getMiddleSensorFixture(i-20));
		m_sensorSet->add(middleSensor);
	}
	for(int i=30;i<=39;i++){
		//add lower sensor to sensorset
		ContactSensor_FromFixture* lowerSensor = new ContactSensor_FromFixture(i,storageArea->getBottomSensorFixture(i-30));
		m_sensorSet->add(lowerSensor);
	}


	m_commandSequenceInterpreter=new CommandSequenceInterpreter(m_actuatorSet,m_sensorSet);


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
	m_conveyor8->setSpeed(0.0f);


	//CONVEYOR2-SENSORS
	LengthSensor* lengthSensor=new LengthSensor(3,p3+b2Vec2(10.0f,1.7f),0.3f,m_world);
	m_sensorSet->add(lengthSensor);

	QualitySensor* qualitySensor=new QualitySensor(4,p3+b2Vec2(10.0f,1.7f),0.3f,m_world);
	m_sensorSet->add(qualitySensor);

	b2Filter filter;//have to sett filter to get sensor to collide with medbringere
	filter.categoryBits=2;
	filter.maskBits=2;
	CounterSensor* counterSensor=new CounterSensor(50,p4+b2Vec2(-12.0f,-1.6f),0.3f,filter,m_world);
	m_sensorSet->add(counterSensor);


	//CONVEYOR5-SENSOR
	ContactSensorBinary* conveyor5Sensor=new ContactSensorBinary(6,p9+b2Vec2(3.0f,1.7f),0.3f,m_world);
	m_sensorSet->add(conveyor5Sensor);



	//SLIDE1
	b2Vec2 p17(16.7f,25.0f);
	//b2Vec2 p18(22.3f,19.4f);
	Beam(p17,p1,2.0f,4,m_world);

	ContactSensorBinary* tippedSensor=new ContactSensorBinary(51,p17+b2Vec2(0,0),0.3f,m_world);
	m_sensorSet->add(tippedSensor);


	ContactSensorBinary* slide1Sensor=new ContactSensorBinary(2,p1+b2Vec2(0,2.3f),0.3f,m_world);
	m_sensorSet->add(slide1Sensor);




	//SLIDE2
	/*b2Vec2 p19(9.2f,9.1f);
	b2Vec2 p20(16.0f,9.1f);*/
	Beam(p9,p12,2.0f,4,m_world);

	ContactSensorBinary* slide2Sensor=new ContactSensorBinary(7,p12+b2Vec2(0.3f,2.2f),0.3f,m_world);
	m_sensorSet->add(slide2Sensor);


	//PACKAGE-INPUT
	PackageInput* packageInput=new PackageInput(b2Vec2(11.9f,20.6f),8.6f,1.17f,7.0f,0.37f,m_world);

	JointActuatorPrismaticStep* liftPackageActuator = new JointActuatorPrismaticStep(1,packageInput->getLiftJoint());
	m_actuatorSet->add(liftPackageActuator);

	JointActuatorRevoluteStep* rotatePackageActuator = new JointActuatorRevoluteStep(2,packageInput->getRotateJoint());
	m_actuatorSet->add(rotatePackageActuator);

	PackageSourceActuator* packageSourceActuator = new PackageSourceActuator(55,packageInput->getPackageSource());
	m_actuatorSet->add(packageSourceActuator);



	ContactSensor_FromFixture* liftSensor = new ContactSensor_FromFixture(1,packageInput->getLiftSensorFixture());
	m_sensorSet->add(liftSensor);


	//PACKAGE-OUTPUT
	PackageOutput* packageOutput=new PackageOutput(b2Vec2(9.0f,2.2f),m_world);

	JointActuatorPrismaticStep* liftPackageOutActuator = new JointActuatorPrismaticStep(30,packageOutput->getLiftJoint());
	m_actuatorSet->add(liftPackageOutActuator);


	ContactSensor_FromFixture* cassetteSensor = new ContactSensor_FromFixture(40,packageOutput->getCassetteSensorFixture());
	m_sensorSet->add(cassetteSensor);

	JointActuatorPrismaticStep* cassetteActuator = new JointActuatorPrismaticStep(31,packageOutput->getCassetteJoint());
	m_actuatorSet->add(cassetteActuator);

	JointActuatorPrismaticBinary* forkActuator = new JointActuatorPrismaticBinary(32,packageOutput->getForkJoint());
	forkActuator->setSpeed(1.5f);
	m_actuatorSet->add(forkActuator);

	JointActuatorPrismaticBinary* forkBaseActuator = new JointActuatorPrismaticBinary(33,packageOutput->getForkBaseJoint());
	forkBaseActuator->setSpeed(2.5f);
	m_actuatorSet->add(forkBaseActuator);

	ContactSensorBinary* arriveSensor=new ContactSensorBinary(6,p13+b2Vec2(-0.7f,0.7f),0.3f,m_world);
	m_sensorSet->add(arriveSensor);

	SprinkleSourceActuator* sprinkleSourceActuator = new SprinkleSourceActuator(56,packageOutput->getSprinkleSource());
	m_actuatorSet->add(sprinkleSourceActuator);



	//CONVEYOR-ACTUATORS
	ConveyorActuatorBinary* conveyor5Actuator = new ConveyorActuatorBinary(5,m_conveyor5);
	m_actuatorSet->add(conveyor5Actuator);

	ConveyorActuatorBinary* conveyor6Actuator = new ConveyorActuatorBinary(6,m_conveyor6);
	m_actuatorSet->add(conveyor6Actuator);

	ConveyorActuatorBinary* conveyor7Actuator = new ConveyorActuatorBinary(7,m_conveyor7);
	m_actuatorSet->add(conveyor7Actuator);

	ConveyorActuatorStep* conveyor8Actuator = new ConveyorActuatorStep(8,m_conveyor8);
	m_actuatorSet->add(conveyor8Actuator);


	//CONVEYOR-SENSORS
	m_sensorSet->add(m_conveyor1->getSensor());
	m_sensorSet->add(m_conveyor2->getSensor());
	m_sensorSet->add(m_conveyor4->getSensor());

	m_world->SetContactListener(m_sensorSet);


	//CONVEYOR SYNCHRONIZER
	m_conveyorSynchronizer=new ConveyorSynchronizer();
	m_conveyorSynchronizer->add(m_conveyor1);
	m_conveyorSynchronizer->add(m_conveyor2);
	m_conveyorSynchronizer->add(m_conveyor4);
}

void PlankSortingPlant::Step(Settings *settings){
	SimulatorPage::Step(settings);
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

	m_sensorSet->drawLabels();

	//m_pakke->runStroFjerner();
	if(m_communicator->dataIsPresent()){
			unsigned char c = m_communicator->read();
			//m_communicator.write(c);
			Keyboard(c);
		}
}

void PlankSortingPlant::Keyboard(unsigned char key){
	SimulatorPage::Keyboard(key);
	m_commandSequenceInterpreter->interpret(key);
}

SimulatorPage* PlankSortingPlant::Create(Communicator* communicator)
{
	return new PlankSortingPlant(communicator);
}
