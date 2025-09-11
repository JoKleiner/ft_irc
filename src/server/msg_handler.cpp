
#include "Server.hpp"

constexpr size_t ct_hash(const char* str)
{
    size_t hash = 5381;
    for (size_t i = 0; str[i] != '\0'; i++)
        hash = hash * 33 ^ static_cast<unsigned char>(str[i]);
    return(hash);
}

void Server::find_command(const std::vector<std::string> &token)
{
    _clients[_iter].set_last_send_time();
    switch(ct_hash(token[0].c_str()))
    {
        case PASS:    pass(token);      break;
        case NICK:    nick(token);      break;
        case USER:    user(token);      break;
        case QUIT:    quit(token);      break;
        case LIST:    list(token);      break;
        case JOIN:    join(token);      break;
        case PART:    part(token);      break;
        case PRIVMSG: privmsg(token);   break;
        case KICK:    kick(token);      break;
        case INVITE:  invite(token);    break;
        case TOPIC:   topic(token);     break;
        case MODE:    mode(token);      break;
        case PING:    ping(token);      break;
        case PONG:    pong(token);      break;
        default: sendRplErr(_clients[_iter], SERVERNAME, "421", token[0] + " :Unknown command"); break;
    }
}

void Server::message_handling(const std::string &client_mssg)
{
    std::cout << "Client (FD " << _fds[_iter].fd << "): " << client_mssg << std::flush;
    
    auto lines = split(client_mssg, "\r\n");

    for (size_t i = 0; i < lines.size(); i++)
    {
        auto token = token_message(lines[i]);
        if(token.empty())
            continue;
        auto hash_check = ct_hash(token[0].c_str());
        if (!_clients[_iter].registered() && hash_check != PASS && hash_check != NICK && hash_check != USER)
        {
            sendRplErr(_clients[_iter], SERVERNAME, "451", ":You have not registered");
            continue;
        }
        find_command(token);
    }
}
