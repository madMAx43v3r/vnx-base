 /*************************************************************************
 * 
 *  [2017] - [2018] Automy Inc. 
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */

#include <vnx/vnx.h>
#include <vnx/Endpoint.hxx>
#include <vnx/UnixEndpoint.hxx>
#include <vnx/TcpEndpoint.hxx>

#include <mutex>
#include <memory>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif


namespace vnx {

static std::unique_ptr<sockaddr_in> resolve_sockaddr_in(const std::string& host_name, const int port)
{
	addrinfo hints = {};
	hints.ai_family = AF_UNSPEC;		// AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	addrinfo* res = nullptr;
	if(::getaddrinfo(host_name.c_str(), std::to_string(port).c_str(), &hints, &res)) {
		throw std::runtime_error("could not resolve: '" + host_name + "'");
	}
	for(auto host = res; host != nullptr; host = host->ai_next) {
		switch(host->ai_family) {
			case AF_INET: {
				auto out = new sockaddr_in();
				::memcpy(out, host->ai_addr, sizeof(sockaddr_in));
				::freeaddrinfo(res);
				return std::unique_ptr<sockaddr_in>(out);
			}
		}
	}
	for(auto host = res; host != nullptr; host = host->ai_next) {
		switch(host->ai_family) {
			case AF_INET6: {
				auto out = new sockaddr_in6();
				::memcpy(out, host->ai_addr, sizeof(sockaddr_in6));
				::freeaddrinfo(res);
				return std::unique_ptr<sockaddr_in>((sockaddr_in*)out);
			}
		}
	}
	::freeaddrinfo(res);
	throw std::runtime_error("missing AF_INET/6 for: '" + host_name + "'");
}

static void set_socket_options(int sock, int send_buffer_size, int receive_buffer_size) {
	if(send_buffer_size > 0 && setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&send_buffer_size, sizeof(send_buffer_size)) < 0) {
		throw std::runtime_error("setsockopt(SO_SNDBUF) failed with: " + get_socket_error_text());
	}
	if(receive_buffer_size > 0 && setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&receive_buffer_size, sizeof(receive_buffer_size)) < 0) {
		throw std::runtime_error("setsockopt(SO_RCVBUF) failed with: " + get_socket_error_text());
	}
}

static void set_tcp_socket_options(int sock, bool tcp_keepalive, bool tcp_no_delay) {
	int value = tcp_keepalive ? 1 : 0;
	if(value && setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&value, sizeof(value)) < 0) {
		throw std::runtime_error("setsockopt(SO_KEEPALIVE) failed with: " + get_socket_error_text());
	}
	value = tcp_no_delay ? 1 : 0;
	if(value && setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof(value)) < 0) {
		throw std::runtime_error("setsockopt(TCP_NODELAY) failed with: " + get_socket_error_text());
	}
}

std::string get_peer_address(int sock) {
	::sockaddr_storage sock_addr = {};
	::socklen_t addr_len = sizeof(sock_addr);
	::getpeername(sock, (::sockaddr*)&sock_addr, &addr_len);

	char address[INET6_ADDRSTRLEN] = {};
	const auto p_sock_addr = (const ::sockaddr*)&sock_addr;
	switch(p_sock_addr->sa_family) {
		case AF_INET:
			::inet_ntop(AF_INET, &((::sockaddr_in*)p_sock_addr)->sin_addr, address, sizeof(address));
			break;
		case AF_INET6:
			::inet_ntop(AF_INET6, &((::sockaddr_in6*)p_sock_addr)->sin6_addr, address, sizeof(address));
			break;
	}
	return std::string(address);
}


int32_t Endpoint::open() const {
	throw std::logic_error("not implemented");
	return -1;
}

void Endpoint::bind(const int32_t& socket) const {
	throw std::logic_error("not implemented");
}

void Endpoint::connect(const int32_t& socket) const {
	throw std::logic_error("not implemented");
}

void Endpoint::listen(const int32_t& socket) const {
	throw std::logic_error("not implemented");
}

void Endpoint::shutdown(const int32_t& socket, const int32_t& mode) const {
	throw std::logic_error("not implemented");
}

int32_t Endpoint::accept(const int32_t& socket) const {
	throw std::logic_error("not implemented");
	return -1;
}

std::string Endpoint::to_url() const {
	throw std::logic_error("not implemented");
	return "";
}

std::shared_ptr<const Endpoint> Endpoint::from_url(const std::string& url) {
	if(url.empty()) {
		return nullptr;
	}
	if(url.size() >= 5 && url.substr(url.size() - 5) == ".sock") {
		return UnixEndpoint::from_url(url);
	}
	return TcpEndpoint::from_url(url);
}

