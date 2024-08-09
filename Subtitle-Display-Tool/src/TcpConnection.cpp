#include "TcpConnection.h"
#include<asio.hpp>
#include <iostream>
#include <functional>

typedef std::shared_ptr<TcpConnection> pointer;
extern std::string current_data;
//std::vector<char> vBuffer(1 * 1024);

/**********************************************************
* Constructor for TcpConnection
***********************************************************/
TcpConnection::TcpConnection(asio::io_context& io_context) : message(1 * 1024), socket_(io_context) {
}

/*******************************************************************
* Called to create the shared pointer for the current TcpConnection
* Returns a pointer to the TcpConnection
********************************************************************/
pointer TcpConnection::create(asio::io_context& io_context) {
	return pointer(new TcpConnection(io_context));
}

/*******************************************************************
* Creates and returns a socket based on the current io_context
********************************************************************/
asio::ip::tcp::socket& TcpConnection::socket() {
	return socket_;
}

/******************************************************************
* Starts the asynchronous reading from the socket into a buffer
*******************************************************************/
void TcpConnection::start() {
	socket_.async_read_some(asio::buffer(message.data(), message.size()),
		std::bind(&TcpConnection::handle_read, shared_from_this(),
			std::placeholders::_1, std::placeholders::_2));
}


/********************************************************************
* Handles the asynchronous read_some function
* Keeps track of the bytes read into the buffer and loops the reading
* ends if an error occurs
*********************************************************************/
void TcpConnection::handle_read(std::error_code ec, std::size_t bytes) {
		if (!ec) {
			std::cout << "Read " << bytes << " bytes\n\n";

			for (int i = 0; i < bytes; i++) {
				std::cout << message[i];
				current_data += message[i];
			}

			start();
		}
}