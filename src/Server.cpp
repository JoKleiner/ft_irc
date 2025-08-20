#include "Server.hpp"

int Server::_sock = 0;
std::string Server::_password = "";
std::vector<pollfd> Server::_fds;
std::vector<Client> Server::_clients;

void Server::run()
{
	serverLoop();
	cleanup();
}

void Server::start(int argc, char **argv)
{
	int port;
	if (argc != 3)
		throw std::invalid_argument("Error: server start with './ircserv <port> <password>'");
	if (!std::regex_match(argv[1], std::regex("(\\d{4,5})")))
		throw std::invalid_argument("Error: Port not in correct Form");
	port = std::stoi(argv[1]);
	if (port < 1024 || port > 49151)
		throw(std::runtime_error("Port should be in between 1024-49151"));
	_password = argv[2];

	// Create socket (File Descriptor): This socket can accept connections over the network.
	_sock = socket(AF_INET, SOCK_STREAM, 0);

	/*
		int opt = 1: SO_REUSEADDR should be enabled
		setsockopt() expects 4 bytes. Therefore, use an integer instead of a bool.
		SOL_SOCKET: Category of the option (here: socket level).
		SO_REUSEADDR: Allows restarting the server without waiting for the port to be released.
	*/
	int opt = 1;
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Initialize a structure containing the IP address and port.
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	// INADDR_ANY: listen on all network interfaces (e.g. localhost and external IPs).
	// htons(8080): Port 8080, converted to network byte order (Big Endian).

	// Bind the socket to the configured address
	bind(_sock, (sockaddr *)&addr, sizeof(addr));

	// Tell the socket to listen with a backlog of "SOMAXCONN" connections. SOMAXCONN = 128 max connections
	listen(_sock, SOMAXCONN);

	_fds.push_back({_sock, POLLIN, 0});
	_clients.push_back(Client(_sock));
	std::cout << "Server runs. Client access with: 'nc localhost " << port << "', Password: '" << _password << "'" << std::endl;
}

void Server::serverLoop()
{
	int ret;
	while ((ret = poll(_fds.data(), _fds.size(), 0)) >= 0)
	{
		if (ret == 0) // ret = 0, when no change in fds
			continue;

		// Iterate through all file descriptors
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents)
			{
				if (_fds[i].fd == _sock)
				{
					int client = accept(_sock, nullptr, nullptr);
					std::cout << "New client connected (FD: " << client << ")" << std::endl;
					fcntl(client, F_SETFL, O_NONBLOCK);
					_fds.push_back({client, POLLIN, 0});
					Client client_class(client);
					_clients.push_back(client_class);
				}
				else
				{
					std::string buffer = _clients[i].getMessage();

					if (buffer.find('\n') != std::string::npos)
						parseInput(buffer, _clients[i]);
					else
					{
						std::cout << "Client disconnected (FD: " << _fds[i].fd << ")" << std::endl;
						close(_fds[i].fd);
						_fds.erase(_fds.begin() + i);
						i--;
					}
				}
			}
		}
	}
	if (ret < 0)
		throw std::runtime_error("Error: poll failed");
}

void Server::cleanup()
{
	for (auto &fd : _fds)
	{
		if (fd.fd != -1)
			close(fd.fd);
	}
	close(_sock);
}

void Server::parseInput(std::string &input, Client &client)
{
	std::string word;
	std::istringstream ss(input);
	ss >> word;
	if (word == "PASS")
	{
		ss >> word;
		client.check_pw(_password, word);
	}
	if (word == "USER")
		client.set_user();
	if (word == "NICK")
		client.set_nick();

	std::cout << "Client (FD " << client.getFd() << "): " << input << std::flush;
	send(client.getFd(), "Message received\n", sizeof("Message received\n"), 0);
}
