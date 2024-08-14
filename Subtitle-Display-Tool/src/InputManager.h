#pragma once
#include<string>
#include"tcpConnection.h"


class InputManager {
public:
	InputManager(int portNum, asio::io_context& io_context);
	std::string getData();
private:
	int m_portNum;
	void startAccept();
	void handleAccept(TcpConnection::pointer new_connection, const std::error_code& error);
	asio::io_context& io_context_;
	asio::ip::tcp::acceptor acceptor_;
};