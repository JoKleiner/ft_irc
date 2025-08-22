
#include "irc.hpp"
#include "Client.hpp"

void message_handling(std::vector<Client> &vec_client, std::vector<pollfd> &vec_pfds, size_t i, std::vector<std::string> vec_token)
{
	if (vec_token[0] == "PASS")
		vec_client[i].set_pw(vec_token[1], i, vec_pfds, vec_client);
	if (vec_token[0] == "USER")
		vec_client[i].set_user(vec_token, i, vec_pfds, vec_client);
	if (vec_token[0] == "NICK")
		vec_client[i].set_nick(vec_token, i, vec_pfds, vec_client);
	if (vec_token[0] == "QUIT")
	 	vec_client[i].kick_user(i, vec_pfds);
	// if (vec_token[0] == "JOIN")
	// 	
	// if (vec_token[0] == "PRIVMSG")
	// 	
	// if (vec_token[0] == "PING")
	// 	
}
