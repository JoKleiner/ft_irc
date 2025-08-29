
#include "Server.hpp"

int Server::_sock = 0;
size_t Server::_iter = 0;
std::string Server::_password = "";
std::vector<pollfd> Server::_fds;
std::vector<Client> Server::_clients;
std::map<std::string, Channel> Server::_channels;

void Server::client_message()
{
	std::string client_mssg;
	if(!_clients[_iter].read_client_message(client_mssg))
		return ;

	if (client_mssg.empty())
	{
		kick_user(_iter);
		_iter--;
	}
	else
		message_handling(client_mssg);
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

void Server::run()
{
	serverLoop();
	cleanup();
}
