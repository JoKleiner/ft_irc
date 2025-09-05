
#include "Server.hpp"

int main(int argc, char **argv)
{
    if(argc != 3)
        return(std::cerr << "Error: starting server: './ircserv <port> <password>'" << std::endl, 1);

    if (!std::regex_match(argv[1], std::regex("(\\d{4}|\\d{5})")))
		return(std::cerr << "Error: Port should be in between 1024-49151" << std::endl, 1);

    try
    {
        Server::start(argv);
        Server::run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
