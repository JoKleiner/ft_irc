
#pragma once

#include "Client.hpp"
#include "Utils.hpp"
#include <string>
#include <map>
#include <vector>
#include <regex>

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
	const std::string &get_channel_name() const;
	const std::string &get_topic() const;
	const bool &get_topic_op() const;
	void set_channel_pw(std::string password);
	void leave_channel(const Client &client, const std::string &msg = "Heute ist nicht alle Tage, ich komm wieder keine Frage.", const std::string &command = "PART");
	const std::map<std::string, client_speci> &get_cha_cl_list() const;
	void broadcast(std::string sender, std::string msg) const;

	void InviteMode(const std::vector<std::string> &token, Client client);
	void TopicMode(const std::vector<std::string> &token, Client client);
	void KeyMode(const std::vector<std::string> &token, Client client, size_t &mode_count);
	void OperatMode(const std::vector<std::string> &token, Client client, size_t &mode_count);
	void LimitMode(const std::vector<std::string> &token, Client client, size_t &mode_count);
	void send_channel_mode(const std::vector<std::string> &token, Client client, std::string mode);
	void rpl_chan_modi(const std::vector<std::string> &token, Client client);

	void KickNick(const std::string &nick);
	void InvNick(const std::string &nick);
	void SetTopic(const std::string &topic);

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
	size_t m_chan_limit;
};
