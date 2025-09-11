
#include "Server.hpp"

bool Server::check_ChaOpCo_input(const std::vector<std::string> &token)
{
	auto chan_ele = token[0] == "INVITE" ? _channels.find(token[2]) : _channels.find(token[1]);

	if (chan_ele == _channels.end())
		return (sendRplErr(_clients[_iter], SERVERNAME, "403", token[0] + " :ERR_NOSUCHCHANNEL"), false);

	auto clie_list = chan_ele->second.get_cha_cl_list();
	auto client = clie_list.find(_clients[_iter].get_nick());

	if (client == clie_list.end())
		return (sendRplErr(_clients[_iter], SERVERNAME, "442", token[0] + " :ERR_NOTONCHANNEL"), false);
	if (token[0] != "TOPIC" && !client->second.ch_operator)
		return (sendRplErr(_clients[_iter], SERVERNAME, "482", token[0] + " :ERR_CHANOPRIVSNEEDED"), false);

	client = clie_list.find(token[1]);

	if (token[0] == "INVITE" && client != clie_list.end())
		return (sendRplErr(_clients[_iter], SERVERNAME, "443", token[0] + " :ERR_USERONCHANNEL"), false);
	return true;
}

bool Server::check_kick(const std::vector<std::string> &token, std::string &channel, std::string &nick_splits)
{
	auto chan_ele = _channels.find(channel);
	if (chan_ele == _channels.end())
		return (sendRplErr(_clients[_iter], SERVERNAME, "403", token[0] + " :ERR_NOSUCHCHANNEL"), false);
	auto clie_list = chan_ele->second.get_cha_cl_list();
	auto client = clie_list.find(_clients[_iter].get_nick());
	if (client == clie_list.end())
		return (sendRplErr(_clients[_iter], SERVERNAME, "442", token[0] + " :ERR_NOTONCHANNEL"), false);
	if (!client->second.ch_operator)
		return (sendRplErr(_clients[_iter], SERVERNAME, "482", token[0] + " :ERR_CHANOPRIVSNEEDED"), false);

	client = clie_list.find(nick_splits);
	if (client == clie_list.end())
		return (sendRplErr(_clients[_iter], SERVERNAME, "441", token[0] + " :ERR_USERNOTINCHANNEL"), false);
	if (client->second.ch_operator)
		return (sendRplErr(_clients[_iter], SERVERNAME, "482", token[0] + " :ERR_BIGGERCHANOPRIVSNEEDED"), false);
	return true;
}

void Server::kick(const std::vector<std::string> &token)
{
	if (token.size() < 3)
		return (sendRplErr(_clients[_iter], SERVERNAME, "461", token[0] + " :ERR_NEEDMOREPARAMS"), void());

	std::vector<std::string> channel_splits = split(token[1], ",");
	std::vector<std::string> nick_splits = split(token[2], ",");
	
	if (channel_splits.size() != 1 && channel_splits.size() != nick_splits.size())
		return (sendRplErr(_clients[_iter], SERVERNAME, "461", token[0] + " :ERR_NEEDMOREPARAMS"), void());

	for (size_t i = 0; i < nick_splits.size(); i++)
	{
		std::string channel = channel_splits[0];
		if (channel_splits.size() != 1)
			channel = channel_splits[i];

		if (!check_kick(token, channel, nick_splits[i]))
			return;

		std::string out = ":" + _clients[_iter].get_user_whole_str() + " " + token[0] + " " + channel + " " + nick_splits[i];
		if (token.size() > 3)
			out += " " + token[3];
		out += "\r\n";

		Channel &chan = _channels.find(channel)->second;
		for (auto it : chan.get_cha_cl_list())
			SEND(it.second.fd, out.c_str());
		chan.KickNick(nick_splits[i]);
	}
}

void Server::invite(const std::vector<std::string> &token)
{
	if (token.size() < 3)
		return (sendRplErr(_clients[_iter], SERVERNAME, "461", token[0] + " :ERR_NEEDMOREPARAMS"), void());
	if (!check_ChaOpCo_input(token))
		return;

	std::string out = ":" + _clients[_iter].get_user_whole_str() + " " + token[0] + " " + token[1] + " " + token[2] + "\r\n";
	Channel &chan = _channels.find(token[2])->second;
	for (auto it : chan.get_cha_cl_list())
		SEND(it.second.fd, out.c_str());
	chan.InvNick(token[1]);
}

void Server::topic(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		return (sendRplErr(_clients[_iter], SERVERNAME, "461", "TOPIC :ERR_NEEDMOREPARAMS"), void());
	if (!check_ChaOpCo_input(token))
		return;
	Channel &chan = _channels.find(token[1])->second;
	if (token.size() == 2)
	{
		std::string topic = chan.get_topic();
		std::string out = ":" + _clients[_iter].get_user_whole_str() + " " + token[0] + " " + token[1] + " :" + topic + "\n\r";
		return (SEND(_clients[_iter].get_fd(), out.c_str()), void());
	}
	if (chan.get_topic_op())
		if (!chan.get_cha_cl_list().find(_clients[_iter].get_nick())->second.ch_operator)
			return (sendRplErr(_clients[_iter], SERVERNAME, "482", token[0] + " :ERR_CHANOPRIVSNEEDED"), void());
	chan.SetTopic(token[2]);

	std::string out = ":" + _clients[_iter].get_user_whole_str() + " " + token[0] + " " + token[1] + " " + token[2] + "\n\r";
	for (auto it : chan.get_cha_cl_list())
		SEND(it.second.fd, out.c_str());
}
