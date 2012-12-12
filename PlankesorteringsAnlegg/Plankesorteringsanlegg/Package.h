/** \file Package.h
 * \brief Contains the classes related to planks, "sprinkles" and packages.
 *
 * \author Roy Kollen Svendsen
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include "UserData.h"

#include <Box2D/Box2D.h>
#include <iostream>
#include <map>

using namespace std;

/** \brief Represent "Sprinkle"-objects(In norwegian: strø).
 *
 * \image html sprinkle.png
 *
 */
class Sprinkle{
public:
	/**
	 *
	 * @param pos The position at which you want place the center of the sprinkle.
	 * @param world Pointer to the world in which you want to create the sprinkle.
	 */
	Sprinkle(b2Vec2 pos,b2World* world);
	~Sprinkle();/**< \brief Deallocate the Sprinkle-object and remove its body from the world.*/
	inline float32 getWidth(){	return m_width;	}
	inline float32 getThickness(){return m_thickness;}
	/** \brief Remove Sprinkle-instance when center-point is outside circular-boundary.
	 *
	 * You need to call this function repeatedly.
	 *
	 * @param senter Center of the circular boundary.
	 * @param avstand Remove Sprinkle from world when distance from senter is grater than this number.
	 */
	void removeWhenOutOfBounds(b2Vec2 senter,float32 avstand);

private:
	b2BodyDef* m_bd;
	b2FixtureDef* m_fd;
	b2World* m_world;
	float32 m_width;
	float32 m_thickness;
	b2Body* m_body;
	bool m_bHasBody;
	b2Fixture* m_fixture;
};

/** \brief Represent Plank-objects.
 *
 * \image html plank.png
 *
 */
class Plank{
public:
	/**
	 *
	 * @param pos The position at which you want place the center of the plank.
	 * @param world Pointer to the world in which you want to create the plank.
	 */
	Plank(b2Vec2 pos,b2World* world);
	~Plank();
	/** \brief Generate random number in interval [a,b].
	 *
	 * Helper-function used to give planks random length and quality.
	 *
	 * @param a Start of interval
	 * @param b End of interval
	 * @return a random number from the interval [a,b]
	 */
	float randomFloat(float a, float b);
	inline float32 getWidth(){return m_width;	}
	inline float32 getThickness(){return m_thickness;}
private:
	b2BodyDef* m_bd;
	b2FixtureDef* m_fd;
	b2World* m_world;
	float32 m_width;
	float32 m_thickness;
	b2Body* m_body;
};

/** \brief Represent Package-objects.
 *
 * \image html package.png
 *
 */
class Package{
public:
	/** Create a package wherever you want.
	 *
	 * @param position Lower left corner of package.
	 * @param world The world in which you want to create the package.
	 */
	Package(b2Vec2 position, b2World* world);
	~Package();
	inline float32 getPlankThickness(){return m_plankThickness;}
	inline float32 getSprinkleThickness(){return m_sprinkleThickness;}
	/** \brief Remove sprinkles when outside a predefined circular boundary.
	 *
	 * Needs to be called continuously.
	 */
	void runSprinkleRemover();

	float32 m_plankThickness;
	float32 m_sprinkleThickness;
	Sprinkle* m_sprinkle[7];
	Plank* m_plank[32];
};

/** \brief Lets you create and destruct sprinkles by id.
 *
 * This source can be controlled by a SprinkleSourceActuator.
 */
class SprinkleSource{
public:
	/** \brief Turn any b2body into an abundant source of fresh sprinkles.
	 *
	 * @param body Pointer to a body
	 * @param world Pointer to the world in which you want to create the SprinleSource
	 *
	 * \todo Remove world parameter. Not needed.
	 */
	SprinkleSource(b2Body* body, b2World* world);
	void create(int id);/**< \brief Force the source to create a new Sprinkle-instance "id".*/
	void destruct(int id);/**< \brief Force the source to remove Sprinkle-instance "id".*/
	void updatePosition();/**< \brief Call continuously to update position of source.*/
	inline b2Vec2 getPosition(){return m_position;}/**< \brief Return current position of source.*/

	multimap<int,Sprinkle*> m_sprinkle;
	b2Vec2 m_position;
	b2Body* m_body;
	b2World* m_world;
};

/** \brief Lets you create and destruct packages by id.
 *
 * This source can be controlled by a PackageSourceActuator.
 */
class PackageSource{
public:
	/** \brief Place an an abundant source of fresh packages at whatever position you'd like.
	 *
	 * @param position Position of package lower left corner.
	 * @param world Pointer to the world in which you want to create the PackageSource
	 */
	PackageSource(b2Vec2 position, b2World* world);
	void create(int id);/**< \brief Force the source to create a new Package-instance "id".*/
	void destruct(int id);/**< \brief Force the source to remove Package-instance "id".*/
	inline b2Vec2 getPosition(){return m_position;}
	/** \brief Remove sprinkles when outside a predefined circular boundary.
	 *
	 * Needs to be called continuously.
	 */
	void runSprinkleRemover();

	multimap<int,Package*> m_package;
	b2Vec2 m_position;
	b2World* m_world;
};

#endif /* PACKAGE_H_ */
