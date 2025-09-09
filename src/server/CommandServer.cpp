
#include "Server.hpp"

void Server::pass(std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendERRRPL(_clients[_iter], SERVERNAME, "461", "PASS :Not enough parameters");
	else if (_clients[_iter].pw_set())
		sendERRRPL(_clients[_iter], SERVERNAME, "462", ":Unauthorized command (already registered)");
	else if (_password != token[1])
		sendERRRPL(_clients[_iter], SERVERNAME, "464", ":Password incorrect");
	else
		_clients[_iter].set_pw(true);
}

void Server::nick(std::vector<std::string> &token)
{
	if (!_clients[_iter].pw_set() /* || _clients[_iter].get_user().empty() */)
		sendERRRPL(_clients[_iter], SERVERNAME, "451", ":You have not registered");
	else if (token.size() < 2)
		sendERRRPL(_clients[_iter], SERVERNAME, "431", ":No nickname given");
	else if (!std::regex_match(token[1], std::regex(FMT_NICKNAME)))
		sendERRRPL(_clients[_iter], SERVERNAME, "432", token[1] + " :Erroneous nickname");
	else if (!Server::checkNickname(token[1]))
		sendERRRPL(_clients[_iter], SERVERNAME, "433", token[1] + " :Nickname is already in use");
	else
	{
		_clients[_iter].set_nick(token[1]);
		if (_clients[_iter].registered())
		{
			_clients[_iter].set_user_whole_str(_clients[_iter].get_nick() + "!" + _clients[_iter].get_user() + "@" + _clients[_iter].get_addr());
			sendERRRPL(_clients[_iter], SERVERNAME, "001", ":Welcome to the Internet Relay Network " + _clients[_iter].get_user_whole_str());
		}
	}
}

void Server::user(std::vector<std::string> &token)
{
	if (!_clients[_iter].pw_set() /* || _clients[_iter].get_user().empty() */)
		sendERRRPL(_clients[_iter], SERVERNAME, "451", ":You have not registered");
	else if (!_clients[_iter].get_user().empty())
		sendERRRPL(_clients[_iter], SERVERNAME, "462", ":Unauthorized command (already registered)");
	else if (token.size() < 5)
		sendERRRPL(_clients[_iter], SERVERNAME, "461", "PASS :Not enough parameters");
	else
	{
		if (!std::regex_match(token[1], std::regex(FMT_USER)))
			sendERRRPL(_clients[_iter], SERVERNAME, "468", token[1] + " :Invalid username");
		else
		{
			_clients[_iter].set_user(token[1], std::accumulate(std::next(token.begin()), token.end(), std::string(""), [](std::string a, const std::string &b) -> std::string
															   { return a + " " + b; }));
			if (_clients[_iter].registered())
			{
				_clients[_iter].set_user_whole_str(_clients[_iter].get_nick() + "!" + _clients[_iter].get_user() + "@" + _clients[_iter].get_addr());
				sendERRRPL(_clients[_iter], SERVERNAME, "001", ":Welcome to the Internet Relay Network " + _clients[_iter].get_user_whole_str());
			}
		}
	}
}

void Server::quit(std::vector<std::string> &token)
{
	if (token.size() >= 3)
		Server::leave_all_channel(_clients[_iter], "QUIT", token[2]);
	else
		Server::leave_all_channel(_clients[_iter], "QUIT");
	Server::server_kick(_iter);
}

void Server::list(std::vector<std::string> &token)
{
	(void)token;

	for (auto &[name, channel] : _channels)
		sendERRRPL(_clients[_iter], SERVERNAME, "322", name + " # " + std::to_string(channel.get_cha_cl_list().size()) + " :" + channel.get_topic());
	sendERRRPL(_clients[_iter], SERVERNAME, "323", ":End of LIST");
}
