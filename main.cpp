#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <regex>

/*
    TCP server using the POSIX Socket API
*/

int main(int argc, char **argv) {

    if(argc != 3)
        return(std::cerr << "Error: server start with './ircserv <port> <password>'" << std::endl, 1);

    if (!std::regex_match(argv[1], std::regex("(\\d{4}|\\d{5})")))
		return(std::cerr << "Error: Port not in correct Form" << std::endl, 1);
   
    
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
    int sock = socket(AF_INET, SOCK_STREAM, 0);     
    

    int opt = 1;
    /*
        int opt = 1: SO_REUSEADDR should be enabled
        setsockopt() expects 4 bytes. Therefore, use an integer instead of a bool.
        SOL_SOCKET: Category of the option (here: socket level).
        SO_REUSEADDR: Allows restarting the server without waiting for the port to be released.
    */
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    

    // Initialize a structure containing the IP address and port.
    sockaddr_in addr{};                             
    addr.sin_family = AF_INET;                      
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    // INADDR_ANY: listen on all network interfaces (e.g. localhost and external IPs).   
    // htons(8080): Port 8080, converted to network byte order (Big Endian).         


    // Bind the socket to the configured address
    bind(sock, (sockaddr*)&addr, sizeof(addr));


    // Tell the socket to listen with a backlog of "SOMAXCONN" connections. SOMAXCONN = 128 max connections
    listen(sock, SOMAXCONN);

    std::cout << "Server runs. Client access with: 'nc localhost " << port << "', Password: '" << argv[2] << "'" << std::endl;


    std::vector<pollfd> fds;
    fds.push_back({sock, POLLIN, 0});

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
        for (size_t i = 0; i < fds.size(); ++i)
        {
            if (fds[i].revents)
            {
                if (fds[i].fd == sock)
                {
                    int client = accept(sock, nullptr, nullptr);
                    std::cout << "New client connected (FD: " << client << ")\n";
                    fds.push_back({client, POLLIN, 0});
                }
                else
                {
                    char buf[100] = {0};
                    ssize_t bytes = read(fds[i].fd, buf, sizeof(buf) - 1);
                    if (bytes <= 0)
                    {
                        std::cout << "Client disconnected (FD: " << fds[i].fd << ")\n";
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    }
                    else
                    {
                        std::cout << "Client (FD " << fds[i].fd << "): " << buf;
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
