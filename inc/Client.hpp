
#pragma once

#include "irc.hpp"

class Client
{
  public:
	Client();
	Client(const Client &other) = default;
	Client &operator=(const Client &other) = default;
	~Client() = default;

	void set_pw(std::string word, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client);
	void set_user(std::vector<std::string> vec_token, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client);
	std::string get_user();
	void set_nick();
	void kick_user(size_t i, std::vector<pollfd> &vec_pfds);

  private:
	bool m_pw;
	std::string m_user;
	std::string m_nick;
};
