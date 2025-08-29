
#pragma once

#include "Client.hpp"
#include <map>
#include <vector>

struct client_speci
{
	bool channel_creator;
	std::string rights;
};

class Channel
{
public:
	Channel(std::string name, std::string client_name);
	~Channel() = default;

	void join(std::vector<std::string> vec_token, std::map<std::string, Channel> _channels);
	std::string get_channel_name();
	void leave(size_t iter);

private:
	std::string m_name;
	std::string m_topic;
	std::string m_mode;
	std::string m_password;
	std::map<std::string, client_speci> m_cl_list;
};
