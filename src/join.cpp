
#include "Server.hpp"
#include <regex>

void Server::create_channel(std::vector<std::string> channel_splits, size_t i, std::vector<std::string> password_splits)
{
	Channel chan(channel_splits[i], _clients[_iter]);

	if (i < password_splits.size() && !password_splits[i].empty())
		chan.set_channel_pw(password_splits[i]);

	_channels.insert({chan.get_channel_name(), chan});
}

bool Server::check_channel_syntax(std::vector<std::string> channel_splits, size_t i)
{
	if (!std::regex_match(channel_splits[i], std::regex("^[#+&][A-Za-z0-9\\-_\\^\\[\\]\\`\\{\\}]{1,49}$")))
	{
		sendERRRPL(_clients[_iter], SERVERNAME, "403", ":Name invalide for channel");
		return (false);
	}
	return (true);
}

void Server::leave_all_channel()
{
	for (auto it = _channels.begin(); it != _channels.end();)
	{
		it->second.leave_channel(_clients[_iter]);
		if (it->second.get_cha_cl_list().empty())
			it = _channels.erase(it);
		else
			it++;
	}
}

void Server::join(std::vector<std::string> token)
{
	if (!_clients[_iter].registered())
		sendERRRPL(_clients[_iter], SERVERNAME, "451", ":You have not registered");
	else if (token.size() < 2)
		sendERRRPL(_clients[_iter], SERVERNAME, "461", "JOIN :Not enough parameters");
	else if (token[1] == "0")
		leave_all_channel();
	else
	{
		std::vector<std::string> channel_splits = split(token[1], ",");
		std::vector<std::string> password_splits;
		if (token.size() >= 3)
			password_splits = split(token[2], ",");
		for (size_t i = 0; i < channel_splits.size(); i++)
		{
			if (!check_channel_syntax(channel_splits, i))
				continue;
			auto chan = _channels.find(channel_splits[i]);
			if (chan == _channels.end())
				create_channel(channel_splits, i, password_splits);
			else
			{
				std::string channel_pw;
				if (i < password_splits.size() && !password_splits[i].empty())
					channel_pw = password_splits[i];
				chan->second.join(_clients[_iter], channel_pw);
			}
		}
	}
}
