#pragma once
#include<asio/ip/tcp.hpp>
#include<asio/io_context.hpp>

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	typedef std::shared_ptr<TcpConnection> pointer;
	static pointer create(asio::io_context&);
	asio::ip::tcp::socket& socket();
	void start();
	std::string message;
private:
	TcpConnection(asio::io_context&);
	void handle_read();
	asio::ip::tcp::socket socket_;
};


