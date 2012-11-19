/*
 * BaneSynkronisator.h
 *
 *  Created on: 17. nov. 2012
 *      Author: roy
 */

#ifndef BANESYNKRONISATOR_H_
#define BANESYNKRONISATOR_H_

#include <list>
#include <algorithm>


	class BaneSensorPar{
	public:
		BaneSensorPar(Bane* bane,FaseSensor* sensor){
			m_bane=bane;
			m_sensor=sensor;
		}

		Bane * m_bane;
		FaseSensor * m_sensor;
	};

	class BaneSynkronisator{
	public:
		BaneSynkronisator(){
		}

		void addBaneSensorPar(BaneSensorPar* baneSensorPar){
			m_baneSensorParListe.push_back(baneSensorPar);
			//m_timer = new b2Timer;
		}

		void run(){
			m_tellere.clear();
			list<BaneSensorPar*>::iterator it;
			it = m_baneSensorParListe.begin();
			while(it!=m_baneSensorParListe.end()){
				FaseSensor* faseSensor = ((*it)->m_sensor);
				int teller = faseSensor->getTeller();
				m_tellere.push_back(teller);
				it++;
			}
			m_currentHighestCount = *max_element(m_tellere.begin(),m_tellere.end());
			m_currentLowestCount = *min_element(m_tellere.begin(),m_tellere.end());
			cout<<"m_currentHighestCount: "<<m_currentHighestCount<<" m_currentLowestCount: "<<m_currentLowestCount<<endl;

			if(m_currentHighestCount!=m_currentLowestCount){
				it = m_baneSensorParListe.begin();
				while(it!=m_baneSensorParListe.end()){
					FaseSensor* faseSensor = ((*it)->m_sensor);
					int teller = faseSensor->getTeller();
					if(teller>=m_currentHighestCount) ((*it)->m_bane)->stop();
					else ((*it)->m_bane)->start();
					it++;
				}
			}else{
				it = m_baneSensorParListe.begin();
				while(it!=m_baneSensorParListe.end()){
					FaseSensor* faseSensor = ((*it)->m_sensor);
					int teller = faseSensor->getTeller();
					((*it)->m_bane)->start();
					it++;
				}

			}

		}

	private:
		//b2Timer* m_timer;
		list<BaneSensorPar*> m_baneSensorParListe;
		int m_currentHighestCount;
		int m_currentLowestCount;
		list<int> m_tellere;
	};



/*class BaneSynkronisator2{
	public:

		BaneSynkronisator2(Bane* bane1,Bane* bane2,FaseSensor* sensor1,FaseSensor* sensor2){
			m_bane1=bane1;
			m_bane2=bane2;
			m_sensor1=sensor1;
			m_sensor2=sensor2;
			m_timer = new b2Timer;
		}

		void run(){
			float32 timeSensor1 = m_sensor1->getTime();
			float32 timeSensor2 = m_sensor2->getTime();
			float32 timeDiff = timeSensor2-timeSensor1;
			unsigned int tellerSensor1 = m_sensor1->getTeller();
			unsigned int tellerSensor2 = m_sensor2->getTeller();
			signed int tellerDiff = tellerSensor2-tellerSensor1;
			float32 speedBane1=m_bane1->getSpeed();
			float32 speedBane2=m_bane2->getSpeed();
			float32 speedDiff=speedBane2-speedBane1;

			signed int synkError;
			if(tellerDiff==0){
				m_bane2->restart();
				m_bane1->restart();

				if(timeDiff>0&speedDiff>=0){
					m_bane2->stop();
				}else if(timeDiff<0&speedDiff<=0){
					m_bane1->stop();
				}else
					m_bane2->restart();
					m_bane1->restart();

				cout<<"FaseSensor timeDiff: "<<timeDiff <<" tellerDiff: "<<tellerDiff<<endl;
				synkError=timeDiff;

			}else{
				if(tellerDiff>0){
					m_bane2->stop();
					m_bane1->restart();
				}else if(tellerDiff<0){
					m_bane1->stop();
					m_bane2->restart();
				}


			}
		}

	private:
		Bane* m_bane1;
		Bane* m_bane2;
		FaseSensor* m_sensor1;
		FaseSensor* m_sensor2;
		b2Timer* m_timer;
	};

*/
#endif /* BANESYNKRONISATOR_H_ */
