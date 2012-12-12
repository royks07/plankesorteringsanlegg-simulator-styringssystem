/** \file ConveyorSynchronizer.h
 * \brief Contains the class which synchronize conveyors.
 *
 * \author Roy Kollen Svendsen
 */

#ifndef CONVEYORSYNCHRONIZER_H_
#define CONVEYORSYNCHRONIZER_H_

#include <vector>
#include "Conveyor.h"

/** \brief Synchronize Conveyors.
 */
class ConveyorSynchronizer{
public:
	ConveyorSynchronizer();
	void add(Conveyor* conveyor);/**< \brief Add as many conveyers as you'd like.*/
	void run();/**< \brief Place this function in a loop to run the synchronization.*/
				/**< (E.g in the SimulatorPage Step(...)-block)*/

	typedef enum{st1,st2} states_type;
	Conveyor* m_conveyor1;
	Conveyor* m_conveyor2;
	states_type m_state;
	bool initialized;
	vector<Conveyor*> m_conveyor;
	int m_numberOfConveyors;
};

#endif /* CONVEYORSYNCHRONIZER_H_ */
