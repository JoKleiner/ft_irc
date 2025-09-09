
#include "Channel.hpp"

void Channel::send_channel_mode(std::vector<std::string> &token, Client client, std::string mode)
{
	std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " " + token[2][0] + mode + "\n\r";
	for (auto it : m_cl_list)
		SEND(it.second.fd, out.c_str());
}

void Channel::InviteMode(std::vector<std::string> &token, Client client)
{
	if (token[2][0] == '-')
		m_invite_only = false;
	else
		m_invite_only = true;
	send_channel_mode(token, client, "i");
}

void Channel::TopicMode(std::vector<std::string> &token, Client client)
{
	if (token[2][0] == '-')
		m_topic_operat = false;
	else
		m_topic_operat = true;
	send_channel_mode(token, client, "t");
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

void Channel::KeyMode(std::vector<std::string> &token, Client client, size_t &mode_count)
{
	if (token[2][0] == '-')
	{
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " :-k" + "\n\r";
		m_password = "";
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
	}
	else if (token.size() < mode_count + 1)
		sendERRRPL(client, SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS +k");
	else if (!check_channel_pw(token[mode_count]))
		sendERRRPL(client, SERVERNAME, "477", "MODE :ERR_KEYSET +k");
	else
	{
		m_password = token[mode_count];
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " :+k " + m_password + "\n\r";
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
		mode_count++;
	}
}

void Channel::OperatMode(std::vector<std::string> &token, Client client, size_t &mode_count)
{
	if (token.size() < mode_count + 1)
		sendERRRPL(client, SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS +-o");
	else if (token[2][0] == '-')
	{
		if (m_cl_list.find(token[mode_count]) == m_cl_list.end())
			sendERRRPL(client, SERVERNAME, "441", std::string("MODE :ERR_USERNOTINCHANNEL -o ") + token[mode_count]);
		else if (!m_cl_list.find(token[mode_count])->second.ch_operator)
			sendERRRPL(client, SERVERNAME, "443", std::string("MODE :ERR_USERNOTOP -o ") + token[mode_count]);
		else
		{
			m_cl_list.find(token[mode_count])->second.ch_operator = false;
			std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " :-o " + token[mode_count] + "\n\r";
			for (auto it : m_cl_list)
				SEND(it.second.fd, out.c_str());
		}
	}
	else
	{
		if (m_cl_list.find(token[mode_count]) == m_cl_list.end())
			sendERRRPL(client, SERVERNAME, "441", std::string("MODE :ERR_USERNOTINCHANNEL +o ") + token[mode_count]);
		else
		{
			m_cl_list.find(token[mode_count])->second.ch_operator = true;
			std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " :+o " + token[mode_count] + "\n\r";
			for (auto it : m_cl_list)
				SEND(it.second.fd, out.c_str());
		}
		mode_count++;
	}
}

void Channel::LimitMode(std::vector<std::string> &token, Client client, size_t &mode_count)
{
	if (token[2][0] == '-')
	{
		m_chan_limit = 0;
		send_channel_mode(token, client, "l");
	}
	else if (token.size() < mode_count + 1)
		sendERRRPL(client, SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS +l");
	else if (!std::regex_match(token[mode_count], std::regex("^[0-9]{1,10}$")))
		sendERRRPL(client, SERVERNAME, "476", "MODE :ERR_BADCHANMASK +l");
	else if (stoi(token[mode_count]) == 0)
		sendERRRPL(client, SERVERNAME, "476", "MODE :ERR_BADCHANMASK +l");
	else
	{
		m_chan_limit = stoi(token[mode_count]);
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " :+l " + token[mode_count] + "\n\r";
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
		mode_count++;
	}
}
