
#include "Channel.hpp"

void Channel::ChannelWelcomeMessage(const Client &client)
{
	sendERRRPL(client, client.get_user_whole_str(), "JOIN", ":" + get_channel_name());
	if (m_topic.empty())
		sendERRRPL(client, SERVERNAME, "331", this->get_channel_name() + " :No topic set");
	else
		sendERRRPL(client, SERVERNAME, "332", this->get_channel_name() + " :" + m_topic);
	std::string msg = "=" + get_channel_name() + ":";
	for (auto &[name, sp] : m_cl_list)
		msg += (sp.rights == "operators" ? "@" : "+") + name + " ";
	msg.pop_back();
	sendERRRPL(client, SERVERNAME, "353", msg);
	sendERRRPL(client, SERVERNAME, "366", get_channel_name() + " :End of NAMES list");
}

Channel::Channel(std::string name, Client client)
{
	client_speci cur_client;
	if (name[0] == '+')
	{
		cur_client.channel_creator = false;
		cur_client.rights = "regular";
	}
	else
	{
		cur_client.channel_creator = true;
		cur_client.rights = "operators";
	}
	cur_client.fd = client.get_fd();
	m_name = name;
	m_cl_list[client.get_nick()] = cur_client;
	ChannelWelcomeMessage(client);
}

std::map<std::string, client_speci> Channel::get_cha_cl_list()
{
	return (m_cl_list);
}

void Channel::leave_channel(Client client)
{
	if (m_cl_list.find(client.get_nick()) != m_cl_list.end())
	{
		std::string out = "You left channel: " + this->get_channel_name() + "\r\n";

		m_cl_list.erase(client.get_nick());
		SEND(client.get_fd(), out.c_str());
	}
	else
		sendERRRPL(client, SERVERNAME, "442", get_channel_name() + " :You're not on that channel");
}

void Channel::join(Client client, std::string channel_pw)
{
	if (!client.registered())
		sendERRRPL(client, SERVERNAME, "451", ":You have not registered");
	else if (channel_pw == m_password)
	{
		if (m_cl_list.find(client.get_nick()) != m_cl_list.end())
		{
			// technicaly we have to ignore that silently
			std::string out = "Client already in channel: " + this->get_channel_name() + "\r\n";
			SEND(client.get_fd(), out.c_str());
			return;
		}
		client_speci client_spec;
		client_spec.channel_creator = false;
		client_spec.rights = "regular";
		m_cl_list[client.get_nick()] = client_spec;

		ChannelWelcomeMessage(client);
	}
	else
		sendERRRPL(client, SERVERNAME, "464", ":Password incorrect");
}

std::string Channel::get_channel_name()
{
	return (m_name);
}

void Channel::set_channel_pw(std::string password)
{
	m_password = password;
}

void Channel::broadcast(std::string sender, std::string msg) const
{
	for (auto &[name, specification] : m_cl_list)
	{
		std::cout << name << std::endl;
		if (name != sender)
		{
			SEND(specification.fd, msg.c_str());
			std::cout << "send to: " << name << " with " << specification.fd << std::endl;
		}
	}
}

// operator

/*
Then, you have to implement the commands that are specific to channel
operators:
∗ KICK - Eject a client from the channel
∗ INVITE - Invite a client to a channel
∗ TOPIC - Change or view the channel topic
∗ MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel
*/
