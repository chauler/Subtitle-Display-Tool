#include"InputManager.h"
#include"TcpConnection.h"
#include<array>
#include<asio.hpp>
#include<iostream>
#include<string>
#include <stack>

//convenience for socket
using asio::ip::tcp;
std::string current_data;

/***********************************************************************************************
* Constructor for InputManager
* @param portNum: integer used as the port number when creating the socket
* @param io_context: an asio object that handles continuity of the socket and async functions
************************************************************************************************/
InputManager::InputManager(int portNum, asio::io_context& io_context) : m_portNum(portNum), io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), portNum)) {
	startAccept();
}

/***************************************************************************************************
* Creates a pointer to the current io_context
* Starts an asynchronous accept function that listens for a tcp connection on the user-defined port
****************************************************************************************************/
void InputManager::startAccept() {
	TcpConnection::pointer new_connection = TcpConnection::create(io_context_);
	acceptor_.async_accept(new_connection->socket(), std::bind(&InputManager::handleAccept, this, new_connection, asio::placeholders::error));
}

/**********************************************************************************************
* Token for the asynchronos connection acceptor
* Handles the acceptor's connection state
* Hands the connection back to the asynchronous read funcion if there is no error
* Starts a new connection if there was an error
***********************************************************************************************/
void InputManager::handleAccept(TcpConnection::pointer new_connection, const std::error_code& error) {
	if (!error) {
		new_connection->start();
	}
	startAccept();
}

/***********************************************************************************************
* Gathers data from socket's buffer
* Checks if the file recieved is in json format
* Constructs and returns a json formatted string by checking for braces in the current data
************************************************************************************************/
std::string InputManager::getData() {
	std::stack<char> opening;
	if (current_data.length() == 0 || current_data[0] != '{') {
		return "";
	}

	//First char is an opening brace, proceed to match braces
	opening.push(current_data[0]);
	int closingPos = 0;
	for (int i = 1; i < current_data.length(); i++) {
		if (current_data[i] == '{') {
			opening.push(current_data[i]);
		}
		else if (current_data[i] == '}') {
			opening.pop();
		}

		//Last brace was popped: this is a complete JSON message
		if (opening.size() == 0) {
			closingPos = i;
			break;
		}
	}

	//Last closing brace was never found, invalid JSON (so far)
	if (closingPos == 0) {
		std::cout << "Incomplete JSON message" << std::endl;
		return "";
	}

	std::string output = current_data.substr(0, closingPos+1);
	current_data.erase(0, closingPos+1);

	return output;
}