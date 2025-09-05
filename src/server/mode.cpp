
#include "Server.hpp"

bool Server::check_mode_input(std::vector<std::string> token)
{
	if (token.size() < 3) {
		std::string out = "461 ERR_NEEDMOREPARAMS\n\r";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return false;
	}
	if (_channels.find(token[1]) == _channels.end()) {
		std::string out = "403 ERR_NOSUCHCHANNEL\n\r";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return false;
	}
	if (token[2].size() < 2 || token[2][0] != '+' || token[2][0] != '-') {
		std::string out = "472 ERR_UNKNOWNMODE\n\r";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return false;
	}
	for (size_t i = 1; i < token[2].size(); i++)
	{
		switch (token[2][i])
		{
		case 'i':  case 't':  case 'k':  case 'o':  case 'l':
			break;
		default:
			std::string out = "472 ERR_UNKNOWNMODE\n\r";
			SEND(_clients[_iter].get_fd(), out.c_str());
			return false;
		}
	}
	return true;
}
/*
	i: Set/remove Invite-only channel
	t: Set/remove the restrictions of the TOPIC command to channel operators
	k: Set/remove the channel key (password)
	o: Give/take channel operator privilege
	l: Set/remove the user limit to channel
*/
/*
	472 ERR_UNKNOWNMODE			Mode existiert nicht 			(MODE #chat +z)
	403 ERR_NOSUCHCHANNEL		Channel existiert nicht			(MODE #nichtvorhanden +i)
	482 ERR_CHANOPRIVSNEEDED	Keine Rechte, Mode zu setzen	(MODE #chat +i)
	401 ERR_NOSUCHNICK			Nick existiert nicht			(MODE NichtExistierenderNick +i)
	461	ERR_NEEDMOREPARAMS		Parameter fehlen				(MODE #chat +o)
*/

void Server::mode(std::vector<std::string> token)
{
	if (!check_mode_input(token))
		return;
	Channel &chan = _channels.find(token[1])->second;
	size_t mode_count = 3;
	for (size_t i = 1; i < token[2].size(); i++)
	{
		switch (token[2][i])
		{
		case 'i':	chan.InviteMode(token, _clients[_iter], i);					break;
		case 't':	chan.TopicMode(token, _clients[_iter], i);					break;
		case 'k':	chan.KeyMode(token, _clients[_iter], i, mode_count++);		break;
		case 'o':	chan.OperatMode(token, _clients[_iter], i, mode_count++);	break;
		case 'l':	chan.LimitMode(token, _clients[_iter], i, mode_count++);	break;
		}
	}
}
