
#include "Server.hpp"

std::string g_server_password;

Client::Client(int fd) : m_pw(false), m_fds_num(fd){}

void Client::set_pw(std::string input, size_t i)
{
	if (!Server::checkPassword(input))
	{
		send(m_fds_num, "ERROR :Password incorrect\n", sizeof("ERROR :Password incorrect\n"), 0);
		Server::kick_user(i);
	}
	else
		m_pw = true;
}

void Client::set_user(std::vector<std::string> vec_token, size_t i)
{
	//USER <username> <hostname> <servername> :<realname>
	if (!m_pw)
		Server::kick_user(i);
	else
	{
		std::string name;
		for (size_t i = 1; i < vec_token.size(); i++)
		{
			if (!name.empty())
				name += ' ';
			name += vec_token[i];
		}
		m_user = name;
	}
}

std::string Client::get_user(){
	return (m_user);
}

void Client::set_nick(std::vector<std::string> vec_token, size_t i)
{
	if (!m_pw)
		Server::kick_user(i);
	else
	{
		std::string name;
		for(size_t u = 1; u < vec_token.size(); u++)
		{
			if (!name.empty())
				name += ' ';
			name += vec_token[u];
		}
		if(!Server::checkUsername(name))
		{
			send(m_fds_num, "ERROR :Nick not available\n", sizeof("ERROR :Nick not available\n"), 0);
			Server::kick_user(i);
			return ;
		}
		m_nick = name;
	}
}

const std::string &Client::get_nick() const {
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
	if(bytes < 0)
		return (false);
	return (true);
}

const int &Client::get_fd() const {
	return (m_fds_num);
}