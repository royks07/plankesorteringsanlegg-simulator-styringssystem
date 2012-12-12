/*
 * CommandSequenceInterpreter.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "CommandSequenceInterpreter.h"

CommandSequenceInterpreter::CommandSequenceInterpreter(ActuatorSet* actuatorSet,SensorSet* sensorSet){
	m_actuatorSet=actuatorSet;
	m_sensorSet=sensorSet;
	m_readState=st_wait_for_new_command;
}

void CommandSequenceInterpreter::interpret(unsigned char key){
	switch(m_readState){
	case st_wait_for_new_command:
		if(key=='a'){
			m_commandType=e_writeActuator;
			m_parameter.clear();
			m_ss.str("");
			cout<<"interpreter in actuator-state..."<<endl;
			m_readState=st_read_parameter;
		}else if (key=='s'){
			m_commandType=e_readSensor;
			m_parameter.clear();
			m_ss.str("");
			cout<<"interpreter in sensor-state..."<<endl;
			m_readState=st_read_parameter;
		}else{
		}
		break;
	case st_read_parameter:
		if(key==','){
			cout<<"finished reading parameter..."<<endl;
			m_readState=st_read_parameter;
			cout<<m_ss.str()<<endl;
			m_parameter.push_back(m_ss.str());
			m_ss.str("");
		//}else if(key=='.' && m_parameter.size()<=1){
			//do not end - need two parameters
		}else if(key=='.'){
			cout<<"finished reading command."<<endl;
			m_readState=st_wait_for_new_command;
			cout<<m_ss.str()<<endl;
			m_parameter.push_back(m_ss.str());
			m_ss.str("");
			//execute command:
			if(m_commandType==e_writeActuator){
				int id;
				int value;
				try {
					 id = boost::lexical_cast<int>(m_parameter.at(0).c_str());
					 value = boost::lexical_cast<int>(m_parameter.at(1).c_str());

				} catch( boost::bad_lexical_cast const& ) {
					std::cout << "Error: parameter was not valid" << std::endl;
				}
				m_actuatorSet->writeActuator(id,value);
				cout<<"Writing "<<value<<" to a"<<id<<endl;
			}else if(m_commandType==e_readSensor){
				int id;
				try {
					 id = boost::lexical_cast<int>(m_parameter.at(0).c_str());
				} catch( boost::bad_lexical_cast const& ) {
					std::cout << "Error: parameter was not valid" << std::endl;
				}
				signed int value=m_sensorSet->readSensor(id);
				cout<<"Value of S"<<id<<" is "<<value<<endl;

			}
		}else{
			cout<<"reading parameter..."<<endl;
			m_ss<<key;
		}
		break;
	}
}



