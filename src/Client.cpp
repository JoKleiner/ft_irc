
#include "../inc/Client.hpp"

Client::Client( std::vector<pollfd> fds, size_t i) : m_pw(false), m_fds_num(fds[i].fd){}

void Client::kick_user(size_t i, std::vector<pollfd> &fds)
{
	std::cout << "Client disconnected (FD: " << fds[i].fd << ")\n";
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
}

void Client::check_pw(std::string password, std::string word, size_t i, std::vector<pollfd> &fds, std::vector<Client> clients)
{
	if(password == word)
		m_pw = true;
	else
		clients[i].kick_user(i, fds);
}

void Client::set_user()
{

}

void Client::set_nick()
{

}
