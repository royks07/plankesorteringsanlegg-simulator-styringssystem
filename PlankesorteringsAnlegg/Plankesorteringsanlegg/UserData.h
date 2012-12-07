/*
 * UserData.h
 *
 *  Created on: 3. des. 2012
 *      Author: roy
 */

#ifndef USERDATA_H_
#define USERDATA_H_

typedef enum {e_unknown,e_plank,e_medbringer,e_storageLift} t_userDataType;

class UserData{
public:
	UserData(){
		m_type=e_unknown;
	}
	t_userDataType m_type;
};

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

class MedbringerkUserData : UserData{
public:
	MedbringerkUserData() : UserData(){
		m_type=e_medbringer;

	}
};

class StorageLiftUserData : UserData{
public:
	StorageLiftUserData() : UserData(){
		m_type=e_storageLift;

	}
};



#endif /* USERDATA_H_ */
