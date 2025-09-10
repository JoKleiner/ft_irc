
#include "Server.hpp"

/*
	socket can accept connections over the network.
	AF_INET: IPv4

	int opt = 1: SO_REUSEADDR should be enabled
	setsockopt() expects 4 bytes. Therefore, use an integer instead of a bool.
	SOL_SOCKET: Category of the option (here: socket level).
	SO_REUSEADDR: Allows restarting the server without waiting for the port to be released.

	INADDR_ANY: listen on all network interfaces (e.g. localhost and external IPs).
	htons(8080): Port 8080, converted to network byte order (Big Endian).

	sockaddr: Initialize a structure containing the IP address and port.

	Bind: bind the socket to the configured address

	listen: Tell the socket to listen with a backlog of "SOMAXCONN" connections. SOMAXCONN = 128 max connections
*/

void Server::start(char **argv)
{
	int port;

	port = std::stoi(argv[1]);
	if (port < 1024 || port > 49151)
		throw(std::runtime_error("Port should be in between 1024-49151"));
	if (!check_pw_syntax(argv[2]))
		throw(std::runtime_error("Password contains invalid characters"));
	_password = argv[2];

	_sock = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(_sock, (sockaddr *)&addr, sizeof(addr)) < 0)
	{
		close(_sock);
		throw(std::runtime_error("Error: bind"));
	}

	listen(_sock, SOMAXCONN);

	_fds.push_back({_sock, POLLIN, 0});
	_clients.push_back(Client(_sock));
	_serverStart = std::chrono::system_clock::now();
	std::cout << "Server runs. Client access with: 'nc localhost " << port << "', Password: '" << _password << "'" << std::endl;
}
