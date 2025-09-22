#include "Utils.hpp"

static std::string wrap(const std::string &s, size_t w)
{
	std::vector<std::string> words;
	std::stringstream sw(s);
	std::string word;
	while ((sw >> word))
		words.push_back(word);

	std::string line;
	std::string lines;
	for (auto &wrd : words)
	{
		if (line.empty())
			line = wrd;
		else if (line.size() + 1 + wrd.size() < w)
			line += " " + wrd;
		else
		{
			lines += line + "\n";
			if (wrd.size() < w)
				line = wrd;
			else
			{
				lines += wrd.substr(0, w) + "\n";
				line = wrd.substr(w);
			}
		}
	}
	lines += line + "\n";

	return lines;
}

static std::vector<std::string> splitIntoParagraphs(const std::string &s)
{
	size_t pos = 0;
	std::string words;

	auto getSpaceType = [](const std::string &s, size_t &pos) -> char
	{
		bool n = false;
		bool retN = false;
		while (isspace(s[pos]))
		{
			if (n == true)
				retN = true;
			if (s[pos] == '\n')
				n = true;
			pos++;
		}
		return (retN ? '\n' : ' ');
	};

	while (pos < s.size())
	{
		std::string word;
		while (pos < s.size() && !isspace(s[pos]))
			word.push_back(s[pos++]);
		word += getSpaceType(s, pos);
		words += word;
	}
	return (split(words, "\n"));
}

std::vector<std::string> getBubble(const std::string &input)
{
	const size_t width = 40;
	std::vector<std::string> splited = splitIntoParagraphs(input);
	std::string temp;
	for (size_t i = 0; i < splited.size(); ++i)
	{
		temp += wrap(splited[i], width);
		temp += "\n";
	}
	temp.pop_back();
	temp.pop_back();

	std::vector<std::string> wrappedLines = split(temp, "\n");

	size_t maxl = 0;
	for (auto &L : wrappedLines)
		maxl = std::max(maxl, L.size());

	std::vector<std::string> output;
	output.push_back(" " + std::string(maxl + 2, '_'));
	for (size_t i = 0; i < wrappedLines.size(); ++i)
	{
		std::string line = (wrappedLines.size() == 1 ? "<" : (i == 0 ? "/" : (i == wrappedLines.size() - 1 ? "\\" : "|")));
		line += " " + wrappedLines[i] + std::string(maxl - wrappedLines[i].size(), ' ') + " ";
		line += (wrappedLines.size() == 1 ? ">" : (i == 0 ? "\\" : (i == wrappedLines.size() - 1 ? "/" : "|")));
		output.push_back(line);
	}
	output.push_back(" " + std::string(maxl + 2, '-'));

	return (output);
}
