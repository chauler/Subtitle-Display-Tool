#include "TcpConnection.h"
#include<asio.hpp>

typedef std::shared_ptr<TcpConnection> pointer;

pointer TcpConnection::create(asio::io_context& io_context) {
	return pointer(new TcpConnection(io_context));
}

asio::ip::tcp::socket& TcpConnection::socket() {
	return socket_;
}

void TcpConnection::start() {
	asio::async_read(socket_, asio::buffer(message), std::bind(&TcpConnection::handle_read, shared_from_this()));
}

TcpConnection::TcpConnection(asio::io_context& io_context) : socket_(io_context) {

}
void TcpConnection::handle_read() {
	
}