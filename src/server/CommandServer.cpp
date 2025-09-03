#include "Server.hpp"

void Server::pass(std::vector<std::string> token)
{
	if (token.size() < 2)
		SEND(_clients[_iter].get_fd(), "461 PASS: :Not enough parameters\n\r");
	else if (_clients[_iter].pw_set())
		SEND(_clients[_iter].get_fd(), "462 :Unauthorized command (already registered)\n\r");
	else if (!Server::checkPassword(token[1]))
		SEND(_clients[_iter].get_fd(), "464 :Password incorrect\n\r");
	else
		_clients[_iter].set_pw(true);
}

void Server::nick(std::vector<std::string> token)
{
	if (!_clients[_iter].pw_set() /* || _clients[_iter].get_user().empty() */)
		SEND(_clients[_iter].get_fd(), "451 :You have not registered\n\r");
	else if (token.size() < 2)
		SEND(_clients[_iter].get_fd(), "431 :No nickname given\n\r");
	else if (!std::regex_match(token[1], std::regex(FMT_NICKNAME)))
		SEND(_clients[_iter].get_fd(), ("432" + token[1] + " :Erroneous nickname\n\r").c_str());
	else if (!Server::checkNickname(token[1]))
		SEND(_clients[_iter].get_fd(), ("433" + token[1] + " :Nickname is already in use\n\r").c_str());
	else
	{
		_clients[_iter].set_nick(token[1]);
		if (_clients[_iter].registered())
			SEND(_clients[_iter].get_fd(), ("001 Welcome to the Internet Relay Network " + _clients[_iter].get_nick() + "!" + _clients[_iter].get_user() + "@" + _clients[_iter].get_addr() + "\n\r").c_str());
	}
}

void Server::user(std::vector<std::string> token)
{
	if (!_clients[_iter].pw_set() /* || _clients[_iter].get_user().empty() */)
		SEND(_clients[_iter].get_fd(), "451 :You have not registered\n\r");
	else if (!_clients[_iter].get_user().empty())
		SEND(_clients[_iter].get_fd(), "462 :Unauthorized command (already registered)\n\r");
	else if (token.size() < 5)
		SEND(_clients[_iter].get_fd(), "461 PASS: :Not enough parameters\n\r");
	else
	{
		if (!std::regex_match(token[1], std::regex(FMT_USER)))
			SEND(_clients[_iter].get_fd(), ("468 " + token[1] + ":Invalid username\n\r").c_str());
		else
		{
			_clients[_iter].set_user(token[1], std::accumulate(std::next(token.begin()), token.end(), std::string(""), [](std::string a, const std::string &b) -> std::string { return a + " " + b; }));
			if (_clients[_iter].registered())
				SEND(_clients[_iter].get_fd(), ("001 Welcome to the Internet Relay Network " + _clients[_iter].get_nick() + "!" + _clients[_iter].get_user() + "@" + _clients[_iter].get_addr() + "\n\r").c_str());
		}
	}
}

void Server::quit(std::vector<std::string> token)
{
	Server::server_kick(_iter);
	std::string msg = std::accumulate(std::next(token.begin()), token.end(), std::string(""), [](std::string a, const std::string &b) -> std::string { return a + " " + b; });
	// broadcast(_clients[iter].get_nick() + "!" + _clients[iter].get_user() + "@" + _clients[iter].get_addr() + "QUIT :" + msg);
}

void Server::list(std::vector<std::string> token)
{
	(void)token;
	std::string msg = "\nList of open channels:\n";
	for (auto chan : _channels)
	{
		msg = msg + chan.second.get_channel_name() + "\n";
	}
	msg = msg + "\n";
	SEND(_clients[_iter].get_fd(), msg.c_str());
}

