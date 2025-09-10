
#pragma once

#include <algorithm>
#include "Client.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <netinet/in.h>
#include <iomanip>
#include <unistd.h>

#define SERVERNAME "server.name"
#define VERSION "v0.1"
#define RPL_WELCOME ":Welcome to " SERVERNAME " " + _clients[_iter].get_user_whole_str()
#define RPL_YOURHOST ":Your host is " SERVERNAME ", running version " VERSION
#define RPL_CREATED ":This server was created " + []() { std::stringstream n; n << [](){auto t = std::chrono::system_clock::to_time_t(_serverStart); return std::put_time(std::localtime(&t), "%b-%d-%Y");}(); return n.str();}()
#define RPL_MYINFO SERVERNAME " " VERSION " o itkl"

#define SEND(fd, mssg) send(fd, mssg, strlen(mssg), 0)

using time_point = std::chrono::time_point<std::chrono::system_clock>;

std::vector<std::string> token_message(std::string line);
std::vector<std::string> split(std::string str, std::string cha);
void sendERRRPL(const Client &target, const std::string &prefix, const std::string &command, const std::string &params);
void sendERRRPL(const int &target, const std::string &prefix, const std::string &command, const std::string &params);