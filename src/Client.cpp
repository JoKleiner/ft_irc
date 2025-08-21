
#include "../inc/irc.hpp"
#include "../inc/Client.hpp"

std::string g_server_password;

Client::Client() : m_pw(false){}

void Client::kick_user(size_t i, std::vector<pollfd> &vec_pfds)
{
	std::cout << "Client disconnected (FD: " << vec_pfds[i].fd << ")" << std::endl;
	close(vec_pfds[i].fd);
	vec_pfds.erase(vec_pfds.begin() + i);
}

void Client::set_pw(std::string input, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client)
{
	if(input.empty() || input != g_server_password)
		vec_client[i].kick_user(i, vec_pfds);
	else
		m_pw = true;
}

void Client::set_user(std::vector<std::string> vec_token, size_t i, std::vector<pollfd> &vec_pfds, std::vector<Client> vec_client)
{
	if(!m_pw)
		vec_client[i].kick_user(i, vec_pfds);
	else
	{
		std::string name;
		for (size_t i = 1; i < vec_token.size(); i++)
		{
			if(!name.empty())
				name += ' ';
			name += vec_token[i];
		}
		m_user = name;
	}
}

std::string Client::get_user() {
	return(m_user);
}

void Client::set_nick()
{

}
