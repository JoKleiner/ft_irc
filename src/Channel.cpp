
#include "Channel.hpp"

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
	m_name = name.substr(1);
	m_cl_list[client.get_nick()] = cur_client;
	std::string out = "Created channel: " + m_name + "\n";
	SEND(client.get_fd(), out.c_str());
}

void Channel::leave(size_t iter)
{
	(void)iter;
	// m_cl_list.erase(m_cl_list.first);
}

void Channel::join(Client client, std::string channel_pw)
{
	if(channel_pw == m_password)
	{
		if(m_cl_list.find(client.get_nick()) != m_cl_list.end())
		{
			std::string out = "Client already in channel: " + this->get_channel_name() + "\n";
			SEND(client.get_fd(), out.c_str());
			return ;
		}
		client_speci client_spec;
		client_spec.channel_creator = false;
		client_spec.rights = "regular";
		m_cl_list[client.get_nick()] = client_spec;

		std::string out = "Joined channel: " + this->get_channel_name() + "\n";
		SEND(client.get_fd(), out.c_str());
	}
	else
	{
		std::string out = "Error: wrong pw for channel: " + this->get_channel_name() + "\n";
		SEND(client.get_fd(), out.c_str());
	}
}

std::string Channel::get_channel_name(){
	return (m_name);
}

void Channel::set_channel_pw(std::string password){
	m_password = password;
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