std::shared_ptr<const UnixEndpoint> UnixEndpoint::create_ex(const std::string& domain_path) {
	std::shared_ptr<UnixEndpoint> endpoint = std::make_shared<UnixEndpoint>();
	endpoint->domain_path = domain_path;
	return endpoint;
}

std::shared_ptr<const UnixEndpoint> UnixEndpoint::from_url(const std::string& url) {
	if(url.empty()) {
		return nullptr;
	}
	std::shared_ptr<UnixEndpoint> endpoint = std::make_shared<UnixEndpoint>();
	endpoint->domain_path = url;
	return endpoint;
}

void UnixEndpoint::listen(const int32_t& socket) const {
	if(::listen(socket, listen_queue_size) < 0) {
		throw std::runtime_error("listen() failed with: " + get_socket_error_text());
	}
}

void UnixEndpoint::shutdown(const int32_t& socket, const int32_t& mode) const {
	if(::shutdown(socket, mode) < 0) {
		throw std::runtime_error("shutdown() failed with: " + get_socket_error_text());
	}
}

void UnixEndpoint::set_options(const int32_t& socket) const {
	if(non_blocking) {
		set_socket_nonblocking(socket);
	}
	set_socket_options(socket, send_buffer_size, receive_buffer_size);
}

int32_t UnixEndpoint::accept(const int32_t& socket) const {
	const auto client = ::accept(socket, 0, 0);
	if(client < 0) {
		if(non_blocking) {
			return client;
		}
		throw std::runtime_error("accept() failed with: " + get_socket_error_text());
	}
	set_options(client);
	return client;
}

std::string UnixEndpoint::to_url() const {
	return domain_path;
}


std::shared_ptr<const TcpEndpoint> TcpEndpoint::create_ex(const std::string& host_name, const int32_t& port) {
	std::shared_ptr<TcpEndpoint> endpoint = std::make_shared<TcpEndpoint>();
	endpoint->host_name = host_name;
	endpoint->port = port;
	return endpoint;
}

std::shared_ptr<const TcpEndpoint> TcpEndpoint::from_url(const std::string& url) {
	if(url.empty()) {
		return nullptr;
	}
	std::string host = "localhost";
	int port = default_port;
	size_t pos = url.find(":");
	if(pos != std::string::npos) {
		if(pos > 0) {
			host = url.substr(0, pos);
		}
		if(pos + 1 < url.size()) {
			port = std::stoi(url.substr(pos + 1));
		}
	} else if(url.size()) {
		host = url;
	}
	return create_ex(host, port);
}

int32_t TcpEndpoint::open() const {
	const auto sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		throw std::runtime_error("socket() failed with: " + get_socket_error_text());
	}
	set_options(sock);
	return sock;
}

void TcpEndpoint::bind(const int32_t& socket) const {
	int enable = reuse_addr ? 1 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(int)) < 0) {
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed with: " + get_socket_error_text());
	}
	auto addr = resolve_sockaddr_in(host_name, port);
	if(::bind(socket, (::sockaddr*)addr.get(), addr->sin_family == AF_INET6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) < 0) {
		throw std::runtime_error("bind() failed with: " + get_socket_error_text());
	}
}

void TcpEndpoint::connect(const int32_t& socket) const {
	auto addr = resolve_sockaddr_in(host_name, port);
	if(::connect(socket, (::sockaddr*)addr.get(), addr->sin_family == AF_INET6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) < 0) {
		if(non_blocking) {
#ifdef _WIN32
			if(WSAGetLastError() == WSAEWOULDBLOCK)
#else
			if(errno == EINPROGRESS)
#endif
			{
				return;
			}
		}
		throw std::runtime_error("connect() failed with: " + get_socket_error_text());
	}
}

void TcpEndpoint::listen(const int32_t& socket) const {
	if(::listen(socket, listen_queue_size) < 0) {
		throw std::runtime_error("listen() failed with: " + get_socket_error_text());
	}
}

void TcpEndpoint::shutdown(const int32_t& socket, const int32_t& mode) const {
	if(::shutdown(socket, mode) < 0) {
		throw std::runtime_error("shutdown() failed with: " + get_socket_error_text());
	}
}

void TcpEndpoint::set_options(const int32_t& socket) const {
	if(non_blocking) {
		set_socket_nonblocking(socket);
	}
	set_socket_options(socket, send_buffer_size, receive_buffer_size);
	set_tcp_socket_options(socket, tcp_keepalive, tcp_no_delay);
}

int32_t TcpEndpoint::accept(const int32_t& socket) const {
	const auto client = ::accept(socket, 0, 0);
	if(client < 0) {
		if(non_blocking) {
			return client;
		}
		throw std::runtime_error("accept() failed with: " + get_socket_error_text());
	}
	set_options(client);
	return client;
}

std::string TcpEndpoint::to_url() const {
	return host_name + ":" + std::to_string(port);
}


} // vnx
