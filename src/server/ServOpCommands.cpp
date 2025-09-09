
#include "Server.hpp"

bool Server::check_ChaOpCo_input(std::vector<std::string> &token)
{
	auto chan_ele = token[0] == "INVITE" ? _channels.find(token[2]) : _channels.find(token[1]);

	if (chan_ele == _channels.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "403", token[0] + " :ERR_NOSUCHCHANNEL"), false);

	auto clie_list = chan_ele->second.get_cha_cl_list();
	auto client = clie_list.find(_clients[_iter].get_nick());

	if (client == clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "442", token[0] + " :ERR_NOTONCHANNEL"), false);
	if (token[0] != "TOPIC" && !client->second.ch_operator)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "482", token[0] + " :ERR_CHANOPRIVSNEEDED"), false);

	client = token[0] == "KICK" ? clie_list.find(token[2]) : clie_list.find(token[1]);

	if (token[0] == "KICK" && client == clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "441", token[0] + " :ERR_USERNOTINCHANNEL"), false);
	if (token[0] == "INVITE" && client != clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "443", token[0] + " :ERR_USERONCHANNEL"), false);
	return true;
}

void Server::KickInv(std::vector<std::string> &token)
{
	if (token.size() < 3)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", token[0] + " :ERR_NEEDMOREPARAMS"), void());
	if (!check_ChaOpCo_input(token))
		return;

	std::string out = ":" + _clients[_iter].get_user_whole_str() + " " + token[0] + " " + token[1] + " " + token[2];
	if (token.size() > 3)
		out += " " + token[3];
	out += "\n\r";
	Channel &chan = token[0] == "INVITE" ? _channels.find(token[2])->second : _channels.find(token[1])->second;
	for (auto it : chan.get_cha_cl_list())
		SEND(it.second.fd, out.c_str());
	
	if (token[0] == "KICK")
		chan.KickNick(token[2]);
	else
		chan.InvNick(token[1]);
}

void Server::topic(std::vector<std::string> &token)
{
	if (token.size() < 2)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "TOPIC :ERR_NEEDMOREPARAMS"), void());
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
			return (sendERRRPL(_clients[_iter], SERVERNAME, "482", token[0] + " :ERR_CHANOPRIVSNEEDED"), void());
	chan.SetTopic(token[2]);

	std::string out = ":" + _clients[_iter].get_user_whole_str() + " " + token[0] + " " + token[1] + " " + token[2] + "\n\r";
	for (auto it : chan.get_cha_cl_list())
		SEND(it.second.fd, out.c_str());
}
