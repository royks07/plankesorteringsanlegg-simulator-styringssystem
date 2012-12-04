/*
 * SensorFlate.h
 *
 *  Created on: 14. nov. 2012
 *      Author: roy
 */

#ifndef SENSORFLATE_H_
#define SENSORFLATE_H_

#include <list>
#include <string>
#include <sstream>
#include <math.h>


#include <boost/dynamic_bitset.hpp>

#include "Kjede.h"


using namespace std;

class SensorFlateTilstand{
public:
	SensorFlateTilstand(){
		m_bitsekvens=boost::dynamic_bitset<>(80);
	}
	void flipBit(int n){
		m_bitsekvens[n]=(m_bitsekvens[n]+1)%2;
	}
	void setBit(int n){
		m_bitsekvens[n]=1;
	}
	void clearBit(int n){
		m_bitsekvens[n]=0;
	}
	void setToValue(int startBit,int endBit,int val){
		for(int i=startBit;i<=endBit;i++){
			m_bitsekvens[i]=val&(1<<(i-startBit));
		}
	}
	unsigned char bitSekvensToChar(int n){
		    unsigned char c=0;
			for(int i=0;i<=8;i++)
				c+=m_bitsekvens[8*n+i]<<i;

			return c;
		}

		void placeCharInBitSekvens(unsigned char c,int n){
			for(int i=0;i<=8;i++)
				m_bitsekvens[8*n+i]=(c&(1<<i));
		}

		void printBitSekvens(){
			string tmpString;
			boost::to_string(m_bitsekvens,tmpString);
			cout<<tmpString<<endl;
		}
		void printBitSekvensAsString(){

			for(int i=0;i<m_bitsekvens.size()/8;i++){
				char c = bitSekvensToChar(i);
				if(c==0) c=' ';
				cout<<c;
			}
			cout<<endl;
		}
		string toString2(){
			stringstream ss("Utskrift av SensorFlateTilstand: ");
			string s;
			for(int i=0;i<m_bitsekvens.size()/8;i++){
				char c = bitSekvensToChar(i);
				if(c==0) c=' ';
				else
					ss<<c;
			}
			s=ss.str();
			return s;
		}
		string toString(){
			stringstream ss;
			string s;
			boost::to_string(m_bitsekvens,s);
			ss<<"SensorFlateTilstand: "<<s;
			return ss.str();
		}

		boost::dynamic_bitset<> getBitsekvens(){
			return m_bitsekvens;
		}

private:
		boost::dynamic_bitset<> m_bitsekvens;

};

class Sensor{
public:
	Sensor(b2Fixture* sensorFixture){
		m_sensorFixture=sensorFixture;
		m_startBit=60;
		m_endBit=60;
		m_name="";
	}
	Sensor(b2Fixture* sensorFixture,string name,int startBit,int endBit){
		m_sensorFixture=sensorFixture;
		m_startBit=startBit;
		m_endBit=endBit;
		m_name=name;
	}
	void getTouchingBody(){

	}
	bool isActivating(b2Contact* contact){
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		bool retVal=false;
		if(fixtureA==m_sensorFixture){
//			cout<<"FixtureA is "<<m_name<<endl;
			retVal=true;
		}
		else if(fixtureB==m_sensorFixture){
//			cout<<"FixtureB is "<<m_name<<endl;
			retVal=true;
		}
		return retVal;
	}

	virtual void runActivationRoutine(){
		setBits();
	}

	bool isDeActivating(b2Contact* contact){
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		bool retVal=false;
		if(fixtureA==m_sensorFixture){
//			cout<<"FixtureA is "<<m_name<<endl;
			retVal=true;
		}
		else if(fixtureB==m_sensorFixture){
//			cout<<"FixtureB is "<<m_name<<endl;
			retVal=true;
		}
		return retVal;

	}

	virtual void runDeActivationRoutine(){
		clearBits();
	}

