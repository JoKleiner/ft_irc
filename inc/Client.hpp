
#pragma once

#include <chrono>
#include <iostream>

using time_point = std::chrono::time_point<std::chrono::system_clock>;

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
	void set_user_whole_str(const std::string &str);
	const std::string &get_user_whole_str() const;
	void set_last_send_time(time_point time = std::chrono::system_clock::now());
	const time_point &get_last_send_time() const;
	const time_point &get_joined_time() const;

	bool read_client_message(std::string &client_mssg);
	void set_ping_send(const bool &send);
	const bool &get_ping_send() const;

  private:
	bool m_pw;
	int m_fds_num;
	std::string m_user;
	std::string m_userstring;
	std::string m_nick;
	std::string m_addr;
	std::string m_user_whole_str;
	time_point m_last_send_time;
	time_point m_joined_time;
	bool m_ping_send;
};
