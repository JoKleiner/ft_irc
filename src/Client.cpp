#include "../inc/Client.hpp"

Client::Client(int fd) : m_pw(false), m_fds_num(fd){}

void Client::check_pw(std::string password, std::string word)
{
	if(password == word)
		m_pw = true;
	else
		m_pw = false;
}

void Client::set_user()
{

}

void Client::set_nick()
{

}

int Client::getFd() const {
	return (m_fds_num);
}

std::string Client::getMessage() const
{
	std::string buffer;
	ssize_t bytes = 1;
	while (buffer.find('\n') == std::string::npos && bytes > 0)
	{
		char buff[5] = {0};
		if ((bytes = read(m_fds_num, buff, sizeof(buff) - 1)) < 0)
			throw std::runtime_error("Error: read failed");
		buffer = buffer + buff;
	}
	return (buffer);
}