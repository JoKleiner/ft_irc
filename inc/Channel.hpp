
#pragma once

#include "Client.hpp"
#include "Server.hpp"
#include <map>
#include <vector>

struct client_speci
{
	int fd;
	bool ch_operator;
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
	void broadcast(std::string sender, std::string msg) const;

	void InviteMode(std::vector<std::string> token, Client client, size_t i);
	void TopicMode(std::vector<std::string> token, Client client, size_t i);
	void KeyMode(std::vector<std::string> token, Client client, size_t i);
	void OperatMode(std::vector<std::string> token, Client client, size_t i);
	void LimitMode(std::vector<std::string> token, Client client, size_t i);
	void send_channel_mode(std::vector<std::string> token, Client client, size_t i);

  private:
	std::string m_name;
	bool m_topic_operat;
	std::string m_topic;
	std::string m_mode;
	std::string m_password;
	std::map<std::string, client_speci> m_cl_list;
	void ChannelWelcomeMessage(const Client &client);
	bool m_invite_only;
	std::vector<std::string> m_invite_list;
};
