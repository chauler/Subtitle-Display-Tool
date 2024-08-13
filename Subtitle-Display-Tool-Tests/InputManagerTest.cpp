#include"gtest/gtest.h"
#include"InputManager.h"
#include<asio/io_context.hpp>
#include<string>
#include<thread>


class InputManagerTest : public testing::Test {
protected:
	InputManagerTest(){
		
	}
};


TEST_F(InputManagerTest, CheckSocketCreation) {
	std::string empty_string = "";
	// set-up io_context and thread
	asio::io_context io_context;
	InputManager im{ 9999,io_context };
	asio::io_context::work idleWork(io_context);
	std::thread ioThread = std::thread([&]() { io_context.run(); });
	// should return a null string without an error, the socket is open
	std::string empty_socket_message = im.getData();
	// tear down io_context and its thread
	io_context.stop();
	ioThread.join();
	// string from getData should be empty because no host has connected
	EXPECT_EQ(empty_string, empty_socket_message);
}