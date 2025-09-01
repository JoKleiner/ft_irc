
#include "Server.hpp"
#include "Channel.hpp"

constexpr unsigned int ct_hash(const char* str)
{
    unsigned int hash = 5381;
    for (size_t i = 0; str[i] != '\0'; i++)
        hash = hash * 33 ^ static_cast<unsigned char>(str[i]);
    return(hash);
}

void Server::switchi(std::vector<std::string> token)
{
    switch(ct_hash(token[0].c_str()))
        {
            case PASS: _clients[_iter].set_pw(token[1], _iter); break;
            case NICK: _clients[_iter].set_nick(token, _iter);  break;
            case USER: _clients[_iter].set_user(token, _iter);  break;
            case QUIT: Server::kick_user(_iter);                break;
            case LIST: Server::send_channel_list(_iter);        break;
            case JOIN: Server::channel_join_reqest(token);      break;
            // case ct_hash("PART"):  xxx; break;
            // case ct_hash("PRIVMSG"): xxx;  break;
            // case ct_hash("PING"):  xxx; break;
            default: SEND(_fds[_iter].fd, "421\n"); break;
        }
}

void Server::message_handling(std::string client_mssg)
{
    std::cout << "Client (FD " << _fds[_iter].fd << "): " << client_mssg << std::flush;
	SEND(_fds[_iter].fd, "Message received\n");
    
    auto lines = split(client_mssg, "\r\n");

    for (size_t i = 0; i < lines.size(); i++)
    {
        auto token = token_message(lines[i]);
        if(token.empty())
            continue;
        switchi(token);
    }
}
