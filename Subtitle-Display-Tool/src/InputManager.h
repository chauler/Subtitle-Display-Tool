#pragma once
#include<string>
#include"tcpConnection.h"

/***********************************************************************************************
* Handles input and connections from the host application
* @param portNum: integer used as the port number when creating the socket
* @param io_context: an asio object that handles continuity of the socket and async functions
************************************************************************************************/
class InputManager {
public:
	InputManager(int portNum, asio::io_context& io_context);
	/***********************************************************************************************
	* Gathers data from socket's buffer
	* Checks if the file recieved is in json format
	* Constructs and returns a json formatted string by checking for braces in the current data
	************************************************************************************************/
	std::string getData();
private:
	int m_portNum;
	/***************************************************************************************************
	* Creates a pointer to the current io_context
	* Starts an asynchronous accept function that listens for a tcp connection on the user-defined port
	****************************************************************************************************/
	void startAccept();
	/**********************************************************************************************
	* Token for the asynchronos connection acceptor
	* Handles the acceptor's connection state
	* Hands the connection back to the asynchronous read funcion if there is no error
	* Starts a new connection if there was an error
	***********************************************************************************************/
	void handleAccept(TcpConnection::pointer new_connection, const std::error_code& error);
	asio::io_context& io_context_;
	asio::ip::tcp::acceptor acceptor_;
};