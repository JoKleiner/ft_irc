
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

void Server::create_channel(std::vector<std::string> channel_splits, size_t i, std::vector<std::string> password_splits)
{
	Channel chan(channel_splits[i], _clients[_iter]);

	if(i < password_splits.size() && !password_splits[i].empty())
		chan.set_channel_pw(password_splits[i]);

	_channels.insert({chan.get_channel_name(), chan});
}

bool Server::check_channel_syntax(std::vector<std::string> channel_splits, size_t i)
{
	if (!std::regex_match(channel_splits[i], std::regex("^[#+&][A-Za-z0-9\\-_\\^\\[\\]\\`\\{\\}]{1,49}$")))
	{
		std::string out = "403 \n\rError: Name invalide for channel: " + channel_splits[i] + "\n";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return (false);
	}
	return (true);
}

void Server::channel_join_reqest(std::vector<std::string> token)
{
	if(token[1].empty())
		return ;
	if(token[1] == "0")
	{
		// for (auto &chan : _channels)
		// {
		// 	leave_channel(chan.second);
		// }
		return ;
	}

	std::vector<std::string> channel_splits = split(token[1], ",");
	std::vector<std::string> password_splits;
	if(!token[2].empty())
		password_splits = split(token[2], ",");
	for(size_t i = 0; i < channel_splits.size(); i++)
	{
		if(!check_channel_syntax(channel_splits, i))
			continue ;
		auto chan = _channels.find(channel_splits[i].substr(1));
		if(chan == _channels.end())
			create_channel(channel_splits, i, password_splits);
		else
		{
			std::string channel_pw;
			if(i < password_splits.size() && !password_splits[i].empty())
				channel_pw = password_splits[i];
			chan->second.join(_clients[_iter], channel_pw);
		}
	}
}

// #, +, &, ! -Channel prefix
// #, &, ! = gleich normaler Channel
// + = unmoderarted Channel

// for(auto chan:_channels)
// {
// 	if(chan.second.get_channel_name() == vec_token[1];
// }