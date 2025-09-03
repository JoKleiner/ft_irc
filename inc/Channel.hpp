
#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include <map>
#include <vector>

struct client_speci
{
	bool ch_operator;
	std::string rights;
};

class Channel
{
  public:
	Channel(std::string name, Client client);
	~Channel() = default;

	void join(Client client, std::string channel_pw);
	std::string get_channel_name();
	void leave(size_t iter);
	void set_channel_pw(std::string password);
	void leave_channel(Client client);
	std::map<std::string, client_speci> get_cha_cl_list();

	void InviteMode(std::vector<std::string> token, Client client);
	void TopicMode(std::vector<std::string> token, Client client);
	void KeyMode(std::vector<std::string> token, Client client);
	void OperatMode(std::vector<std::string> token, Client client);
	void LimitMode(std::vector<std::string> token, Client client);

  private:
	std::string m_name;
	std::string m_topic;
	std::string m_mode;
	std::string m_password;
	std::map<std::string, client_speci> m_cl_list;
	bool m_invite_only;
	std::vector<std::string> m_invite_list;
};
