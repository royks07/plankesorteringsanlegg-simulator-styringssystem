/** \file SimulatorPageEntries.cpp
 * \brief Assign simulator-page entries to global array g_simulatorPageEntries
 *
 * To add an entry for ExampleSimulatorPage add a line like this:
 * \code
 *	SimulatorPageEntry g_simulatorPageEntries[] =
 *	{
 *		{"Plank sorting plant", PlankSortingPlant::Create},
 *		{"Example simulator-page", ExampleSimulatorPage::Create},
 *		{NULL, NULL}
 *	};
 * \endcode
 * The entry will show up in the Simulator-page list:
 * \image html simulator_page.png
 *
 * \author Roy Kollen Svendsen
 *
 * This file is based on Erin Cattos Box2D 2.2.1 Testbed TestEntries.cpp
 */

#include "PlankSortingPlant.h"

SimulatorPageEntry g_simulatorPageEntries[] =
{
	{"Plank sorting plant", PlankSortingPlant::Create},
	{NULL, NULL}
};
