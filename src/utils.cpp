
#include "Utils.hpp"

void sendERRRPL(const Client &target, const std::string &prefix, const std::string &command, const std::string &params)
{
	if (std::all_of(command.begin(), command.end(), [](char c) { return std::isdigit(c); }))
		SEND(target.get_fd(), (":" + prefix + " " + command + " " + (target.get_nick().empty() ? "*" : target.get_nick()) + " " + params + "\r\n").c_str());
	else
		SEND(target.get_fd(), (":" + prefix + " " + command + " " + params + "\r\n").c_str());
	std::cout << (":" + prefix + " " + command + " " + (target.get_nick().empty() ? "*" : target.get_nick()) + " " + params + "\r\n").c_str();
}

void sendERRRPL(const int &target, const std::string &prefix, const std::string &command, const std::string &params)
{
	SEND(target, (":" + prefix + " " + command + " " + params + "\r\n").c_str());
	std::cout << (":" + prefix + " " + command + " " + params + "\r\n").c_str();
}

std::vector<std::string> token_message(std::string line)
{
	std::vector<std::string> vec_token;
	std::istringstream ss(line);
	std::string token;

	while (ss >> token)
	{
		if (token[0] == ':')
		{
			token = line.substr(line.find(':') + 1);
			vec_token.push_back(token);
			break;
		}
		else
			vec_token.push_back(token);
	}
	return (vec_token);
}

std::vector<std::string> split(std::string str, std::string cha)
{
	size_t pos = 0;
	size_t comma;
	std::vector<std::string> split;

	while ((comma = str.find(cha, pos)) != std::string::npos)
	{
		std::string line = str.substr(pos, comma - pos);
		pos = comma + 1;
		split.push_back(line);
	}
	std::string line = str.substr(pos, comma - pos);
	split.push_back(line);
	return (split);
}
