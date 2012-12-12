/** \file Communicator.h
 *  \brief Contains the Communicator-class.
 *
 *  \author Roy Kollen Svendsen
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

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

/** \brief Read frowm and write to an arbitrary serial-port.
 */
class Communicator{
public:
	/** \brief Create a Communicator instance and place it at a specific serialport.
	 *
	 * @param serieport The serialport in form of a string. E.g COM1 on windows or /dev/ttyS0 on linux.
	 */
	Communicator(string serieport);
	void init(string serieport);/**< \brief Initialize the serialport*/
	void write(const unsigned char c);/**< \brief Write a character to the serialport*/
	void write(string str);/**< \brief Write a string to the serialport*/
	void print(char *format, ...);/**< Write a formated string to the serialport*/
	bool dataIsPresent();/**< \brief Check if incoming data exist.*/
	unsigned char read();/**< \brief Read a char from incoming fifo-buffer.*/
	void close();/**< \brief Close serialport*/
	inline bool active(){	return m_active;}/**< \brief Check if port in use*/
private:
	static const int max_read_length = 512;
	void read_start(void);
	void read_complete(const boost::system::error_code& error, size_t bytes_transferred);
	void do_write(const unsigned char c);
	void write_start(void);
	void write_complete(const boost::system::error_code& error);
	void do_close(const boost::system::error_code& error);

	io_service m_io;
	serial_port m_port;
	bool m_dataIsPresent;
	thread communicationThread;
	deque<unsigned char> m_fifoWrite;
	deque<unsigned char> m_fifoRead;
	unsigned char read_msg_[max_read_length];
	bool m_active;
};

#endif /* COMMUNICATOR_H_ */
