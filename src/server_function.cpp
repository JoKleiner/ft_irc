
#include "Server.hpp"
#include <regex>

bool Server::checkPassword(const std::string &pw)
{
	return (pw == _password);
}

bool Server::checkNickname(const std::string &un)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].get_nick() == un)
			return (false);
	}
	return (true);
}

void Server::server_kick(size_t user)
{
	std::cout << "Client disconnected (FD: " << _fds[user].fd << ")" << std::endl;
	close(_fds[user].fd);
	_fds.erase(_fds.begin() + user);
	_clients.erase(_clients.begin() + user);
}

void Server::send_channel_list(size_t iter)
{
	std::string msg = "\nList of open channels:\n";
	for (auto chan : _channels)
	{
		msg = msg + chan.second.get_channel_name() + "\n";
	}
	msg = msg + "\n";
	SEND(_clients[iter].get_fd(), msg.c_str());
}

void Server::leave_channel(std::vector<std::string> token)
{
	std::vector<std::string> channel_splits = split(token[1], ",");

	for (size_t i = 0; i < channel_splits.size(); i++)
	{
		auto channel_map_point = _channels.find(channel_splits[i]);
		if (channel_map_point != _channels.end())
		{
			Channel chan = channel_map_point->second;
			chan.leave_channel(_clients[_iter]);
			if (chan.get_cha_cl_list().empty())
				_channels.erase(channel_map_point->first);
		}
		else
		{
			std::string out = "Not existing channel: " + channel_splits[i] + "\n";
			SEND(_clients[_iter].get_fd(), out.c_str());
		}
	}
}
