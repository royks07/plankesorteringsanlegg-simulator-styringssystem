/** \file Conveyor.h
 * \brief Contains the Conveyor-class and the classes needed to build it.
 *
 * \author Roy Kollen Svendsen
 */

#ifndef CONVEYOR_H_
#define CONVEYOR_H_

#include <vector>
#include "Sensors.h"

typedef enum{ingen,knott,krok} t_medbringerType;

/** \brief The conveyors wheels.
 */
class Wheel{
public:
	Wheel(b2Vec2 p,float32 r,int cmBits,b2World* world);
	inline void setSpeed(float32 speed){m_speed=speed;}
	void run();
	inline b2Body* getBody(){return m_body;}
private:
	b2Body* m_body;
	float32 m_speed;
};

/** \brief The beam which the wheels are attached to and which the belt runs along.
 */
class Beam{
public:
	Beam(b2Vec2 p1,b2Vec2 p2,float32 width,int cmBits,b2World* world);
	inline b2Vec2& getLocalAnchor1(){return m_localAnchor1;}
	inline b2Vec2& getLocalAnchor2(){	return m_localAnchor2;}
	inline b2Body* getBody(){return m_body;}
private:
	b2Body* m_body;
	b2Vec2 m_localAnchor1;
	b2Vec2 m_localAnchor2;
};

/** \brief The conveyorbelt
 */
class ConveyorBelt{
public:
	/** \brief Create the conveyorbelt.
	 * @param p1 Center position of first wheel.
	 * @param p2 Center position of second wheel.
	 * @param beamThickness Thickness of the beam.
	 * @param beltThickness Thickness of the belt.
	 * @param N Number of links.
	 * @param M Number of links between every "medbringer"
	 * @param medbringerType The type of "medbringers" which are connected to the belt.
	 * @param sensorPos The number of the link which the sensor is placed at.
	 * @param sensorAdjustment
	 * @param cmBits Category- and mask-bits.
	 * @param world Pointer to the world in which you want to create a conveyorbelt.
	 */
	ConveyorBelt(b2Vec2 p1,b2Vec2 p2,float32 beamThickness,float32 beltThickness,int N,int M,t_medbringerType medbringerType,int sensorPos,float32 sensorAdjustment,int cmBits,b2World* world);
	float32 getAngleAt(b2Vec2 p);/**< Get the angle of the nearest belt-tangent*/
	void pause();/**< Pause the belt*/
	void play();/**< Restart the belt in the same state as when paused.*/
	inline ContactSensor* getSensor(){return m_contactSensorCircular;	}/**< Return the synchronization sensor*/

	b2Vec2 m_p1;
	b2Vec2 m_p2;
	vector<b2Body*> link;
	vector<b2Vec2> linkSpeed;/**< List of all the links velocity- vectors*/
	bool m_paused;
	ContactSensor_Sync* m_contactSensorCircular;
	t_medbringerType m_medbringerType;
};

/** \brief Hinder the conveyorbelt in falling off.
 */
class ConveyorBeltShield{
public:
	ConveyorBeltShield(b2Vec2 p1,b2Vec2 p2,float32 beamThickness,float32 beltThickness,int cmBits,b2World* world);

	b2Vec2 m_p1;
	b2Vec2 m_p2;
	float32 m_beamThickness;
	float32 m_beltThickness;
};

/** \brief Assemble the conveyor and expose an interface through which controlling the conveyor is possible.
 */
class Conveyor{
public:
	Conveyor(b2Vec2 p1, b2Vec2 p2, float32 beamThickness,float32 beltThickness,int N,int M,t_medbringerType medbringerType,int sensorPos,float32 sensorAdjustment,int cmBits, b2World* world);
	void setSpeed(float32 speed);/**< \brief Set the speed of the conveyorbelt.*/
	void pause();/**< \brief Pause the conveyor*/
	void play();/**< \brief Run the conveyor*/
	inline bool isPaused(){return m_bPaused;}/**< \brief Check if conveyor is paused.*/
	void run();/**< \brief You need to call this function iteratively.*/
	inline Wheel* getWheel1() const {	return m_wheel1;}
	inline Wheel* getWheel2() const {	return m_wheel2;}
	ContactSensor* getSensor();/**< \brief Return the synchronization-sensor.*/
	bool isSensorActivated();/**< \brief Check state of synchronization-sensor*/

	Wheel* m_wheel1;
	Wheel* m_wheel2;
	b2Vec2 m_p1;
	b2Vec2 m_p2;
	float32 m_beltThickness;
	float32 m_beamThickness;
	float32 m_speed;
	ConveyorBelt* m_conveyorBelt;
	ConveyorBeltShield* m_conveyorBeltShield;
	bool m_bPaused;
};

#endif /* CONVEYOR_H_ */
