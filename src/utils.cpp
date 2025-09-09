
#include "Server.hpp"
#include <algorithm>

bool read_message(std::string &client_mssg, int fds)
{
	ssize_t bytes = 1;

	while (client_mssg.find('\n') == std::string::npos && bytes > 0)
	{
		char buff[1024] = {0};
		bytes = read(fds, buff, sizeof(buff) - 1);
		client_mssg = client_mssg + buff;
	}
	if (bytes < 0)
		return (false);
	return (true);
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

bool check_pw_syntax(std::string line)
{
	for (size_t i = 0; i < line.size(); i++)
		if (!std::isprint(line[i]) || line[i] == ' ')
            return false;
	return true;
}	

void sendERRRPL(const Client &target, const std::string &prefix, const std::string &command, const std::string &params)
{
	if (std::all_of(command.begin(), command.end(), [](char c) { return std::isdigit(c); }))
		SEND(target.get_fd(), (":" + prefix + " " + command + " " + (target.get_nick().empty() ? "*" : target.get_nick()) + " " + params + "\r\n").c_str());
	else
		SEND(target.get_fd(), (":" + prefix + " " + command + " " + params + "\r\n").c_str());
	//std::cout << (":" + prefix + " " + command + " " + (target.get_nick().empty() ? "*" : target.get_nick()) + " " + params + "\r\n").c_str();
}
