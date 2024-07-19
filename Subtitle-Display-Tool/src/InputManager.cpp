#include"InputManager.h"
#include<iostream>
#include<string>
#include<array>
#include<asio.hpp>

using asio::ip::tcp;
/*------------------------------------------------------------------------
* Return a string that is a a json file read from the buffer as a string
-------------------------------------------------------------------------*/
std::string InputManager::GetData(int portNum) {
	std::string default_ip = "127.0.0.1:" + portNum;
	std::string in_data;
	// create asio context
	asio::io_context io_context;
	// initialize acceptor to listen on the specified portnumber
	tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), portNum));
	// initialize buffer and put that data into a string
	asio::dynamic_string_buffer in_buf(in_data);
	//read data from connection
	for (;;) {
		tcp::socket socket(io_context);
		//listen for file:
		acceptor.accept(socket);
		std::error_code ignored_error;
		asio::read(socket, in_buf);
	}
	return in_data;
}