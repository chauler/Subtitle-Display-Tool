#include"InputManager.h"
#include"TcpConnection.h"
#include<array>
#include<asio.hpp>
#include<iostream>
#include<string>

//convenience for socket
using asio::ip::tcp;
std::string current_data;

/*--------------------------------------------------------------------------------
* Constructor for InputManager
----------------------------------------------------------------------------------*/
InputManager::InputManager(int portNum, asio::io_context& io_context) : m_portNum(portNum), io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), portNum)) {
	startAccept();
}

void InputManager::startAccept() {
	TcpConnection::pointer new_connection = TcpConnection::create(io_context_);
	acceptor_.async_accept(new_connection->socket(), std::bind(&InputManager::handleAccept, this, new_connection, asio::placeholders::error));
}

void InputManager::handleAccept(TcpConnection::pointer new_connection, const std::error_code& error) {
	if (!error) {
		new_connection->start();
		current_data = new_connection->message;
	}
	startAccept();
}

std::string InputManager::getData() {
	return current_data;
}