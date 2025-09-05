
#include "Server.hpp"

void Server::kick(std::vector<std::string> &token)
{
	if (token.size() < 4)
		sendERRRPL(_clients[_iter], SERVERNAME, "461", "PASS :Not enough parameters");
	//...
}
