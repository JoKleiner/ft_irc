
#include "../inc/irc.hpp"
#include <regex>

/*
	socket can accept connections over the network.
    AF_INET: IPv4

    int opt = 1: SO_REUSEADDR should be enabled
    setsockopt() expects 4 bytes. Therefore, use an integer instead of a bool.
    SOL_SOCKET: Category of the option (here: socket level).
    SO_REUSEADDR: Allows restarting the server without waiting for the port to be released.

	INADDR_ANY: listen on all network interfaces (e.g. localhost and external IPs).   
    htons(8080): Port 8080, converted to network byte order (Big Endian).
	
	sockaddr: Initialize a structure containing the IP address and port.

	Bind: bind the socket to the configured address

	listen: Tell the socket to listen with a backlog of "SOMAXCONN" connections. SOMAXCONN = 128 max connections
*/

static bool init_server(char **argv, int &sock)
{
    int port;

    try
    {
        port = std::stoi(argv[1]);
        if(port < 1024 || port > 49151)
            throw (std::runtime_error("Error: Port should be in between 1024-49151"));
    }
    catch(const std::exception &e) {
        return(std::cerr << e.what() << std::endl, false);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};                             
    addr.sin_family = AF_INET;                      
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);      

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
        return(close(sock), std::cerr << "Error: bind" << std::endl, false);
        
    listen(sock, SOMAXCONN);

    std::cout << "Server runs. Client access with: 'nc localhost " << port << "', Password: '" << argv[2] << "'" << std::endl;
    return(true);
}


int main(int argc, char **argv)
{
    if(argc != 3)
        return(std::cerr << "Error: starting server: './ircserv <port> <password>'" << std::endl, 1);

    if (!std::regex_match(argv[1], std::regex("(\\d{4}|\\d{5})")))
		return(std::cerr << "Error: Port should be in between 1024-49151" << std::endl, 1);

    int sock;
    if(!init_server(argv, sock))
        return(1);

    g_server_password = argv[2];
    run_irc(sock);
    close(sock);
}
