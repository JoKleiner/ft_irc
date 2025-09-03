
#include "Server.hpp"
#include <regex>

void Server::msg_client(std::string cl_name, std::string msg)
{
	for (auto &iter : _clients)
	{
		if (iter.get_nick() == cl_name)
		{
			std::string out = ":" + _clients[_iter].get_user_whole_str() + " PRIVMSG " +  iter.get_nick() + " :" + msg + "\r\n";
			SEND(iter.get_fd(), out.c_str());
			return;
		}
	}
	sendERRRPL(_clients[_iter], SERVERNAME, "401", ":ERR_NOSUCHNICK");
}

void Server::msg_channel(std::string channel, std::string msg)
{
	std::string cl_name = _clients[_iter].get_nick();

	if (_channels.find(channel) == _channels.end())
	{
		sendERRRPL(_clients[_iter], SERVERNAME, "403", ":ERR_NOSUCHCHANNEL");
		return;
	}

	Channel &chan = _channels.find(channel)->second;
	std::map<std::string, client_speci> cl_list = chan.get_cha_cl_list();

	if (cl_list.find(cl_name) == cl_list.end())
	{
		sendERRRPL(_clients[_iter], SERVERNAME, "404", ":ERR_CANNOTSENDTOCHAN");
		return;
	}
	std::string nmsg = ":" + _clients[_iter].get_user_whole_str() + " PRIVMSG " + channel + " :" + msg + "\r\n";
	chan.broadcast(cl_name, nmsg);
}

std::string create_msg(std::vector<std::string> token)
{
	std::string msg;
	for (size_t i = 2; i < token.size(); i++)
	{
		msg += token[i];
		if (i != token.size())
			msg += ' ';
	}
	return (msg);
}

bool Server::check_privmsg_input(std::vector<std::string> token)
{
	if (token.size() < 3)
	{
		if (token.size() < 2)
			sendERRRPL(_clients[_iter], SERVERNAME, "411", ":ERR_NORECIPIENT");
		else
			sendERRRPL(_clients[_iter], SERVERNAME, "412", ":ERR_NOTEXTTOSEND");
		return false;
	}
	return true;
}

void Server::privmsg(std::vector<std::string> token)
{
	if (!check_privmsg_input(token))
		return;

	std::vector<std::string> msg_receiv = split(token[1], ",");
	for (size_t i = 0; i < msg_receiv.size(); i++)
	{
		switch (msg_receiv[i][0])
		{
			case '#': case '&': case '+':
				msg_channel(msg_receiv[i], token[2]);
				break;
			default:
				msg_client(msg_receiv[i], token[2]);
				break;
		}
	}
	return;
}
