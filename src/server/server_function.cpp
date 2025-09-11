
#include "Server.hpp"

bool Server::checkNickname(const std::string &n_name)
{
	for (size_t i = 0; i < _clients.size(); ++i)
		if (_clients[i].get_nick() == n_name)
			return (false);
	
	return (true);
}

void Server::server_kick(size_t user, const std::string &reason)
{
	std::cout << "Client disconnected (FD: " << _fds[user].fd << ")" << std::endl;
	if (reason.empty())
		Server::leave_all_channel(_clients[user], "QUIT");
	else
	{
		Server::leave_all_channel(_clients[user], reason);
		SEND(_clients[user].get_fd(), ("Error :" + reason + "\n\r").c_str());
	}
	close(_fds[user].fd);
	_fds.erase(_fds.begin() + user);
	_clients.erase(_clients.begin() + user);
}

void Server::part(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendRplErr(_clients[_iter], SERVERNAME, "461", "PART :Not enough parameters");
	else
	{
		std::vector<std::string> channel_splits = split(token[1], ",");

		for (size_t i = 0; i < channel_splits.size(); i++)
		{
			auto channel_map_point = _channels.find(channel_splits[i]);
			if (channel_map_point == _channels.end())
				sendRplErr(_clients[_iter], SERVERNAME, "403", channel_splits[i] + " :No such channel");
			else
			{
				Channel &chan = channel_map_point->second;
				if (token.size() > 2)
					chan.leave_channel(_clients[_iter], token[2]);
				else
					chan.leave_channel(_clients[_iter]);
				if (chan.get_cha_cl_list().empty())
					_channels.erase(channel_map_point->first);
			}	
		}
	}
}
