
#pragma once

#include "Client.hpp"
#include "Server.hpp"
#include <map>
#include <vector>

struct client_speci
{
	int fd;
	bool channel_creator;
	std::string rights;
};

class Channel
{
  public:
	Channel(std::string name, Client client);
	~Channel() = default;

	void join(Client client, std::string channel_pw);
	const std::string &get_channel_name() const;
	const std::string &get_topic() const;
	void set_channel_pw(std::string password);
	void leave_channel(const Client &client, const std::string &msg = "Heute ist nicht alle Tage, ich komm wieder keine Frage.", const std::string &command = "PART");
	const std::map<std::string, client_speci> &get_cha_cl_list() const;
	void broadcast(std::string sender, std::string msg) const;

  private:
	std::string m_name;
	std::string m_topic;
	std::string m_mode;
	std::string m_password;
	std::map<std::string, client_speci> m_cl_list;
	void ChannelWelcomeMessage(const Client &client);
};
