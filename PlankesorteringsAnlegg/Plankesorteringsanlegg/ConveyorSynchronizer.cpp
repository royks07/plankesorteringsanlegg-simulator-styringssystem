/*
 * ConveyorSynchronizer.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "ConveyorSynchronizer.h"

ConveyorSynchronizer::ConveyorSynchronizer(){
	m_state=st1;
	initialized=false;
	m_numberOfConveyors=0;
}
void ConveyorSynchronizer::add(Conveyor* conveyor){
	m_conveyor.push_back(conveyor);
	m_numberOfConveyors++;
}

void ConveyorSynchronizer::run(){
	if(!initialized){
		initialized=true;
		//m_conveyor1->play();
		//m_conveyor2->play();
	}
	//state-machine:

	vector<Conveyor*>::iterator it;
	int i;

	switch(m_state){
	case st1:	//wait until all sensors activated
		i=0;
		for(it=m_conveyor.begin();it!=m_conveyor.end();it++){
			if((*it)->isSensorActivated()){
				(*it)->pause();
				i++;
				if(i==m_numberOfConveyors){
					for(int j=0;j<m_numberOfConveyors;j++) m_conveyor.at(j)->play();
					m_state=st2;
				}
			}else{
				(*it)->play();
			}
		}
		break;
	case st2:	//wait until all sensors deactivated
		i=0;
		for(it=m_conveyor.begin();it!=m_conveyor.end();it++){
			(*it)->play();
			if(!(*it)->isSensorActivated()){
				i++;
				if(i==m_numberOfConveyors){
					m_state=st1;
				}
			}
		}
		break;
	}
}

