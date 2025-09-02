
#include "Server.hpp"
#include <regex>

void Server::msg_channel(std::string channel, std::string msg)
{
	std::string cl_name = _clients[_iter].get_nick();
	Channel chan;

	if(_channels.find(channel) == _channels.end())
	{
		std::string out = "403 \n\rERR_NOSUCHCHANNEL\n";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return ;
	}

	chan = _channels.find(channel)->second;
	std::map<std::string, client_speci> cl_list = chan.get_cha_cl_list();

	if(cl_list.find(cl_name) == cl_list.end())
	{
		std::string out = "404 \n\rRR_CANNOTSENDTOCHAN\n";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return ;
	}

	

	(void)msg;
}

std::string create_msg(std::vector<std::string> token)
{
	std::string msg;
	for (size_t i = 2; i < token.size(); i++)
	{
		msg += token[i];
		if(i != token.size())
			msg += ' ';
	}
	return (msg);
}

bool Server::check_privmsg_input(std::vector<std::string> token)
{
	if (token.size() < 3)
	{
		if (token.size() < 2 || token[1].find(':') != std::string::npos)
		{
			std::string out = "411 \n\rERR_NORECIPIENT\n";
			SEND(_clients[_iter].get_fd(), out.c_str());
		}
		else
		{
			std::string out = "412 \n\rERR_NOTEXTTOSEND\n";
			SEND(_clients[_iter].get_fd(), out.c_str());
		}
		return false;
	}
	if(token[2].find(':') != 0)
	{
		std::string out = "412 \n\rERR_NOTEXTTOSEND\n";
		SEND(_clients[_iter].get_fd(), out.c_str());
		return false;
	}
	return true;
}

void Server::privmsg(std::vector<std::string> token)
{
	if (!check_privmsg_input(token))
		return;
	std::string msg = create_msg(token);

	std::vector<std::string> msg_receiv = split(token[1], ",");
	for (size_t i = 0; i < msg_receiv.size(); i++)
	{
		switch (msg_receiv[i][0])
		{
			case '#': case '&': case '+':
				msg_channel(msg_receiv[i], msg);
				break;
			default:
				// ist ein Nick
				break;
		}
	}
	return;
}
