
#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <numeric>

#include "Client.hpp"
#include "Channel.hpp"
#include "Format.hpp"

#define SEND(fd, mssg) send(fd, mssg, strlen(mssg), 0)
#define PASS ct_hash("PASS")
#define NICK ct_hash("NICK")
#define USER ct_hash("USER")
#define QUIT ct_hash("QUIT")
#define LIST ct_hash("LIST")
#define JOIN ct_hash("JOIN")
#define PART ct_hash("PART")
#define PRIVMSG ct_hash("PRIVMSG")
#define KICK ct_hash("KICK")
#define INVITE ct_hash("INVITE")
#define TOPIC ct_hash("TOPIC")
#define MODE ct_hash("MODE")
#define PING ct_hash("PING")

#define SERVERNAME "server.name"

std::vector<std::string> split(std::string str, std::string cha);
std::vector<std::string> token_message(std::string client_mssg);
void sendERRRPL(const Client &target, const std::string &prefix, const std::string &command, const std::string &params = "");

class Channel;

class Server
{
  public:
	static void start(char **argv);
	static void run();

	static bool checkPassword(const std::string &pw);
	static bool checkNickname(const std::string &un);
	static void server_kick(size_t user);

  private:
	static size_t _iter;
	static int _sock;
	static std::vector<pollfd> _fds;
	static std::vector<Client> _clients;
	static std::map<std::string, Channel> _channels;
	static std::string _password;

	// Commands
	static void pass(std::vector<std::string> token);
	static void nick(std::vector<std::string> token);
	static void user(std::vector<std::string> token);
	static void quit(std::vector<std::string> token);
	static void list(std::vector<std::string> token);
	static void join(std::vector<std::string> token);
	static void part(std::vector<std::string> token);
	static void privmsg(std::vector<std::string> token);
	// static void kick(std::vector<std::string> token);
	// static void invite(std::vector<std::string> token);
	// static void topic(std::vector<std::string> token);
	// static void mode(std::vector<std::string> token);
	// static void ping(std::vector<std::string> token);

	// dont know yet
	static void serverLoop();
	static void cleanup();
	// static void send_err();
	static void switchi(std::vector<std::string> token);
	static void create_channel(std::vector<std::string> channel_splits, size_t i, std::vector<std::string> password_splits);
	static void leave_all_channel();
	static void msg_channel(std::string channel, std::string msg);
	static void msg_client(std::string cl_name, std::string msg);

	// Utilities
	static void iter_vec_pfds();
	static void connect_new_client();
	static void client_message();
	static void message_handling(std::string client_mssg);
	static bool check_channel_syntax(std::vector<std::string> channel_splits, size_t i);
	static bool check_privmsg_input(std::vector<std::string> token);
};
