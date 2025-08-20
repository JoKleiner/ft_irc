#pragma once

#include "Client.hpp"

class Server
{
  public:
	Server(int argc, char **argv);
	~Server() = default;
	void run();

  private:
	int _sock;
	int _port;
	std::vector<pollfd> _fds;
	std::vector<Client> _clients;
	std::string _password;

	void startServer();
	void serverLoop();
	void cleanup();

	// Utilities
	void parseInput(std::string &buffer, Client &client); // should be in a diiferent parser class
};
