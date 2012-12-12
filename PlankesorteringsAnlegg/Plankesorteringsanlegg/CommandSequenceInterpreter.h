/** \file CommandSequenceInterpreter.h
 * \brief Contains the CommandSequanceInterpreter-class.
 *
 * \author Roy Kollen Svendsen
 */

#ifndef COMMANDSEQUENCEINTERPRETER_H_
#define COMMANDSEQUENCEINTERPRETER_H_

#include <string>
#include <sstream>
#include <vector>

#include "Actuators.h"
#include "Sensors.h"


/** \brief Use keyboard- or terminal-input to control the actuator- and sensor-sets.
 */
class CommandSequenceInterpreter{
public:
	CommandSequenceInterpreter(ActuatorSet* actuatorSet,SensorSet* sensorSet);
	void interpret(unsigned char key);

	typedef enum {st_wait_for_new_command,st_read_parameter} t_readState;
	typedef enum {e_writeActuator,e_readSensor} t_commandType;
	t_commandType m_commandType;
	t_readState m_readState;
	stringstream m_ss;
	vector<string> m_parameter;
	ActuatorSet* m_actuatorSet;
	SensorSet* m_sensorSet;
};

#endif /* COMMANDSEQUENCEINTERPRETER_H_ */
