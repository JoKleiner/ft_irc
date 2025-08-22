
#include "Server.hpp"

int Server::_sock = 0;
size_t Server::_iter = 0;
std::string Server::_password = "";
std::vector<pollfd> Server::_fds;
std::vector<Client> Server::_clients;

void Server::kick_user(size_t user)
{
	std::cout << "Client disconnected (FD: " << _fds[user].fd << ")" << std::endl;
	close(_fds[user].fd);
	_fds.erase(_fds.begin() + user);
	_clients.erase(_clients.begin() + user);
}

void Server::run()
{
	serverLoop();
	cleanup();
}

static std::vector<std::string> token_message(std::string client_mssg)
{
	std::vector<std::string> vec_token;
	std::istringstream ss(client_mssg);
	std::string token;

	while (ss >> token)
		vec_token.push_back(token);
	return (vec_token);
}

void Server::client_message()
{
	std::string client_mssg;
	if(!_clients[_iter].read_client_message(client_mssg))
	return ;

	std::cout << "Client (FD " << _fds[_iter].fd << "): " << client_mssg << std::flush;
	send(_fds[_iter].fd, "Message received\n", sizeof("Message received\n"), 0);

	if (client_mssg.empty())
	{
		kick_user(_iter);
		_iter--;
	}
	else if(std::vector<std::string> vec_token = token_message(client_mssg); vec_token.size() != 0)
		message_handling(vec_token);
}

void Server::connect_new_client()
{
	int client = accept(_sock, nullptr, nullptr);
	std::cout << "New client connected (FD: " << client << ")" << std::endl;
	_fds.push_back({client, POLLIN, 0});
	Client client_class(client);
	_clients.push_back(client_class);
}

void Server::iter_vec_pfds()
{
	_iter = 0;
	for (;_iter < _fds.size(); _iter++)
	{
		if (_fds[_iter].revents)
		{
			if (_fds[_iter].fd == _sock)
				Server::connect_new_client();
			else
				Server::client_message();
		}
	}
}

void Server::serverLoop()
{

	while (true)
	{
		int ret = poll(_fds.data(), _fds.size(), 0);
		if (ret == 0)
			continue;
		if (ret < 0)
		{
			std::cerr << "Error: poll() went wrong" << std::endl;
			break;
		}
		Server::iter_vec_pfds();
	}
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

bool Server::checkPassword(const std::string &pw)
{
	return (pw == _password);
}

bool Server::checkUsername(const std::string &un)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].get_nick() == un)
			return (false);
	}
	return (true);
}

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
	_password = argv[2];

	_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};                             
    addr.sin_family = AF_INET;                      
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);      

    if (bind(_sock, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(_sock);
		throw(std::runtime_error("Error: bind"));
	}
        
    listen(_sock, SOMAXCONN);

	_fds.push_back({_sock, POLLIN, 0});
	_clients.push_back(Client(_sock));
	std::cout << "Server runs. Client access with: 'nc localhost " << port << "', Password: '" << _password << "'" << std::endl;
}
