
#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <sstream>

#include "Client.hpp"

extern std::string g_server_password;

void run_irc(int sock);
void message_handling(std::vector<Client> &vec_client, std::vector<pollfd> &vec_pfds, size_t i, std::vector<std::string> vec_token);
