
#pragma once

#include <iostream>

class Client
{
  public:
	Client(int fd);
	Client(const Client &other) = default;
	Client &operator=(const Client &other) = default;
	~Client() = default;

	void set_pw(const bool &set);
	void set_nick(const std::string &nick);
	void set_user(std::string name, std::string namestr);
	void set_ip(const std::string &addr);
	const std::string &get_addr() const;
	const int &get_fd() const;
	const std::string &get_user() const;
	const std::string &get_nick() const;
	bool registered() const;
	const bool &pw_set() const;

	bool read_client_message(std::string &client_mssg);

  private:
	bool m_pw;
	int m_fds_num;
	std::string m_user;
	std::string m_userstring;
	std::string m_nick;
	std::string m_addr;
};
