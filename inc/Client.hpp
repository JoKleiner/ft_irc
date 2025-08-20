
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

	void check_pw(std::string password, std::string first_word);
	void set_user();
	void set_nick();

  private:
	bool m_pw;
	int m_fds_num;
	std::string m_user;
	std::string m_nick;
};
