/** \file Sensors.h
 * \brief Contains the sensor class-hierarchy and the sensor-set.
 *
 * \author Roy Kollen Svendsen
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <string>
#include <vector>

#include "Text.h"
#include "UserData.h"
#include "Communicator.h"

typedef enum{e_unknownSensor,e_normal,e_sync,e_lengthSensor,e_qualitySensor,e_counterSensor} t_sensorType;

/** \brief Abstract class that other sensors inherit from.
 */
class ContactSensor{
public:
	ContactSensor();
	virtual ~ContactSensor(){	}
	virtual int getId(){return m_id;}
	virtual string getName(){return m_name;}
	virtual void drawLabel(){};
	virtual signed int get(){	return 0;};
	virtual t_sensorType getType(){return m_sensorType;}

	int32 m_id;
	b2Color m_color;
	string m_name;
	b2Vec2 m_position;
	float32 m_radius;
	b2World* m_world;
	b2Body* m_body;
	b2Fixture* m_fixture;
	bool m_activated;
	b2BodyDef m_bd;
	b2FixtureDef m_fd;
	t_sensorType m_sensorType;
};

/** \brief Turn arbitrary Box2D fixture into binary sensor.
 *
 * \b Example:
 * \image html s1.png
 * To create S1 and activate the sensor, by adding it to the sensor-set, we write the following lines of code:
 * \code
 *	ContactSensor_FromFixture* liftSensor = new ContactSensor_FromFixture(1,packageInput->getLiftSensorFixture());
 *	m_sensorSet->add(liftSensor);
 * \endcode
 *
 */
class ContactSensor_FromFixture : public ContactSensor{
public:
	/** \brief Turn arbitrary b2Fixture into binary sensor.
	 * @param id The id of the sensor. This value will be visible as part of the sensor label.
	 * @param fixture Pointer to the b2Fixture that you want to turn in to a sensor.
	 */
	ContactSensor_FromFixture(int id,b2Fixture* fixture);
	signed int get();/**< \brief Return sensors measured value.*/
	void drawLabel();/**< \brief Draw sensor label.*/

	float32 m_radius;
};

/** \brief Used by the ConveyorSynchronizer.
 *
 * Each Conveyor has one of these sensors. If a Conveyor is registered with a ConveyorSynchronizer
 * a label with the text \b sync will show up next to the sensor:
 *
 * \image html sync.png
 */
class ContactSensor_Sync : public ContactSensor{
public:
	/** \brief Constructs a synchronization-sensor.
	 *
	 * @param position The position of the sensors circular shaped fixture.
	 * @param radius The radius of the sensors fixture.
	 * @param filter The sensors filter is used to control what sensors collide with she sensors fixture.
	 * @param world Pointer to the world in which you want to create the sensor.
	 */
	ContactSensor_Sync(b2Vec2 position,float32 radius, b2Filter filter,b2World* world);
	virtual void drawLabel();/**< \brief Draw "sync" in red next to the sensor.*/

	float32 m_radius;
};

/** \brief This sensor is circular shaped and has only two states, ON and OFf.
 */
class ContactSensorBinary : public ContactSensor{
public:
	ContactSensorBinary(int id,b2Vec2 position,float32 radius,b2World* world);
	virtual void drawLabel();
	virtual signed int get();/**< \brief Return the state of the sensor.*/

	float32 m_radius;
};

/** \brief Measure length of a colliding Plank-instance.
 */
class LengthSensor : public ContactSensorBinary{
public:
	LengthSensor(int id,b2Vec2 position,float32 radius,b2World* world);
	void meassureLength(float32 length);
	inline signed int get(){return m_meassuredLenght;}/**< \brief Return the state of the sensor.*/

	signed int m_meassuredLenght;/**< \brief Store the last measured plank length*/
};

/** \brief Measure quality of a colliding Plank-instance.
 */
class QualitySensor : public ContactSensorBinary{
public:
	QualitySensor(int id,b2Vec2 position,float32 radius,b2World* world);

	void meassureQuality(float32 quality);
	inline signed int get(){return m_meassuredQuality;}/**< \brief Return the state of the sensor.*/

	signed int m_meassuredQuality;/**< \brief Store the last measured plank quality*/
};

/** \brief Count the number of collisions.
 *
 * This is used to measure how far a the conveyor-belt has traveled.
 * \image html countsensor.png
 */
class CounterSensor : public ContactSensor_Sync{
public:
	CounterSensor(int id,b2Vec2 position,float32 radius,b2Filter filter,b2World* world);
	void updateCounter();/**< \brief Increase counter by one unit.*/
	void drawLabel();/**< \brief Draw sensor label.*/
	inline signed int get(){return m_counter;}/**< \brief Return the state of the sensor.*/

	signed int m_counter;/**< \brief Stores the number of registered collisions.*/
};

/** \brief Listen to sensors and transmit state-changes to the communicator.
 */
class SensorSet : public b2ContactListener{
public:
	SensorSet(Communicator* communicator);
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void drawLabels();/**< Draw all sensor labels*/
	signed int readSensor(signed int id);/**< \brief Read sensor state on demand.*/
	void add(ContactSensor* contactSensor);/**< \brief Add sensor to sensor-set.*/

	vector<ContactSensor*> m_contactSensorVector;
	Communicator* m_communicator;/**< \brief The communicator with whom SensorSet speaks.*/
};

#endif /* SENSORS_H_ */
