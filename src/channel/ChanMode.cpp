
#include "Channel.hpp"

void Channel::send_channel_mode(std::vector<std::string> token, Client client, size_t i)
{
	std::string out = client.get_user_whole_str() + " MODE " + token[1] + token[2][0] + token[2][i];
	for (auto it : m_cl_list)
		SEND(it.second.fd, out.c_str());
}

void Channel::InviteMode(std::vector<std::string> token, Client client, size_t i)
{
	if (token[2][0] == '-')
		m_invite_only = false;
	else
		m_invite_only = true;
	send_channel_mode(token, client, i);
}

void Channel::TopicMode(std::vector<std::string> token, Client client, size_t i)
{
	if (token[2][0] == '-')
		m_topic_operat = false;
	else
		m_topic_operat = true;
	send_channel_mode(token, client, i);
}

void Channel::KeyMode(std::vector<std::string> token, Client client, size_t i)
{
	if (token[2][0] == '-')
	{
		std::string out = client.get_user_whole_str() + " MODE " + token[1] + token[2][0] + token[2][i] + m_password;
		m_password = nullptr;
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
	}
	else if (token.size() != 4)
	{
		std::string out = "461 ERR_NEEDMOREPARAMS\n\r";
		SEND(client.get_fd(), out.c_str());
	}
	else
	{
		m_password = token[3];
		std::string out = client.get_user_whole_str() + " MODE " + token[1] + token[2][0] + token[2][i] + m_password;
		m_password = nullptr;
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
	}
}

void Channel::OperatMode(std::vector<std::string> token, Client client, size_t i)
{
	(void)token;
	(void)client;
	(void)i;
}

void Channel::LimitMode(std::vector<std::string> token, Client client, size_t i)
{
	(void)token;
	(void)client;
	(void)i;
}