	virtual void addSensorFlateTilstand(SensorFlateTilstand* sensorFlateTilstand){
		m_sensorFlateTilstand=sensorFlateTilstand;
	}
	virtual void setBits(){
		for(int i=m_startBit;i<=m_endBit;i++){
			m_sensorFlateTilstand->setBit(i);
		}
	}
	virtual void clearBits(){
		for(int i=m_startBit;i<=m_endBit;i++){
			m_sensorFlateTilstand->clearBit(i);
		}
	}


protected:
	b2Fixture* m_sensorFixture;
	int m_antallBit;
	SensorFlateTilstand* m_sensorFlateTilstand;
	int m_endBit;
	int m_startBit;
	string m_name;
};

class TellendeSensor : Sensor{
public:
	TellendeSensor(b2Fixture* sensorFixture,string name,int startBit,int endBit)
	:Sensor(sensorFixture,name,startBit,endBit){
		m_teller=0;
		m_tellerTop=pow(2,(m_endBit-m_startBit+1))-1;
	}
	virtual void runActivationRoutine(){
		updateTellerBits();
	}
	virtual void runDeActivationRoutine(){

	}

	void tellOpp(){
		m_teller++;
		m_teller%=(m_tellerTop+1);
	}

	void updateTellerBits(){
		tellOpp();
		m_sensorFlateTilstand->setToValue(m_startBit,m_endBit,m_teller);
	}

	string toString(){
		stringstream ss;
		ss.str("");
		ss<<m_teller<<" av "<<m_tellerTop;
		string s = ss.str();
		return s;
	}

private:
	unsigned int m_teller;
	unsigned int m_tellerTop;
};





class FaseSensor : Sensor{
public:
	FaseSensor(b2Fixture* sensorFixture,string name)
	:Sensor(sensorFixture){
		m_activationTimer = new b2Timer;
		m_deActivationTimer = new b2Timer;
		m_name=name;
		m_lastActivationTime=0.0f;
		m_lastDeActivationTime=0.0f;
		m_teller=0;
	}
//private:
	FaseSensor(b2Fixture* sensorFixture,string name,int startBit,int endBit)
	:Sensor(sensorFixture,name,startBit,endBit){

	}
public:


	void runActivationRoutine(){
		float64 time = m_activationTimer->GetMilliseconds();
		m_activationTimer->Reset();
		m_lastActivationTime+=time;
		m_teller++;
		//cout<<"FaseSensor: "<<m_name<<", aktivert: "<<time<<endl;
	}

	void runDeActivationRoutine(){
		float64 time = m_activationTimer->GetMilliseconds();
		m_deActivationTimer->Reset();
		m_lastDeActivationTime+=time;
		//cout<<"FaseSensor: "<<m_name<< ", deaktivert: "<<time<<endl;
	}

	float64 getTime() const {
		return m_lastActivationTime;
	}

	int getTeller() const {
		return m_teller;
	}

private:
	 void setBits(){
	}
	void clearBits(){
	}
	int m_teller;
	b2Timer* m_activationTimer;
	b2Timer* m_deActivationTimer;
	float64 m_lastActivationTime;
	float64  m_lastDeActivationTime;
	string m_name;

protected:

};


class SensorFlate : public b2ContactListener{
public:
	SensorFlate(SensorFlateTilstand *sensorFlateTilstand){
		m_sensorFlateTilstand=sensorFlateTilstand;
	}
	void addSensor(Sensor* sensor){
		m_sensorListe.push_back(sensor);
		sensor->addSensorFlateTilstand(m_sensorFlateTilstand);
	}

	void BeginContact(b2Contact* contact)
	{
		list<Sensor*>::iterator it;
		for(it=m_sensorListe.begin();it!=m_sensorListe.end();it++){
			if((*it)->isActivating(contact)){
				(*it)->runActivationRoutine();
			}
		}
	}

	void EndContact(b2Contact* contact)
	{
		list<Sensor*>::iterator it;
		for(it=m_sensorListe.begin();it!=m_sensorListe.end();it++){
			if((*it)->isDeActivating(contact)){
				(*it)->runDeActivationRoutine();
			}
		}
	}

private:
	list<Sensor*> m_sensorListe;
	SensorFlateTilstand* m_sensorFlateTilstand;
};


#endif /* SENSORFLATE_H_ */
