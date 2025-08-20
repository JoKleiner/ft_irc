
#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <regex>
#include <sstream>
#include <fcntl.h>

class Client
{
  public:
	Client(int fds);
	~Client() = default;

	void check_pw(std::string password, std::string first_word);
	void set_user();
	void set_nick();
	int getFd() const;
	std::string getMessage() const;

  private:
	bool m_pw;
	int m_fds_num;
	std::string m_user;
	std::string m_nick;
};
