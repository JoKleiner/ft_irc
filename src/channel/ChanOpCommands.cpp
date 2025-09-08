
#include "Channel.hpp"

void Channel::KickNick(std::string &nick) {
	m_cl_list.erase(nick);
}

void Channel::InvNick(std::string &nick) {
	m_invite_list.push_back(nick);
}

void Channel::SetTopic(std::string topic) {
	m_topic = topic;
}
