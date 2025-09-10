
#include "Server.hpp"

bool Server::checkNickname(const std::string &u_name)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].get_nick() == u_name)
			return (false);
	}
	return (true);
}

void Server::server_kick(size_t user, const std::string &reason)
{
	std::cout << "Client disconnected (FD: " << _fds[user].fd << ")" << std::endl;
	Server::leave_all_channel(_clients[user], "KICK");
	if(!reason.empty())
		SEND(_clients[user].get_fd(), reason.c_str());
	close(_fds[user].fd);
	_fds.erase(_fds.begin() + user);
	_clients.erase(_clients.begin() + user);
}

void Server::part(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendERRRPL(_clients[_iter], SERVERNAME, "461", "PART :Not enough parameters");
	else
	{
		std::vector<std::string> channel_splits = split(token[1], ",");

		for (size_t i = 0; i < channel_splits.size(); i++)
		{
			auto channel_map_point = _channels.find(channel_splits[i]);
			if (channel_map_point != _channels.end())
			{
				Channel &chan = channel_map_point->second;
				if (token.size() > 2)
					chan.leave_channel(_clients[_iter], token[2]);
				else
					chan.leave_channel(_clients[_iter]);
				if (chan.get_cha_cl_list().empty())
					_channels.erase(channel_map_point->first);
			}
			else
				sendERRRPL(_clients[_iter], SERVERNAME, "403", channel_splits[i] + " :No such channel");
		}
	}
}
