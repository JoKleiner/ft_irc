
#include "Channel.hpp"

void Channel::InviteMode(std::vector<std::string> token, Client client)
{
	if(token[2][0] == '-')
	{
		m_invite_only = false;
		for(auto it : m_cl_list)
		{
			std::string out = "324 RPL_CHANNELMODEIS channel needs no invite\n\r";
			SEND(client.get_fd(), out.c_str());
		}
	}
	else
	{
		m_invite_only = true;
		std::string out = "324 RPL_CHANNELMODEIS channel needs invite\n\r";
		SEND(client.get_fd(), out.c_str());
	}
}

void Channel::TopicMode(std::vector<std::string> token, Client client)
{
	(void)token;
	(void) client;
}

void Channel::KeyMode(std::vector<std::string> token, Client client)
{
	(void)token;
	(void) client;
}

void Channel::OperatMode(std::vector<std::string> token, Client client)
{
	(void)token;
	(void) client;
}

void Channel::LimitMode(std::vector<std::string> token, Client client)
{
	(void)token;
	(void) client;
}
