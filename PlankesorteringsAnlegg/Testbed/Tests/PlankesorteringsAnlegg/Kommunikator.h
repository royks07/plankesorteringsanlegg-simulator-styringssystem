/*
 * Kommunikator.h
 *
 *  Created on: 17. nov. 2012
 *      Author: roy
 */

#ifndef KOMMUNIKATOR_H_
#define KOMMUNIKATOR_H_

#include <iostream>
#include <string.h>
#include <sstream>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include <exception>

#include <deque>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

using namespace std;
using namespace boost;
using namespace boost::asio;

class Communicator{
public:


	/*! \brief Doxygen-eksempel - mscgen og graphviz må være installert...
	\msc
	arcgradient = 8;
	a [label="Client"],b [label="Server"];
	a-xb [label="get accel"];
	a=>b [label="get accel"];
	a<=b [label="ack"];
	a<=b [label="accel data"];
	\endmsc

	\dot

	digraph example_api_graph {
	node [shape=box];
	DBNC_KEY_IDLE    [fillcolor=lightblue,style=filled,label="DBNC_KEY_IDLE" ];
	DBNC_KEY_PRESSED [fillcolor=lightblue,style=filled,label="DBNC_KEY_PRESSED" ];
	DBNC_KEY_RELEASE [fillcolor=lightblue,style=filled,label="DBNC_KEY_WAIT_RELEASE"];
	DBNC_KEY_IDLE -> DBNC_KEY_PRESSED -> DBNC_KEY_RELEASE ;
	DBNC_KEY_PRESSED -> DBNC_KEY_PRESSED ;
	DBNC_KEY_IDLE -> DBNC_KEY_IDLE ;
	}
	\enddot
	*/
	Communicator(string serieport):m_port(m_io){
			init(serieport);
			read_start();
			boost::thread t(boost::bind(&boost::asio::io_service::run, &m_io));
	}

	/*!
	 *
	 * @param serieport
	 */
	void init(string serieport){
		m_port.open(serieport);
		m_port.set_option( serial_port_base::parity() );	//none
		m_port.set_option( serial_port_base::character_size( 8 ) );
		m_port.set_option( serial_port_base::stop_bits() );	//1
		m_port.set_option( serial_port_base::baud_rate( 115200 ) );

	}

	void write(const unsigned char c){
			m_io.post(boost::bind(&Communicator::do_write, this, c));
	}

	void write(string str){
		for(string::iterator it=str.begin();it!=str.end();it++)
			write((*it));
	}


	void print(char *format, ...)
	{
		char buffer[256];
	    va_list args;
	    va_start(args,format);
	    vsprintf (buffer,format,args);
	    stringstream ss;
	    ss.str("");
	    ss<<buffer;
	    write(ss.str());
	}

	bool dataIsPresent(){
		if(!m_fifoRead.empty()) return true;
		else return false;

	}

	unsigned char read(){
		if(!m_fifoRead.empty()){
			unsigned char retChar = m_fifoRead.front();
			m_fifoRead.pop_front();
			return retChar;
		}else return NULL;
	}

	void close()
	{
			m_io.post(boost::bind(&Communicator::do_close, this, boost::system::error_code()));
	}

	bool active()
	{
			return m_active;
	}


private:

	static const int max_read_length = 512;

	void read_start(void){
		   m_port.async_read_some(boost::asio::buffer(read_msg_, max_read_length),
					boost::bind(&Communicator::read_complete,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
	}

	void read_complete(const boost::system::error_code& error, size_t bytes_transferred){
		if (!error){
			//cout.write(read_msg_, bytes_transferred);
			for(int i=0;i<bytes_transferred;i++){
				m_fifoRead.push_back(read_msg_[i]);
			}
			read_start();
		}
		else
			do_close(error);
	}

	void do_write(const unsigned char c){
			bool write_in_progress = !m_fifoWrite.empty();
			m_fifoWrite.push_back(c);
			if (!write_in_progress)
					write_start();
	}

	void write_start(void){
		boost::asio::async_write(m_port,
			boost::asio::buffer(&m_fifoWrite.front(), 1),
			boost::bind(&Communicator::write_complete,
						this,
						boost::asio::placeholders::error));
	}

	void write_complete(const boost::system::error_code& error){
		if (!error){
			m_fifoWrite.pop_front();
			if (!m_fifoWrite.empty())
				write_start();
		}
		else
			do_close(error);
	}

	void do_close(const boost::system::error_code& error){
		if (error == boost::asio::error::operation_aborted)
				return;
		if (error)
				cerr << "Error: " << error.message() << endl;
		else
				cout << "Error: Connection did not succeed.\n";
		cout << "Press Enter to exit\n";
		m_port.close();
		m_active = false;
	}

private:
	io_service m_io;
	serial_port m_port;
	bool m_dataIsPresent;
	thread communicationThread;
	deque<unsigned char> m_fifoWrite;
	deque<unsigned char> m_fifoRead;
	unsigned char read_msg_[max_read_length];
	bool m_active;
};


#endif /* KOMMUNIKATOR_H_ */
