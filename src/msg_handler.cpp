
#include "Server.hpp"
#include "Channel.hpp"

void Server::message_handling(std::vector<std::string> vec_token)
{
	if (vec_token[0] == "PASS")
		_clients[_iter].set_pw(vec_token[1], _iter);
	if (vec_token[0] == "USER")
		_clients[_iter].set_user(vec_token, _iter);
	if (vec_token[0] == "NICK")
		_clients[_iter].set_nick(vec_token, _iter);
	if (vec_token[0] == "QUIT")
		Server::kick_user(_iter);
	if (vec_token[0] == "JOIN")
		Channel::join(vec_token);
	// if (vec_token[0] == "PRIVMSG")
	// 	Channel::privmsg();
	// if (vec_token[0] == "PING")
	// 	Channel::ping();
	// if (vec_token[0] == "LIST")
	//
	
}
