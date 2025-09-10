
#include "Server.hpp"

bool Server::check_mode_input(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "MODE :Not enough parameters"), false);

	auto chan_ele = _channels.find(token[1]);
	if (chan_ele == _channels.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "403", token[1] + " :No such channel"), false);

	if (token.size() > 2)
	{
		if (token[2].size() < 2 || (token[2][0] != '+' && token[2][0] != '-'))
			return (sendERRRPL(_clients[_iter], SERVERNAME, "472", std::string("") + token[2][0] + " :is unknown mode char to me for " + token[1]), false);

		for (size_t i = 1; i < token[2].size(); i++)
		{
			switch (token[2][i])
			{
			case 'i': case 't': case 'k': case 'o': case 'l':
				break;
			default:
				return (sendERRRPL(_clients[_iter], SERVERNAME, "472", std::string("") + token[2][i] + " :is unknown mode char to me for " + token[1]), false);
			}
		}

		auto clie_list = chan_ele->second.get_cha_cl_list();
		auto clie_ele = clie_list.find(_clients[_iter].get_nick());

		if (clie_ele == clie_list.end())
			return (sendERRRPL(_clients[_iter], SERVERNAME, "442", token[1] + " :You're not on that channel"), false);

		if (!clie_ele->second.ch_operator)
			return (sendERRRPL(_clients[_iter], SERVERNAME, "482", token[1] + " :You're not channel operator"), false);
	}
	return true;
}

void Server::mode(const std::vector<std::string> &token)
{
	if (!check_mode_input(token))
		return;
	Channel &chan = _channels.find(token[1])->second;
	if (token.size() == 2)
		return (chan.rpl_chan_modi(token, _clients[_iter]), void());
	size_t mode_count = 3;
	for (size_t i = 1; i < token[2].size(); i++)
	{
		switch (token[2][i])
		{
		case 'i':	chan.InviteMode(token, _clients[_iter]);				break;
		case 't':	chan.TopicMode(token, _clients[_iter]);					break;
		case 'k':	chan.KeyMode(token, _clients[_iter], mode_count);		break;
		case 'o':	chan.OperatMode(token, _clients[_iter], mode_count);	break;
		case 'l':	chan.LimitMode(token, _clients[_iter], mode_count);		break;
		}
	}
}
