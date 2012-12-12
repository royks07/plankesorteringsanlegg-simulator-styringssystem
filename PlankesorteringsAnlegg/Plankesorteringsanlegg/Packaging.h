/** \file Packaging.h
 * \brief Contains the PackageInput- and PackageOutput-class.
 *
 * \author Roy Kollen Svendsen
 */
#ifndef PACKAGING_H_
#define PACKAGING_H_

#include <Box2D/Box2D.h>

#include "Package.h"
#include "UserData.h"

/** \brief Represent the package input block.
 * \image html packageinput.png
 */
class PackageInput{
public:
	/** Construct a PackageInput-instance at any position.
	 *
	 * @param p Pivot-point of PackageInput.
	 * @param lengthLyingBeam Length of the lying beam.
	 * @param thicknessLyingBeam Thickness of the lying beam
	 * @param heightStandingBeam Height of the standing beam.
	 * @param thicknessStandingBeam Thickness of the standing beam.
	 * @param world Pointer to the world i which you want to create the PackageInput-instance.
	 */
	PackageInput(b2Vec2 p,float32 lengthLyingBeam,float32 thicknessLyingBeam,float32 heightStandingBeam,float32 thicknessStandingBeam,b2World* world);
	inline b2PrismaticJoint* getLiftJoint(){return m_jointLift;}/**< \brief Return pointer to the LiftJoint.*/
	inline b2RevoluteJoint* getRotateJoint(){	return m_jointRotate;}/**< \brief Return pointer to RotateJoint.*/
	inline b2Fixture* getLiftSensorFixture(){return m_fixtureLiftSensor;}/**< \brief Return pointer to the Lift-sensor Fixture.*/
	inline PackageSource* getPackageSource(){return m_packageSource;}/**< \brief Return a pointer to the integrated PackageSource.*/

	b2PrismaticJoint* m_jointLift;
	b2RevoluteJoint* m_jointRotate;
	b2Fixture* m_fixtureLiftSensor;
	PackageSource* m_packageSource;
};

/** \brief Represent the package output block.
 * \image html packageoutput.png
 */
class PackageOutput{
public:
	/** \brief Create PackageOutput-instance with center of lyingbeam at any position.
	 *
	 * @param position Position of LyingBeams center.
	 * @param world Pointer to the world in which you want to create the PackageOutput.
	 */
	PackageOutput(b2Vec2 position,b2World* world);
	inline b2PrismaticJoint* getForkJoint(){return m_jointFork;}/**< \brief Return pointer to ForkJoint.*/
	inline b2PrismaticJoint* getForkBaseJoint(){return m_jointForkBase;}/**< \brief Return pointer to ForkBaseJoint.*/
	inline b2PrismaticJoint* getLiftJoint(){return m_jointLift;}/**< \brief Return pointer to LiftJoint.*/
	inline b2PrismaticJoint* getCassetteJoint(){return m_jointCassette;}/**< \brief Return pointer to CassetteJoint.*/
	inline b2Fixture* getCassetteSensorFixture(){return m_fixtureCassetteSensor;}/**< \brief Return pointer to CassetteSensorFixture.*/
	inline SprinkleSource* getSprinkleSource(){return m_sprinkleSource;}/**< \brief Return pointer to SprinkleSource.*/

	b2PrismaticJoint* m_jointForkBase;
	b2PrismaticJoint* m_jointFork;
	b2PrismaticJoint* m_jointLift;
	b2PrismaticJoint* m_jointCassette;
	b2PrismaticJoint* m_jointCassetteSensor;
	b2Fixture* m_fixtureCassetteSensor;
	SprinkleSource* m_sprinkleSource;
};

#endif /* PACKAGING_H_ */
