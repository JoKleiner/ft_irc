
#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <sstream>

#include "Client.hpp"
#include "Channel.hpp"

class Server
{
  public:
	static void start(char **argv);
	static void run();

	static bool checkPassword(const std::string &pw);
	static bool checkUsername(const std::string &un);
	static void kick_user(size_t user);

  private:
  	static size_t _iter;
	static int _sock;
	static std::vector<pollfd> _fds;
	static std::vector<Client> _clients;
	static std::vector<Channel> _channels;
	static std::string _password;

	static void serverLoop();
	static void cleanup();

	// Utilities
	static void iter_vec_pfds();
	static void connect_new_client();
	static void client_message();
	static void message_handling(std::vector<std::string> vec_token);
};
