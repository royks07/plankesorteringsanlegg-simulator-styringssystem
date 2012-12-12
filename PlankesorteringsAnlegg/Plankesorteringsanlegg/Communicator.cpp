/*
 * Communicator.cpp
 *
 *  Created on: 8. des. 2012
 *      Author: roy
 */

#include "Communicator.h"



Communicator::Communicator(string serieport):m_port(m_io){
	try{
		init(serieport);
		read_start();
		boost::thread t(boost::bind(&boost::asio::io_service::run, &m_io));
	}catch (std::exception e) {
		cout<<"Error: unable to open serialport."<<endl;

	}
}

void Communicator::init(string serieport){
	m_port.open(serieport);

	m_port.set_option( serial_port_base::parity() );	//none
	m_port.set_option( serial_port_base::character_size( 8 ) );
	m_port.set_option( serial_port_base::stop_bits() );	//1
	m_port.set_option( serial_port_base::baud_rate( 115200 ) );

}

void Communicator::write(const unsigned char c){
		m_io.post(boost::bind(&Communicator::do_write, this, c));
}

void Communicator::write(string str){
	for(string::iterator it=str.begin();it!=str.end();it++)
		write((*it));
}


void Communicator::print(char *format, ...)
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

bool Communicator::dataIsPresent(){
	if(!m_fifoRead.empty()) return true;
	else return false;

}

unsigned char Communicator::read(){
	if(!m_fifoRead.empty()){
		unsigned char retChar = m_fifoRead.front();
		m_fifoRead.pop_front();
		return retChar;
	}else return NULL;
}

void Communicator::close()
{
		m_io.post(boost::bind(&Communicator::do_close, this, boost::system::error_code()));
}

void Communicator::read_start(void){
	   m_port.async_read_some(boost::asio::buffer(read_msg_, max_read_length),
				boost::bind(&Communicator::read_complete,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
}

void Communicator::read_complete(const boost::system::error_code& error, size_t bytes_transferred){
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

void Communicator::do_write(const unsigned char c){
		bool write_in_progress = !m_fifoWrite.empty();
		m_fifoWrite.push_back(c);
		if (!write_in_progress)
				write_start();
}

void Communicator::write_start(void){
	boost::asio::async_write(m_port,
		boost::asio::buffer(&m_fifoWrite.front(), 1),
		boost::bind(&Communicator::write_complete,
					this,
					boost::asio::placeholders::error));
}

void Communicator::write_complete(const boost::system::error_code& error){
	if (!error){
		m_fifoWrite.pop_front();
		if (!m_fifoWrite.empty())
			write_start();
	}
	else
		do_close(error);
}

void Communicator::do_close(const boost::system::error_code& error){
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
