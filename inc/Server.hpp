
#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include "Format.hpp"
#include "Utils.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <numeric>
#include <poll.h>
#include <regex>
#include <sstream>
#include <unistd.h>
#include <vector>

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
#define PONG ct_hash("PONG")

bool read_message(std::string &client_mssg, int fds);
std::vector<std::string> split(std::string str, std::string cha);
std::vector<std::string> token_message(std::string client_mssg);
bool check_pw_syntax(std::string line);

class Channel;

class Server
{
  public:
	static void start(char **argv);
	static void run();

	static bool checkNickname(const std::string &un);
	static void server_kick(size_t user, const std::string &reason = "");

  private:
	static size_t _iter;
	static int _sock;
	static std::vector<pollfd> _fds;
	static std::vector<Client> _clients;
	static std::map<std::string, Channel> _channels;
	static std::string _password;
	static time_point _serverStart;

	// Commands
	static void pass(const std::vector<std::string> &token);
	static void nick(const std::vector<std::string> &token);
	static void user(const std::vector<std::string> &token);
	static void quit(const std::vector<std::string> &token);
	static void list(const std::vector<std::string> &token);
	static void join(const std::vector<std::string> &token);
	static void part(const std::vector<std::string> &token);
	static void privmsg(const std::vector<std::string> &token);
	static void kick(const std::vector<std::string> &token);
	static void invite(const std::vector<std::string> &token);
	static void topic(const std::vector<std::string> &token);
	static void mode(const std::vector<std::string> &token);
	static void ping(const std::vector<std::string> &token);
	static void pong(const std::vector<std::string> &token);

	// dont know yet
	static void serverLoop();
	static void cleanup();
	// static void send_err();
	static void find_command(const std::vector<std::string> &token);
	static void create_channel(const std::string &channel, const std::string &channel_pw);
	static void leave_all_channel(const Client &client, const std::string &command = "PART", const std::string &msg = "Heute ist nicht alle Tage, ich komm wieder keine Frage.");
	static void msg_channel(const std::string &channel, const std::string &msg);
	static void msg_client(const std::string &cl_name, const std::string &msg);

	// Utilities
	static void iter_vec_pfds();
	static void connect_new_client();
	static void client_message();
	static void message_handling(const std::string &client_mssg);
	static bool check_channel_syntax(const std::string &channel);
	static bool check_privmsg_input(const std::vector<std::string> &token);
	static bool check_mode_input(const std::vector<std::string> &token);
	static bool check_ChaOpCo_input(const std::vector<std::string> &token);
	static bool check_kick(std::string &channel_splits, std::string &nick_splits);
	static void checkActivity();
	static void welcomeMessage();
	static void sendPing(Client &client);
};
