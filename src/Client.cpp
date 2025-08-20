
#include "../inc/Client.hpp"

Client::Client( std::vector<pollfd> fds, size_t i) : m_pw(false), m_fds_num(fds[i].fd){}

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
