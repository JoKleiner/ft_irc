
#include "Channel.hpp"

void Channel::KickNick(const std::string &nick) {
	m_cl_list.erase(nick);
}

void Channel::InvNick(const std::string &nick) {
	m_invite_list.push_back(nick);
}

void Channel::SetTopic(const std::string &topic) {
	m_topic = topic;
}
