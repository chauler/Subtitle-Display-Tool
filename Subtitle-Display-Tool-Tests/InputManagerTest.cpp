#include"gtest/gtest.h"
#include"InputManager.h"
#include<asio/io_context.hpp>
#include<string>
#include<thread>

int GetPortNum(InputManager& im) {

	return im.m_portNum;
}

bool GetAcceptorOpen(InputManager& im) {
	return im.acceptor_.is_open();
}


class InputManagerTest : public testing::Test {
protected:
	InputManagerTest(){
		
	}
};

TEST_F(InputManagerTest, CheckAcceptor) {
	//set-up io_context and thread
	asio::io_context io_context;
	InputManager im{ 9999,io_context };
	asio::io_context::work idleWork(io_context);
	std::thread ioThread = std::thread([&]() { io_context.run(); });
	// test whether the acceptor has the socket open
	bool isCreated = GetAcceptorOpen(im);
	// tear down io_context and its thread
	io_context.stop();
	ioThread.join();
	EXPECT_EQ(isCreated, true);
}

TEST_F(InputManagerTest, CheckManagerCreation) {
	std::string empty_string = "";
	// set-up io_context and thread
	asio::io_context io_context;
	InputManager im{ 9999,io_context };
	asio::io_context::work idleWork(io_context);
	std::thread ioThread = std::thread([&]() { io_context.run(); });
	// should return a null string without an error, the socket acceptor was created
	std::string empty_socket_message = im.getData();
	// tear down io_context and its thread
	io_context.stop();
	ioThread.join();
	// string from getData should be empty because no host has connected
	EXPECT_EQ(empty_string, empty_socket_message);
}

TEST_F(InputManagerTest, CheckPortNum) {
	// set-up io_context and thread
	asio::io_context io_context;
	InputManager im{ 9999,io_context };
	asio::io_context::work idleWork(io_context);
	std::thread ioThread = std::thread([&]() { io_context.run(); });
	// should return the private variable that stores the port number
	int current_portNum = GetPortNum(im);
	// tear down io_context and its thread
	io_context.stop();
	ioThread.join();
	EXPECT_EQ(current_portNum, 9999);
}