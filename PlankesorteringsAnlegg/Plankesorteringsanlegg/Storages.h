/** \file Storages.h
 *	\brief Define classes to create individual storages and storage area.
 *
 *	\author Roy Kollen Svendsen
 */

#ifndef STORAGES_H_
#define STORAGES_H_

#include <Box2D/Box2D.h>
#include <vector>
#include "UserData.h"

using namespace std;

/** \brief Create an individual storage to sort planks into.
 *
 *	The Following line of code generates an instance of the Storage-class:
 *	\code
 *	Storage* storage = new Storage(b2Vec2(-40.0f,0.0f),9,5,0.5f,m_world);
 *	\endcode
 *
 *	Storage-instance + descriptive text:
 *	\image html storage1.png
 */
class Storage{
public:
	/** \brief Construct a Storage.
	 *
	 * @param upperLeftCorner The upper left corner of the storage.
	 * @param width The width of the storage.
	 * @param height The height of the storage.
	 * @param wallThickness The thickness of the storages walls.
	 * @param world Pointer to world you want to create the storage in.
	 */
	Storage(b2Vec2 upperLeftCorner,float32 width,float32 height, float32 wallThickness,b2World* world);
	inline b2PrismaticJoint* getLiftJoint(){return m_jointLift;}/**< \brief Return the liftJoint.*/
	inline b2PrismaticJoint* getStopperJoint(){return m_jointStopper;}/**< \brief Return the stopperJoint.*/
	inline b2Fixture* getTopSensorFixture(){return m_fixtureTopSensor;}/**< \brief Return the topSensorFixture.*/
	inline b2Fixture* getMiddleSensorFixture(){return m_fixtureMiddleSensor;}/**< \brief Return the middleSensorFixture.*/
	inline b2Fixture* getBottomSensorFixture(){return m_fixtureBottomSensor;}/**< \brief Return the bottomSensorFixture.*/

	b2Body* m_bodyStopper;
	b2PrismaticJoint* m_jointLift;
	b2PrismaticJoint* m_jointStopper;
	b2Fixture* m_fixtureTopSensor;
	b2Fixture* m_fixtureMiddleSensor;
	b2Fixture* m_fixtureBottomSensor;
};

/** \brief Create an arbitrary number of storages side-by-side to sort planks into.
 *
 *	The Following line of code generates the storage area in the image below.
 *	\code
 *	StorageArea* storageArea0 = new StorageArea(3,b2Vec2(-40.0f,0.0f),9,5,0.5f,m_world);
 *	\endcode
 *
 *	Instance of StorageArea + descriptive text:
 *	\image html storagearea.png
 */
class StorageArea{
public:
	/** \brief Construct an arbitrary number of storages side-by-side.
	 *
	 * @param numberOfStorages The number of storages.
	 * @param upperLeftCorner Positioin of the upper left corner of the first storage.
	 * @param width The width of an individual storage.
	 * @param height The height of an individual storage.
	 * @param wallThickness The wall-thickness of an individual storage.
	 * @param world Pointer to the world you want to create the storage-area in.
	 */
	StorageArea(int numberOfStorages,b2Vec2 upperLeftCorner,float32 width,float32 height, float32 wallThickness,b2World* world);
	b2PrismaticJoint* getLiftJoint(int i);
	b2PrismaticJoint* getStopperJoint(int i);
	b2Fixture* getTopSensorFixture(int i);
	b2Fixture* getMiddleSensorFixture(int i);
	b2Fixture* getBottomSensorFixture(int i);

	vector<Storage*> m_storage;/**< \brief Vector that contains pointers to the individual storages.*/
};

#endif /* STORAGES_H_ */
