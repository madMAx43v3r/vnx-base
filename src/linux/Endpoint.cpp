
#include <vnx/TcpEndpoint.hxx>
#include <vnx/UnixEndpoint.hxx>
#include <vnx/Util.h>

#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>



namespace vnx{


::sockaddr_un get_sockaddr_bypath(const std::string& domain_path) {
	::sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	::strcpy(addr.sun_path, domain_path.c_str());
	return addr;
}


std::string get_socket_error_text() {
	return std::string(std::strerror(errno)) + " (" + std::to_string(errno) + ")";
}


void set_socket_nonblocking(int fd) {
	const auto res = ::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	if(res < 0) {
		throw std::runtime_error("fcntl() failed with: " + get_socket_error_text());
	}
}


int32_t UnixEndpoint::open() const {
	const auto sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock < 0) {
		throw std::runtime_error("socket() failed with: " + get_socket_error_text());
	}
	set_options(sock);
	return sock;
}


void UnixEndpoint::close(const int32_t& socket) const {
	::close(socket);
}


void UnixEndpoint::bind(const int32_t& socket) const {
	::unlink(domain_path.c_str());
	::sockaddr_un addr = get_sockaddr_bypath(domain_path);
	if(::bind(socket, (::sockaddr*)&addr, sizeof(addr)) < 0) {
		throw std::runtime_error("bind() failed with: " + get_socket_error_text());
	}
}


void UnixEndpoint::connect(const int32_t& socket) const {
	::sockaddr_un addr = get_sockaddr_bypath(domain_path);
	if(::connect(socket, (::sockaddr*)&addr, sizeof(addr)) < 0) {
		throw std::runtime_error("connect() failed with: " + get_socket_error_text());
	}
}


void TcpEndpoint::close(const int32_t& socket) const {
	::close(socket);
}


} // vnx


