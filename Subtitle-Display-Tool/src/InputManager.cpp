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
	try {
		std::string in_data;
		std::size_t limit = 1000;
		// create asio context
		asio::io_context io_context;

		// initialize acceptor to listen on the specified portnumber
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), portNum));

		// initialize buffer and put that data into a string
		asio::dynamic_string_buffer in_buf(in_data, limit);
		//read data from connection
		for (;;) {
			tcp::socket socket(io_context);
			//listen for file:
			acceptor.accept(socket);

			std::error_code error;
			//read into the buffer
			asio::read(socket, in_buf);

			//break out of loop at the end
			if (error == asio::error::eof) {
				break;
			}
			else if (error) {
				throw std::system_error(error);
			}

		}
		return in_data;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
}