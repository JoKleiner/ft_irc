
#include "Server.hpp"

std::string g_server_password;

Client::Client(int fd) : m_pw(false), m_fds_num(fd), m_user(), m_nick(), m_last_send_time(std::chrono::system_clock::now()), m_joined_time(std::chrono::system_clock::now()), m_ping_send(false)
{
}

void Client::set_pw(const bool &set)
{
	m_pw = set;
}

void Client::set_user(std::string name, std::string namestr)
{
	// USER <user> <mode> <unused> <realname>
	m_user = name;
	m_userstring = namestr;
}

const std::string &Client::get_user() const
{
	return (m_user);
}

void Client::set_nick(const std::string &nick)
{
	m_nick = nick;
}

const std::string &Client::get_nick() const
{
	return (m_nick);
}

bool Client::read_client_message(std::string &client_mssg)
{
	ssize_t bytes = 1;

	while (client_mssg.find('\n') == std::string::npos && bytes > 0)
	{
		char buff[1024] = {0};
		bytes = read(m_fds_num, buff, sizeof(buff) - 1);
		client_mssg = client_mssg + buff;
	}
	if (bytes < 0)
		return (false);
	return (true);
}

const int &Client::get_fd() const
{
	return (m_fds_num);
}

bool Client::registered() const
{
	return (m_pw && !m_nick.empty() && !m_user.empty());
}

const bool &Client::pw_set() const
{
	return (m_pw);
}

void Client::set_ip(const std::string &addr)
{
	m_addr = addr;
}

const std::string &Client::get_addr() const
{
	return (m_addr);
}

const std::string &Client::get_user_whole_str() const
{
	return (m_user_whole_str);
}

void Client::set_user_whole_str(const std::string &str)
{
	m_user_whole_str = str;
}

void Client::set_last_send_time(time_point time)
{
	m_last_send_time = time;
}

const time_point &Client::get_last_send_time() const
{
	return (m_last_send_time);
}

const time_point &Client::get_joined_time() const
{
	return (m_joined_time);
}

const bool &Client::get_ping_send() const
{
	return (m_ping_send);
}

void Client::set_ping_send(const bool &send)
{
	m_ping_send = send;
}