
#include "Server.hpp"

void Server::create_channel(const std::string &channel, const std::string &channel_pw)
{
	if (!channel_pw.empty() && !check_channel_pw(channel_pw))
	{
		sendRplErr(_clients[_iter], SERVERNAME, "461", "JOIN :Not enough parameters");
		return;
	}

	Channel chan(channel, _clients[_iter]);

	chan.set_channel_pw(channel_pw);

	_channels.insert({chan.get_channel_name(), chan});
}

bool Server::check_channel_syntax(const std::string &channel)
{
	if (!std::regex_match(channel, std::regex("^[#+&][A-Za-z0-9\\-_\\^\\[\\]\\`\\{\\}]{1,49}$")))
		return (sendRplErr(_clients[_iter], SERVERNAME, "403", channel + " :Invalid channel name"), false);
	return (true);
}

void Server::leave_all_channel(const Client &client, const std::string &command, const std::string &msg)
{
	for (auto it = _channels.begin(); it != _channels.end();)
	{
		if (it->second.get_cha_cl_list().count(client.get_nick()))
			it->second.leave_channel(client, msg, command);
		if (it->second.get_cha_cl_list().empty())
			it = _channels.erase(it);
		else
			it++;
	}
}

void Server::join(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendRplErr(_clients[_iter], SERVERNAME, "461", "JOIN :Not enough parameters");
	else if (token[1] == "0")
	{
		if (token.size() > 1)
			leave_all_channel(_clients[_iter], "QUIT", token[2]);
		else
			leave_all_channel(_clients[_iter], "QUIT");
	}
	else
	{
		std::vector<std::string> channel_splits = split(token[1], ",");
		std::vector<std::string> password_splits;

		if (token.size() > 2)
			password_splits = split(token[2], ",");

		for (size_t i = 0; i < channel_splits.size(); i++)
		{
			std::string channel_pw;
			if (!check_channel_syntax(channel_splits[i]))
				continue;
			if (i < password_splits.size())
				channel_pw = password_splits[i];

			auto chan = _channels.find(channel_splits[i]);
			if (chan == _channels.end())
				create_channel(channel_splits[i], channel_pw);
			else
				chan->second.join(_clients[_iter], channel_pw);
		}
	}
}
