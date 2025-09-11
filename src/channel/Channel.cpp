
#include "Channel.hpp"

void Channel::ChannelWelcomeMessage(const Client &client)
{
	broadcast(client.get_user_whole_str(), "JOIN", ":" + get_channel_name());
	if (m_topic.empty())
		sendERRRPL(client, SERVERNAME, "331", this->get_channel_name() + " :No topic set");
	else
		sendERRRPL(client, SERVERNAME, "332", this->get_channel_name() + " :" + m_topic);
	std::string msg = "= " + get_channel_name() + " :";
	for (auto &[name, sp] : m_cl_list)
		msg += (sp.ch_operator == true ? "@" : "+") + name + " ";
	msg.pop_back();
	sendERRRPL(client, SERVERNAME, "353", msg);
	sendERRRPL(client, SERVERNAME, "366", get_channel_name() + " :End of NAMES list");
}

Channel::Channel(std::string name, Client client) : m_name(name), m_topic_operat(false), m_invite_only(false), m_chan_limit(0)
{
	client_speci cur_client;
	if (name[0] == '+')
		cur_client.ch_operator = false;
	else
		cur_client.ch_operator = true;
	cur_client.fd = client.get_fd();
	m_cl_list[client.get_nick()] = cur_client;

	ChannelWelcomeMessage(client);
}

void Channel::leave_channel(const Client &client, const std::string &msg, const std::string &command)
{
	if (m_cl_list.find(client.get_nick()) != m_cl_list.end())
	{
		if (command == "PART")
			broadcast(client.get_user_whole_str(), command, get_channel_name() + " :" + msg);
		else
			broadcast(client.get_user_whole_str() ,command + " :" + msg);
		m_cl_list.erase(client.get_nick());
	}
	else
		sendERRRPL(client, SERVERNAME, "442", get_channel_name() + " :You're not on that channel");
}

void Channel::join(Client client, std::string channel_pw)
{
	auto inv_client = std::find(m_invite_list.begin(), m_invite_list.end(), client.get_nick());

	if (m_cl_list.find(client.get_nick()) != m_cl_list.end())
		ChannelWelcomeMessage(client);
	else if (m_invite_only && inv_client == m_invite_list.end())
		sendERRRPL(client, SERVERNAME, "473", get_channel_name() + ":Cannot join channel (+i)");
	else if (m_chan_limit != 0 && m_cl_list.size() >= m_chan_limit)
		sendERRRPL(client, SERVERNAME, "471", get_channel_name() + ":Cannot join channel (+l)");
	else if (channel_pw != m_password)
		sendERRRPL(client, SERVERNAME, "475", get_channel_name() + ":Cannot join channel (+k)");
	else
	{
		client_speci client_spec;
		client_spec.ch_operator = false;
		client_spec.fd = client.get_fd();
		m_cl_list[client.get_nick()] = client_spec;

		if (m_invite_only == true)
			m_invite_list.erase(inv_client);

		ChannelWelcomeMessage(client);
	}
}

const std::map<std::string, client_speci> &Channel::get_cha_cl_list() const
{
	return (m_cl_list);
}

const std::string &Channel::get_channel_name() const
{
	return (m_name);
}

const std::string &Channel::get_topic() const
{
	return (m_topic);
}

const bool &Channel::get_topic_op() const
{
	return (m_topic_operat);
}

void Channel::set_channel_pw(std::string password)
{
	m_password = password;
}

void Channel::broadcast(std::string sender, std::string msg) const
{
	std::cout << msg;
	for (auto &[name, specification] : m_cl_list)
	{
		if (name != sender)
			SEND(specification.fd, msg.c_str());
	}
}

void Channel::broadcast(const std::string &prefix, const std::string &command, const std::string &params) const
{
	for (auto &[name, specification] : m_cl_list)
		sendERRRPL(specification.fd, prefix, command, params);
}
