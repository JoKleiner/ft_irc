
#include "Channel.hpp"

Channel::Channel(std::string name, std::string client_name)
{
	m_name = name.substr(1);
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
	m_cl_list[client_name] = cur_client;
}

void Channel::leave(size_t iter)
{
	(void)iter;
	// m_cl_list.erase(m_cl_list.first);
}

void Channel::join(std::vector<std::string> vec_token, std::map<std::string, Channel> _channels)
{
	(void)vec_token;
	(void)_channels;
}

std::string Channel::get_channel_name()
{
	return (m_name);
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
