
#include "Server.hpp"
#include <regex>

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

void Server::kick_user(size_t user)
{
	std::cout << "Client disconnected (FD: " << _fds[user].fd << ")" << std::endl;
	close(_fds[user].fd);
	_fds.erase(_fds.begin() + user);
	_clients.erase(_clients.begin() + user);
}

void Server::send_channel_list(size_t iter)
{
	for (auto chan : _channels)
	{
		std::string msg = chan.second.get_channel_name() + "\n";
		send(_clients[iter].get_fd(), msg.c_str(), sizeof(msg), 0);
	}
}

void Server::leave_channel(Channel chan)
{
	chan.leave(_iter);
	// send(_clients[_iter].get_fd(), msg.c_str(), sizeof(msg), 0);
}

void Server::channel_join_reqest(std::string line)
{
	std::vector<std::string> splits = split(line, ",");
	// if(_vec_token[1][0] == '0')
	// {
	// 	for (auto &chan : _channels)
	// 	{
	// 		leave_channel(chan.second);
	// 	}
	// }
	// if(_vec_token[1][0] == '#', _vec_token[1][0] == '&', _vec_token[1][0] == '!')
}

// #, +, &, ! -Channel prefix
// #, &, ! = gleich normaler Channel
// + = unmoderarted Channel

// for(auto chan:_channels)
// {
// 	if(chan.second.get_channel_name() == vec_token[1];
// }