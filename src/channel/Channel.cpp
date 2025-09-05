
#include "Channel.hpp"

void Channel::ChannelWelcomeMessage(const Client &client)
{
	sendERRRPL(client, client.get_user_whole_str(), "JOIN", ":" + get_channel_name());
	broadcast(client.get_nick(), ":" + client.get_user_whole_str() + " JOIN :" + get_channel_name() + "\r\n");
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

Channel::Channel(std::string name, Client client) :
	m_name(name),	m_topic_operat(false),	m_invite_only(false),	m_chan_limit(0)
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

const std::map<std::string, client_speci> &Channel::get_cha_cl_list() const{
	return (m_cl_list);
}

void Channel::leave_channel(const Client &client, const std::string &msg, const std::string &command)
{
	if (m_cl_list.find(client.get_nick()) != m_cl_list.end())
	{
		m_cl_list.erase(client.get_nick());
		if (command == "PART")
			broadcast(client.get_nick(), (":" + client.get_user_whole_str() + " " + command + " " + get_channel_name() + " :" + msg + "\r\n").c_str());
		else
			broadcast(client.get_nick(), (":" + client.get_user_whole_str() + " " + command + " :" + msg + "\r\n").c_str());
	}
	else
		sendERRRPL(client, SERVERNAME, "442", get_channel_name() + " :You're not on that channel");
}

void Channel::join(Client client, std::string channel_pw)
{
	if (!client.registered())
		sendERRRPL(client, SERVERNAME, "451", ":You have not registered");
	else if(m_invite_only == true && std::find(m_invite_list.begin(), m_invite_list.end(), client.get_nick())	!= m_invite_list.end())
		sendERRRPL(client, SERVERNAME, "473", ":ERR_INVITEONLYCHAN");
	else if (m_chan_limit != 0 && m_cl_list.size() >= m_chan_limit)
		sendERRRPL(client, SERVERNAME, "471", ":ERR_CHANNELISFULL");
	else if (channel_pw != m_password)
		sendERRRPL(client, SERVERNAME, "464", ":Password incorrect");
	else
	{
		if (m_cl_list.find(client.get_nick()) != m_cl_list.end())
			return;
		client_speci client_spec;
		client_spec.ch_operator = false;
		client_spec.fd = client.get_fd();
		m_cl_list[client.get_nick()] = client_spec;

		ChannelWelcomeMessage(client);
	}
}

const std::string &Channel::get_channel_name() const
{
	return (m_name);
}

const std::string &Channel::get_topic() const
{
	return (m_topic);
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

// operator

/*
Then, you have to implement the commands that are specific to channel
operators:
∗ KICK - Eject a client from the channel
∗ INVITE - Invite a client to a channel
∗ TOPIC - Change or view the channel topic
*/
