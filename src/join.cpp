
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
		std::string out = "403 \n\rError: Name invalide for channel: " + channel_splits[i] + "\n";
		SEND(_clients[_iter].get_fd(), out.c_str());
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
	if (token.size() < 2)
		return;
	if (token[1] == "0")
		return (leave_all_channel(), void());

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
