
#include "Channel.hpp"

void Channel::send_channel_mode(const std::vector<std::string> &token, const Client &client, std::string mode)
{
	for (auto it : m_cl_list)
		sendRplErr(it.second.fd, client.get_user_whole_str(), "MODE", token[1] + " " + token[2][0] + mode);
}

void Channel::InviteMode(const std::vector<std::string> &token, const Client &client)
{
	if (token[2][0] == '-')
		m_invite_only = false;
	else
		m_invite_only = true;
	send_channel_mode(token, client, "i");
}

void Channel::TopicMode(const std::vector<std::string> &token, const Client &client)
{
	if (token[2][0] == '-')
		m_topic_operat = false;
	else
		m_topic_operat = true;
	send_channel_mode(token, client, "t");
}

bool check_channel_pw(std::string line, const Client &client)
{
	if (line.size() < 2 || line.size() >= 32)
		return (sendRplErr(client, SERVERNAME, "525", ":Invalid key"), false);

	for (size_t i = 0; i < line.size(); i++)
		if (!std::isprint(line[i]) || line[i] == ' ')
		return (sendRplErr(client, SERVERNAME, "525", ":Invalid key"), false);
	
	return true;
}

void Channel::KeyMode(const std::vector<std::string> &token, const Client &client, size_t &mode_count)
{
	if (token[2][0] == '-')
	{
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " -k" + "\n\r";
		m_password = "";
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
	}
	else if (token.size() < mode_count + 1)
		sendRplErr(client, SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS +k");
	else if (!check_channel_pw(token[mode_count], client))
		return ;
	else
	{
		m_password = token[mode_count];
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " +k " + m_password + "\n\r";
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
		mode_count++;
	}
}

void Channel::OperatMode(const std::vector<std::string> &token, const Client &client, size_t &mode_count)
{
	if (token.size() < mode_count + 1)
		sendRplErr(client, SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS +-o");
	else if (token[2][0] == '-')
	{
		if (m_cl_list.find(token[mode_count]) == m_cl_list.end())
			sendRplErr(client, SERVERNAME, "441", std::string("MODE :ERR_USERNOTINCHANNEL -o ") + token[mode_count]);
		else if (!m_cl_list.find(token[mode_count])->second.ch_operator)
			sendRplErr(client, SERVERNAME, "443", std::string("MODE :ERR_USERNOTOP -o ") + token[mode_count]);
		else
		{
			m_cl_list.find(token[mode_count])->second.ch_operator = false;
			std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " -o " + token[mode_count] + "\n\r";
			for (auto it : m_cl_list)
				SEND(it.second.fd, out.c_str());
		}
	}
	else
	{
		if (m_cl_list.find(token[mode_count]) == m_cl_list.end())
			sendRplErr(client, SERVERNAME, "441", std::string("MODE :ERR_USERNOTINCHANNEL +o ") + token[mode_count]);
		else
		{
			m_cl_list.find(token[mode_count])->second.ch_operator = true;
			std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " +o " + token[mode_count] + "\n\r";
			for (auto it : m_cl_list)
				SEND(it.second.fd, out.c_str());
		}
		mode_count++;
	}
}

void Channel::LimitMode(const std::vector<std::string> &token, const Client &client, size_t &mode_count)
{
	if (token[2][0] == '-')
	{
		m_chan_limit = 0;
		send_channel_mode(token, client, "l");
	}
	else if (token.size() < mode_count + 1)
		sendRplErr(client, SERVERNAME, "461", "MODE :ERR_NEEDMOREPARAMS +l");
	else if (!std::regex_match(token[mode_count], std::regex("^[0-9]{1,10}$")))
		sendRplErr(client, SERVERNAME, "476", "MODE :ERR_BADCHANMASK +l");
	else if (stoi(token[mode_count]) == 0)
		sendRplErr(client, SERVERNAME, "476", "MODE :ERR_BADCHANMASK +l");
	else
	{
		m_chan_limit = stoi(token[mode_count]);
		std::string out = ":" + client.get_user_whole_str() + " MODE " + token[1] + " +l " + token[mode_count] + "\n\r";
		for (auto it : m_cl_list)
			SEND(it.second.fd, out.c_str());
		mode_count++;
	}
}

void Channel::rpl_chan_modi(const std::vector<std::string> &token, const Client &client)
{
	std::string out = token[1];
	if (m_topic_operat || m_invite_only || m_chan_limit > 0 || !m_password.empty())
	{
		out += " +";
		out += m_topic_operat 		? "t" : "";
		out += m_invite_only  		? "i" : "";
		out += m_chan_limit > 0 	? "l" : "";
		out += !m_password.empty() 	? "k" : "";

		out += m_chan_limit > 0 	? " " + std::to_string(m_chan_limit) : "";
		out += !m_password.empty() 	? " " + m_password 	 : "";
	}
	sendRplErr(client, SERVERNAME, "324", out);
}
