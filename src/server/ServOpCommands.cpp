
#include "Server.hpp"

bool Server::check_ChaOpCo_input(const std::vector<std::string> &token)
{
	auto chan_ele = token[0] == "INVITE" ? _channels.find(token[2]) : _channels.find(token[1]);

	if (chan_ele == _channels.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "403", (token[0] == "INVITE" ? token[2] : token[1]) + " :No such channel"), false);

	auto clie_list = chan_ele->second.get_cha_cl_list();
	auto client = clie_list.find(_clients[_iter].get_nick());
	std::string channelName = chan_ele->second.get_channel_name();

	if (client == clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "442", channelName + " :You're not on that channel"), false);
	if (token[0] != "TOPIC" && !client->second.ch_operator)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "482", channelName + " :You're not channel operator"), false);

	client = clie_list.find(token[1]);

	if (token[0] == "INVITE" && client != clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "443", token[1] + " " + channelName + " :is already on channel"), false);
	return true;
}

bool Server::check_kick(const std::vector<std::string> &token, std::string &channel, std::string &nick_splits)
{
	auto chan_ele = _channels.find(channel);
	if (chan_ele == _channels.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "403", channel + " :No such channel"), false);
	auto clie_list = chan_ele->second.get_cha_cl_list();
	auto client = clie_list.find(_clients[_iter].get_nick());
	if (client == clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "442", channel + " :You're not on that channel"), false);
	if (!client->second.ch_operator)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "482", channel + " :You're not channel operator"), false);

	client = clie_list.find(nick_splits);
	if (client == clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "441", nick_splits + " " + channel +" :They aren't on that channel"), false);
	if (client->second.ch_operator)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "482", channel + " :You're not allowed to kick operator"), false);
	return true;
}

void Server::kick(const std::vector<std::string> &token)
{
	if (token.size() < 3)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "KICK :Not enough parameters"), void());

	std::vector<std::string> channel_splits = split(token[1], ",");
	std::vector<std::string> nick_splits = split(token[2], ",");

	if (channel_splits.size() != 1 && channel_splits.size() != nick_splits.size())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "KICK :Not enough parameters"), void());

	for (size_t i = 0; i < nick_splits.size(); i++)
	{
		std::string channel = channel_splits[0];
		if (channel_splits.size() != 1)
			channel = channel_splits[i];

		if (!check_kick(token, channel, nick_splits[i]))
			return;

		Channel &chan = _channels.find(channel)->second;
		chan.broadcast(_clients[_iter].get_user_whole_str(), "KICK", channel + " " + nick_splits[i] + (token.size() > 3 ? " " + token[3] : ""));
		chan.KickNick(nick_splits[i]);
	}
}

void Server::invite(const std::vector<std::string> &token)
{
	if (token.size() < 3)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "INVITE :Not enough parameters"), void());
	if (!check_ChaOpCo_input(token))
		return;

	Channel &chan = _channels.find(token[2])->second;
	chan.broadcast(_clients[_iter].get_user_whole_str(), "INVITE", token[1] + " " + token[2]);
	chan.InvNick(token[1]);
}

void Server::topic(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "TOPIC :Not enough parameters"), void());
	if (!check_ChaOpCo_input(token))
		return;
	Channel &chan = _channels.find(token[1])->second;
	if (token.size() == 2)
	{
		std::string topic = chan.get_topic();
		sendERRRPL(_clients[_iter].get_fd(), _clients[_iter].get_user_whole_str(), "TOPIC", token[1] + " :" + topic);
		return;
	}
	if (chan.get_topic_op())
		if (!chan.get_cha_cl_list().find(_clients[_iter].get_nick())->second.ch_operator)
			return (sendERRRPL(_clients[_iter], SERVERNAME, "482", chan.get_channel_name() + " :You're not channel operator"), void());
	chan.SetTopic(token[2]);
	chan.broadcast(_clients[_iter].get_user_whole_str(), "TOPIC", token[1] + " " + token[2]);
}
