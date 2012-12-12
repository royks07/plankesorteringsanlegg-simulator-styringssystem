/** \file UserData.h
 * \brief Contains the UserData class-hierarchy.
 *
 * \author Roy Kollen Svendsen
 */
#ifndef USERDATA_H_
#define USERDATA_H_

#include <Box2D/Box2D.h>

typedef enum {e_unknown,e_plank,e_medbringer,e_storageLift} t_userDataType;

/** \brief The archtypical userdata-class which other userdata-classes inherit from.
 *
 *  These common features makes handling user-data much simpler.
 */
class UserData{
public:
	UserData(){
		m_type=e_unknown;
	}
	t_userDataType m_type;
};

/** \brief Carry information about planks.
 *
 * Each planks b2Fixture points to an instance of this class. Contains
 * information about a planks objective length and planks objective quality.
 */
class PlankUserData : UserData{
public:
	PlankUserData(float32 length,float32 quality) : UserData(){
		m_length=length;
		m_quality=quality;
		m_type=e_plank;

	}
	float32 m_length; //between 30 and 40;
	float32 m_quality;//between 0 and 1;
};

/** \brief Identify a b2Fixture as a "medbringer".
 *
 *  Each "medbringers" b2Fixture points to an instance of this class.
 */
class MedbringerkUserData : UserData{
public:
	MedbringerkUserData() : UserData(){
		m_type=e_medbringer;

	}
};

/** \brief Identify a b2Fixture as a storage-lift.
 *
 * Each storage-lifts b2Fixture points to an instance of this class.
 */
class StorageLiftUserData : UserData{
public:
	StorageLiftUserData() : UserData(){
		m_type=e_storageLift;

	}
};

#endif /* USERDATA_H_ */
