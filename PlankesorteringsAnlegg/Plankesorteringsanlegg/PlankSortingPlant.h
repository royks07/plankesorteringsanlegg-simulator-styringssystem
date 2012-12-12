/**	\file PlankSortingPlant.h
 *	\brief Contains the SimulationPage to create a "plank sorting plant".
 *
 *	\author Roy Kollen Svendsen
 */

#ifndef PLANKSORTINGPLANT_H_
#define PLANKSORTINGPLANT_H_

#include "Conveyor.h"
#include "ConveyorSynchronizer.h"
#include "Storages.h"
#include "Actuators.h"
#include "Sensors.h"
#include "Packaging.h"
#include "Communicator.h"
#include "CommandSequenceInterpreter.h"
#include "SimulatorPage.h"

/** \brief Extend the default SimulatorPage to create a simulation of a plank sorting plant.
 *
 *	A snapshot of the plant:
 *	\image html plant.png
 *
 *	You are welcome to add your own SimulatorPages in the file SimulatorPageEntries.cpp in this maner:
 *	\code
 *	SimulatorPageEntry g_simulatorPageEntries[] =
 *	{
 *		{"Plank sorting plant", PlankSortingPlant::Create},
 *		{NULL, NULL}
 *	}
 *	\endcode
 */

class PlankSortingPlant : public SimulatorPage{
public:
	PlankSortingPlant(Communicator* communicator);
	void Step(Settings *settings);
	void Keyboard(unsigned char key);
	static SimulatorPage* Create(Communicator* communicator);

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
	SensorSet*  m_sensorSet;
};

#endif /* PLANKSORTINGPLANT_H_ */
