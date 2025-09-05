
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

static bool check_channel_pw(std::string line)
{
	if (line.size() < 2 || line.size() >= 32)
		return false;

	for (size_t i = 0; i < line.size(); i++)
		if (!std::isprint(line[i]) || line[i] == ' ')
            return false;
	
	return true;
}

void Channel::KeyMode(std::vector<std::string> token, Client client, size_t i, size_t mode_count)
{
	if (token[2][0] == '-')
	{
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + token[2][0] + token[2][i] + "\n\r";
		m_password = nullptr;
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
	}
	else if (token.size() < mode_count + 1)
		sendERRRPL(client, SERVERNAME, "461", std::string("MODE :ERR_NEEDMOREPARAMS ") + token[2][0] + token[2][i]);
	else if (!check_channel_pw(token[mode_count]))
		sendERRRPL(client, SERVERNAME, "477", std::string("MODE :ERR_KEYSET ") + token[2][0] + token[2][i]);
	else
	{
		m_password = token[mode_count];
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " :" + token[2][0] + token[2][i] + " " + m_password + "\n\r";
		m_password = nullptr;
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
	}
}

void Channel::OperatMode(std::vector<std::string> token, Client client, size_t i, size_t mode_count)
{
	(void)token;
	(void)client;
	(void)i;
	(void)mode_count;
}

void Channel::LimitMode(std::vector<std::string> token, Client client, size_t i, size_t mode_count)
{
	(void)token;
	(void)client;
	(void)i;
	(void)mode_count;
}
