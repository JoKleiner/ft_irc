
#include "Server.hpp"

std::vector<std::string> token_message(std::string line)
{
	std::vector<std::string> vec_token;
	std::istringstream ss(line);
	std::string token;

	while (ss >> token)
		vec_token.push_back(token);
	return(vec_token);
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
