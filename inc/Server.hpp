#pragma once

#include "Client.hpp"

class Server
{
  public:
	static void start(int argc, char **argv);
	static void run();

  private:
	static int _sock;
	static std::vector<pollfd> _fds;
	static std::vector<Client> _clients;
	static std::string _password;

	static void serverLoop();
	static void cleanup();

	// Utilities
	static void parseInput(std::string &buffer, Client &client);
};
