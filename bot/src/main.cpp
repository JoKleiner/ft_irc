#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main(int argc, char **argv) {
	// 1. Connect to the server with the IP and port passed
	if (argc != 5) {
		std::cerr << "Usage: " << argv[0] << " <IP> <port> <password> <channel>" << std::endl;
		return 1;
	}
	std::string ip = argv[1];
	int port = std::stoi(argv[2]);
	std::string password = argv[3];
	std::string password_command = std::string("PASS ").append(password).append("\r\n");
	std::string channel = std::string("#").append(argv[4]);

	// Establish socket connection
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	// 2. Authenticate using the provided password
	send(clientSocket, password_command.c_str(), password_command.length(), 0);
	send(clientSocket, "NICK bot\r\n", 10, 0);
	send(clientSocket, "USER bot bot bot bot bot\r\n", 26, 0);
	
	// 3. Join the awesome bot channel
	std::string join_command = std::string("JOIN ").append(channel).append("\r\n");
	send(clientSocket, join_command.c_str(), join_command.length(), 0);

	// 4. Listen for messages and respond to commands
	char buffer[512];
	std::string privmsg_channel = std::string("PRIVMSG ").append(channel).append(" :");
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived <= 0) {
			std::cerr << "Connection closed or error occurred" << std::endl;
			break;
		}
		buffer[bytesReceived] = '\0';
		std::cout << "Received: " << buffer << std::endl;
		std::string message(buffer);

		// Respond to the hello command
		if (message.find("!hello") != std::string::npos) {
			std::string response = privmsg_channel + "Hello, I am a bot!\r\n";
			send(clientSocket, response.c_str(), response.length(), 0);
		}

		// Time command
		if (message.find("!time") != std::string::npos) {
			time_t now = time(0);
			std::string time_str = ctime(&now);
			std::string response = privmsg_channel + "Current time is " + time_str + "\r\n";
			send(clientSocket, response.c_str(), response.length(), 0);
		}

		// And a command for quitting the bot
		if (message.find("!quit") != std::string::npos) {
			std::string response = privmsg_channel + "Goodbye!\r\n";
			send(clientSocket, response.c_str(), response.length(), 0);
			break;
		}
	}

	// Clean up the socket stuff
	close(clientSocket);

	std::cout << "Bot exited" << std::endl;
	return 0;
}
