
#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <sstream>

extern std::string g_server_password;

void run_irc(int sock);
