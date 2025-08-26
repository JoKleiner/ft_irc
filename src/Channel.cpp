
#include "Channel.hpp"

Channel::Channel(std::string name, std::string client_name): m_name(name)
{
	client_speci cur_client;
	cur_client.channel_creator = true;
	cur_client.rights = "owner";
	m_cl_list[client_name] = cur_client;
}

void Channel::join(std::vector<std::string> vec_token)
{
	(void)vec_token;
}
