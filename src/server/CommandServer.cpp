
#include "Server.hpp"

void Server::pass(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendRplErr(_clients[_iter], SERVERNAME, "461", "PASS :Not enough parameters");
	else if (_clients[_iter].pw_set())
		sendRplErr(_clients[_iter], SERVERNAME, "462", ":Unauthorized command (already registered)");
	else if (_password != token[1])
		sendRplErr(_clients[_iter], SERVERNAME, "464", ":Password incorrect");
	else
		_clients[_iter].set_pw(true);
}

void Server::nick(const std::vector<std::string> &token)
{
	if (!_clients[_iter].pw_set())
		sendRplErr(_clients[_iter], SERVERNAME, "451", ":Password required");
	else if (token.size() < 2)
		sendRplErr(_clients[_iter], SERVERNAME, "431", ":No nickname given");
	else if (!std::regex_match(token[1], std::regex(FMT_NICKNAME)))
		sendRplErr(_clients[_iter], SERVERNAME, "432", token[1] + " :Erroneous nickname");
	else if (!Server::checkNickname(token[1]))
		sendRplErr(_clients[_iter], SERVERNAME, "433", token[1] + " :Nickname is already in use");
	else
	{
		if (_clients[_iter].registered())
		{
			for (auto &[name, channel] : _channels)
			{
				if (channel.get_cha_cl_list().count(_clients[_iter].get_nick()))
					channel.broadcast(_clients[_iter].get_user_whole_str(), "NICK", token[1]);
			}
			_clients[_iter].set_nick(token[1]);
		}
		else
		{
			_clients[_iter].set_nick(token[1]);
			if (_clients[_iter].registered())
				Server::welcomeMessage();
		}
	}
}

void Server::user(const std::vector<std::string> &token)
{
	if (!_clients[_iter].pw_set())
		sendRplErr(_clients[_iter], SERVERNAME, "451", ":Password required");
	else if (!_clients[_iter].get_user().empty())
		sendRplErr(_clients[_iter], SERVERNAME, "462", ":Unauthorized command (already registered)");
	else if (token.size() < 5)
		sendRplErr(_clients[_iter], SERVERNAME, "461", "USER :Not enough parameters");
	else
	{
		if (!std::regex_match(token[1], std::regex(FMT_USER)))
			sendRplErr(_clients[_iter], SERVERNAME, "468", token[1] + " :Invalid username");
		else
		{
			_clients[_iter].set_user(token[1], std::accumulate(std::next(token.begin()), token.end(), std::string(""), [](std::string a, const std::string &b) -> std::string { return a + " " + b; }));
			if (_clients[_iter].registered())
				Server::welcomeMessage();
		}
	}
}

void Server::quit(const std::vector<std::string> &token)
{
	if (token.size() >= 3)
		Server::leave_all_channel(_clients[_iter], "QUIT", token[2]);
	else
		Server::leave_all_channel(_clients[_iter], "QUIT");
	Server::server_kick(_iter);
}

void Server::list(const std::vector<std::string> &token)
{
	(void)token;

	for (auto &[name, channel] : _channels)
		sendRplErr(_clients[_iter], SERVERNAME, "322", name + " # " + std::to_string(channel.get_cha_cl_list().size()) + " :" + channel.get_topic());
	sendRplErr(_clients[_iter], SERVERNAME, "323", ":End of LIST");
}

void Server::ping(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendRplErr(_clients[_iter], SERVERNAME, "409", ":No origin specified");
	else
		sendRplErr(_clients[_iter], SERVERNAME, "PONG", ":" + token[1]);
}

void Server::pong(const std::vector<std::string> &token)
{
	if (token.size() < 2)
		sendRplErr(_clients[_iter], SERVERNAME, "409", ":No origin specified");
}

void Server::sendPing(Client &client)
{
	client.set_ping_send(true);
	sendRplErr(client, SERVERNAME, "PING", ":" SERVERNAME);
}
