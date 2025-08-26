
#pragma once

#include <iostream>

class Client
{
  public:
	Client(int fd);
	Client(const Client &other) = default;
	Client &operator=(const Client &other) = default;
	~Client() = default;

	void set_pw(std::string word, size_t i);
	void set_user(std::vector<std::string> vec_token, size_t i);
	std::string get_user();
	void set_nick(std::vector<std::string> vec_token, size_t i);
	const std::string &get_nick() const;
	const int &get_fd() const;

	bool read_client_message(std::string &client_mssg);

  private:
	bool m_pw;
	int m_fds_num;
	std::string m_user;
	std::string m_nick;
};
