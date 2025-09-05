
#include "Server.hpp"

bool Server::check_mode_input(std::vector<std::string> &token)
{
	if (token.size() < 3) 
		return (sendERRRPL(_clients[_iter], SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS"), false);

	auto chan_ele = _channels.find(token[1]);
	if (chan_ele == _channels.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "403", "MODE :ERR_NOSUCHCHANNEL"), false);

	if (token[2].size() < 2 || ( token[2][0] != '+' && token[2][0] != '-'))
		return (sendERRRPL(_clients[_iter], SERVERNAME, "472", "MODE :ERR_UNKNOWNMODE"), false);

	for (size_t i = 1; i < token[2].size(); i++)
	{
		switch (token[2][i])
		{
			case 'i':  case 't':  case 'k':  case 'o':  case 'l':
				break;
			default:
				return (sendERRRPL(_clients[_iter], SERVERNAME, "472", "MODE :ERR_UNKNOWNMODE"), false);
		}
	}

	auto clie_list = chan_ele->second.get_cha_cl_list();
	auto clie_ele = clie_list.find(_clients[_iter].get_nick());

	if (clie_ele == clie_list.end())
		return (sendERRRPL(_clients[_iter], SERVERNAME, "442", "MODE :ERR_NOTONCHANNEL"), false);

	if (!clie_ele->second.ch_operator)
		return (sendERRRPL(_clients[_iter], SERVERNAME, "482", "MODE :ERR_CHANOPRIVSNEEDED"), false);

	return true;
}

void Server::mode(std::vector<std::string> &token)
{
	if (!check_mode_input(token))
		return;
	Channel &chan = _channels.find(token[1])->second;
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
