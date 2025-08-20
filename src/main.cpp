
#include "../inc/Client.hpp"

int start_server(char **argv, int *sock)
{
    int port;

    try
    {
        port = std::stoi(argv[1]);
        if(port < 1024 || port > 49151)
            throw (std::runtime_error("Port should be in between 1024-49151"));
    }
    catch(const std::exception &e)
    {
        return(std::cerr << e.what() << std::endl, 1);
    }

    // Create socket (File Descriptor): This socket can accept connections over the network.
    *sock = socket(AF_INET, SOCK_STREAM, 0);     
    
    /*
        int opt = 1: SO_REUSEADDR should be enabled
        setsockopt() expects 4 bytes. Therefore, use an integer instead of a bool.
        SOL_SOCKET: Category of the option (here: socket level).
        SO_REUSEADDR: Allows restarting the server without waiting for the port to be released.
    */
    int opt = 1;
    setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    

    // Initialize a structure containing the IP address and port.
    sockaddr_in addr{};                             
    addr.sin_family = AF_INET;                      
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    // INADDR_ANY: listen on all network interfaces (e.g. localhost and external IPs).   
    // htons(8080): Port 8080, converted to network byte order (Big Endian).         

    // Bind the socket to the configured address
    bind(*sock, (sockaddr*)&addr, sizeof(addr));

    // Tell the socket to listen with a backlog of "SOMAXCONN" connections. SOMAXCONN = 128 max connections
    listen(*sock, SOMAXCONN);

    std::cout << "Server runs. Client access with: 'nc localhost " << port << "', Password: '" << argv[2] << "'" << std::endl;
    return(0);
}


int main(int argc, char **argv) {

    if(argc != 3)
        return(std::cerr << "Error: server start with './ircserv <port> <password>'" << std::endl, 1);

    if (!std::regex_match(argv[1], std::regex("(\\d{4}|\\d{5})")))
		return(std::cerr << "Error: Port not in correct Form" << std::endl, 1);

    int sock;
    if(start_server(argv, &sock))
        return(1);

    std::vector<pollfd> fds;
    fds.push_back({sock, POLLIN, 0});

    std::vector<Client> clients;

    while (true)
    {
        // Wait for events in fds (new client connect or message received)
        int ret = poll(fds.data(), fds.size(), 0); // -1 = no timeout
        if (ret < 0) {
            std::cerr << "Error in poll()\n";
            break;
        }
        if(ret == 0) // ret = 0, when no change in fds
            continue;

        // Iterate through all file descriptors
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents)
            {
                if (fds[i].fd == sock)
                {
                    int client = accept(sock, nullptr, nullptr);
                    std::cout << "New client connected (FD: " << client << ")\n";
                    fds.push_back({client, POLLIN, 0});
                    Client client_class(fds, i);
                    clients.push_back(client_class);
                }
                else
                {
                    ssize_t bytes = 1;
                    std::string buffer;
                    
                    while (buffer.find('\n') == std::string::npos && bytes > 0)
                    {
                        char buff[5] = {0};
                        bytes = read(fds[i].fd, buff, sizeof(buff) - 1);
                        buffer = buffer + buff;
                    }

                    if (buffer.find('\n') == std::string::npos)
                    {
                        clients[i].kick_user(i, fds);
                        i--;
                    }
                    else
                    {
                        std::string word;
                        std::istringstream ss(buffer);
                        std::getline(ss, word, ' ');
                        if(word.back() == '\n')
                            word.pop_back();
                        if(word == "PASS")
                        {
                            std::getline(ss, word, ' ');
                            if(word.empty())
                                clients[i].kick_user(i, fds);
                            else
                            {
                                if(word.back() == '\n')
                                    word.pop_back();
                                clients[i].check_pw(argv[2], word, i, fds, clients);
                            }
                        }
                        if(word == "USER")
                            clients[i].set_user();
                        if(word == "NICK")
                            clients[i].set_nick();
                        
                        std::cout << "Client (FD " << fds[i].fd << "): " << buffer << std::flush;
                        send(fds[i].fd, "Message received\n", sizeof("Message received\n"), 0);
                    }
                }
            }
        }
    }
    for (auto& fd : fds) {
        if (fd.fd != -1)
            close(fd.fd);
    }
    close(sock);
    return 0;
}
