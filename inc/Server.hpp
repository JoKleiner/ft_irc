
#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <map>
#include <sstream>

#include "Client.hpp"
#include "Channel.hpp"

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

std::vector<std::string> split(std::string str, std::string cha);
std::vector<std::string> token_message(std::string client_mssg);

class Channel;

class Server
{
public:
	static void start(char **argv);
	static void run();

	static bool checkPassword(const std::string &pw);
	static bool checkUsername(const std::string &un);
	static void kick_user(size_t user);
	static void send_channel_list(size_t iter);
	static void channel_join_reqest(std::vector<std::string> token);
	static void leave_channel(std::vector<std::string> token);
	static void create_channel(std::vector<std::string> channel_splits, size_t i, std::vector<std::string> password_splits);
	static bool check_channel_syntax(std::vector<std::string> channel_splits, size_t i);

private:
	static size_t _iter;
	static int _sock;
	static std::vector<pollfd> _fds;
	static std::vector<Client> _clients;
	static std::map<std::string, Channel> _channels;
	static std::string _password;

	static void serverLoop();
	static void cleanup();
	static void leave_all_channel();
	static void switchi(std::vector<std::string> token);
	static void send_err();

	// Utilities
	static void iter_vec_pfds();
	static void connect_new_client();
	static void client_message();
	static void message_handling(std::string client_mssg);
};
