
#include "Server.hpp"

void Server::msg_client(const std::string &cl_name, const std::string &msg)
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
	sendRplErr(_clients[_iter], SERVERNAME, "401", cl_name + " :No such nick");
}

void Server::msg_channel(const std::string &channel, const std::string &msg)
{
	std::string cl_name = _clients[_iter].get_nick();

	if (_channels.find(channel) == _channels.end())
	{
		sendRplErr(_clients[_iter], SERVERNAME, "403", channel + " :No such channel");
		return;
	}

	Channel &chan = _channels.find(channel)->second;
	std::map<std::string, client_speci> cl_list = chan.get_cha_cl_list();

	if (cl_list.find(cl_name) == cl_list.end())
	{
		sendRplErr(_clients[_iter], SERVERNAME, "404", channel + " :ERR_CANNOTSENDTOCHAN");
		return;
	}
	std::string nmsg = ":" + _clients[_iter].get_user_whole_str() + " PRIVMSG " + channel + " :" + msg + "\r\n";
	chan.broadcast(cl_name, nmsg);
}

bool Server::check_privmsg_input(const std::vector<std::string> &token)
{
	if (token.size() < 3)
	{
		if (token.size() < 2)
			sendRplErr(_clients[_iter], SERVERNAME, "411", ":No recipient given (PRIVMSG)");
		else
			sendRplErr(_clients[_iter], SERVERNAME, "412", ":No text to send");
		return false;
	}
	return true;
}

void Server::privmsg(const std::vector<std::string> &token)
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
