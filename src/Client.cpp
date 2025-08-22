
#include "irc.hpp"

std::string g_server_password;

Client::Client() : m_pw(false) {}

void Client::kick_user(size_t i, std::vector<pollfd> &vec_pfds)
{
	std::cout << "Client disconnected (FD: " << vec_pfds[i].fd << ")" << std::endl;
	close(vec_pfds[i].fd);
	vec_pfds.erase(vec_pfds.begin() + i);
}

void Client::set_pw(std::string input, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client)
{
	if (input.empty() || input != g_server_password)
	{
		vec_client[i].kick_user(i, vec_pfds);
		send(vec_pfds[i].fd, "ERROR :Password incorrect\n", sizeof("ERROR :Password incorrect\n"), 0);
	}
	else
		m_pw = true;
}

void Client::set_user(std::vector<std::string> vec_token, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client)
{
	//USER <username> <hostname> <servername> :<realname>
	if (!m_pw)
		vec_client[i].kick_user(i, vec_pfds);
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

void Client::set_nick(std::vector<std::string> vec_token, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client)
{
	if (!m_pw)
		vec_client[i].kick_user(i, vec_pfds);
	else
	{
		std::string name;
		for(size_t u = 1; u < vec_token.size(); u++)
		{
			if (!name.empty())
				name += ' ';
			name += vec_token[u];
		}
		for(size_t v = 0; v < vec_client.size(); v++)
		{
			if(name == vec_client[v].m_nick)
			{
				send(vec_pfds[i].fd, "ERROR :Nick not available\n", sizeof("ERROR :Nick not available\n"), 0);
				vec_client[i].kick_user(i, vec_pfds);
				return ;
			}
		}
		m_nick = name;
	}
}

std::string Client::get_nick(){
	return (m_nick);
}
