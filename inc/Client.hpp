
#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <regex>
#include <sstream>

class Client
{
  public:
	Client( std::vector<pollfd> fds, size_t i);
	~Client() = default;

	void check_pw(std::string password, std::string word, size_t i, std::vector<pollfd> &fds, std::vector<Client> clients);
	void set_user();
	void set_nick();
	void kick_user(size_t i, std::vector<pollfd> &fds);

  private:
	bool m_pw;
	int m_fds_num;
	std::string m_user;
	std::string m_nick;
};
