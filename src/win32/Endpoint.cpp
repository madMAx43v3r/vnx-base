
#include <vnx/TcpEndpoint.hxx>
#include <vnx/UnixEndpoint.hxx>
#include <vnx/Util.h>

#include <winsock2.h>
#include <ws2tcpip.h>


namespace vnx {

unsigned int get_port_bypath(const std::string &path) {
	return (std::hash<std::string>{}(path) % 10000) + 30000;
}


std::string get_socket_error_text() {
	return std::to_string(WSAGetLastError());
}


void set_socket_nonblocking(int fd) {
	u_long mode = 1;
	const auto res = ::ioctlsocket(fd, FIONBIO, &mode);
	if(res != 0){
		throw std::runtime_error("ioctlsocket() failed with: " + get_socket_error_text());
	}
}


int32_t UnixEndpoint::open() const {
	const auto sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		throw std::runtime_error("socket() failed with: " + get_socket_error_text());
	}
	set_options(sock);
	return sock;
}


void UnixEndpoint::close(const int32_t& socket) const {
	::closesocket(socket);
}


void UnixEndpoint::bind(const int32_t& socket) const {
	unsigned int port = get_port_bypath(domain_path);

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(::bind(socket, (sockaddr *)&addr, sizeof(addr)) < 0){
		throw std::runtime_error("bind() failed with: " + get_socket_error_text());
	}
	if(::listen(socket, 5) != 0){
		throw std::runtime_error("listen() failed with: " + get_socket_error_text());
	}
}


void UnixEndpoint::connect(const int32_t& socket) const {
	unsigned int port = get_port_bypath(domain_path);

	char port_str[16];
	snprintf(port_str, 16, "%d", port);

	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	addrinfo *info;
	if(getaddrinfo("localhost", port_str, &hints, &info) != 0){
		throw std::runtime_error("getaddrinfo() failed with: " + get_socket_error_text());
	}

	bool connected = false;
	for(addrinfo *r=info; r!=NULL; r=r->ai_next){
		if(::connect(socket, r->ai_addr, r->ai_addrlen) == -1){
			closesocket(socket);
			continue;
		}

		connected = true;
		break;
	}
	freeaddrinfo(info);
	if(!connected) throw std::runtime_error("could not establish connection");
}


void TcpEndpoint::close(const int32_t& socket) const {
	::closesocket(socket);
}



} // vnx



