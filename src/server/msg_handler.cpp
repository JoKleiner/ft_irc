
#include "Server.hpp"
#include "Channel.hpp"

constexpr size_t ct_hash(const char* str)
{
    size_t hash = 5381;
    for (size_t i = 0; str[i] != '\0'; i++)
        hash = hash * 33 ^ static_cast<unsigned char>(str[i]);
    return(hash);
}

void Server::switchi(std::vector<std::string> token)
{
    switch(ct_hash(token[0].c_str()))
    {
        case PASS: Server::pass(token);         break;
        case NICK: Server::nick(token);         break;
        case USER: Server::user(token);         break;
        case QUIT: Server::quit(token);         break;
        case LIST: Server::list(token);         break;
        case JOIN: Server::join(token);         break;
        case PART: Server::part(token);         break;
        case PRIVMSG: Server::privmsg(token);   break;
        // case KICK: Server::kick(token);      break;
        // case INVITE: Server::invite(token);  break;
        // case TOPIC: Server::topic(token);    break;
        // case MODE: Server::mode(token);      break;
        // case PING: Server::ping(token);      break;
        default: sendERRRPL(_clients[_iter], SERVERNAME, "421", token[0] + " :Unknown command"); break;
    }
}

void Server::message_handling(std::string client_mssg)
{
    std::cout << "Client (FD " << _fds[_iter].fd << "): " << client_mssg << std::flush;
    
    auto lines = split(client_mssg, "\r\n");

    for (size_t i = 0; i < lines.size(); i++)
    {
        auto token = token_message(lines[i]);
        if(token.empty())
            continue;
        switchi(token);
    }
}
