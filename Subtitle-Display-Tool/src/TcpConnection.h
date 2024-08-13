#pragma once
#include<asio/ip/tcp.hpp>
#include<asio/io_context.hpp>
/**********************************************************************************
* Handles reading from the socket and provides a shared pointer for the connection
***********************************************************************************/
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	/***********************************************
	* shared pointer to handle the created socket
	************************************************/
	typedef std::shared_ptr<TcpConnection> pointer;
	/*******************************************************************
	* Creates and returns a socket based on the current io_context
	********************************************************************/
	static pointer create(asio::io_context&);
	asio::ip::tcp::socket& socket();
	/******************************************************************
	* Starts the asynchronous reading from the socket into a buffer
	*******************************************************************/
	void start();
	std::vector<char> message;
private:
	/********************************************************************
	* Handles the asynchronous read_some function
	* Keeps track of the bytes read into the buffer and loops the reading
	* ends if an error occurs
	*********************************************************************/
	TcpConnection(asio::io_context&);
	void handle_read(std::error_code, std::size_t);
	asio::ip::tcp::socket socket_;
};
