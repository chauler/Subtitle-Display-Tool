#include "TcpConnection.h"
#include<asio.hpp>
#include <iostream>
#include <functional>

typedef std::shared_ptr<TcpConnection> pointer;
extern std::string current_data;
//std::vector<char> vBuffer(1 * 1024);


TcpConnection::TcpConnection(asio::io_context& io_context) : message(1 * 1024), socket_(io_context) {
}


pointer TcpConnection::create(asio::io_context& io_context) {
	return pointer(new TcpConnection(io_context));
}


asio::ip::tcp::socket& TcpConnection::socket() {
	return socket_;
}


void TcpConnection::start() {
	socket_.async_read_some(asio::buffer(message.data(), message.size()),
		std::bind(&TcpConnection::handle_read, shared_from_this(),
			std::placeholders::_1, std::placeholders::_2));
}



void TcpConnection::handle_read(std::error_code ec, std::size_t bytes) {
		if (!ec) {
			//std::cout << "Read " << bytes << " bytes\n\n";

			for (int i = 0; i < bytes; i++) {
				//std::cout << message[i];
				current_data += message[i];
			}

			start();
		}
}