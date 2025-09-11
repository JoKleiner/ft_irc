
#include "Server.hpp"

int Server::_sock = 0;
size_t Server::_iter = 0;
std::string Server::_password = "";
std::vector<pollfd> Server::_fds;
std::vector<Client> Server::_clients;
std::map<std::string, Channel> Server::_channels;
time_point Server::_serverStart;

void Server::client_message()
{
	std::string client_mssg;
	if (!read_message(client_mssg, _fds[_iter].fd))
		return ;

	if (client_mssg.empty())
	{
		server_kick(_iter);
		_iter--;
	}
	else
		message_handling(client_mssg);
}

void Server::connect_new_client()
{
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	int client = accept(_sock, (struct sockaddr *)&cli_addr, &clilen);
	std::cout << "New client connected (FD: " << client << ")" << std::endl;
	_fds.push_back({client, POLLIN, 0});
	Client client_class(client);
	client_class.set_ip(inet_ntoa(cli_addr.sin_addr));
	_clients.push_back(client_class);
}

void Server::iter_vec_pfds()
{
	_iter = 0;
	for (; _iter < _fds.size(); _iter++)
	{
		if (_fds[_iter].revents)
		{
			if (_fds[_iter].fd == _sock)
				Server::connect_new_client();
			else
				Server::client_message();
		}
	}
}

void Server::checkActivity()
{
	auto now = std::chrono::system_clock::now();
	size_t count = -1;
	for (auto &c : _clients)
	{
		++count;
		if (count == 0)
			continue;
		if (c.registered())
		{
			auto inactive = now - c.get_last_send_time();
			if ( inactive > std::chrono::minutes(10))
				server_kick(count, "ERROR :Closing Link: " + c.get_user_whole_str() + " (Ping timeout: 300)\r\n");
			else if(inactive > std::chrono::minutes(5) && !c.get_ping_send())
				sendPing(c);
		}
		else
		{
			if (now - c.get_joined_time() > std::chrono::seconds(20))
				server_kick(count, "ERROR :Closing Link (Connection timeout: 50)\r\n");
		}
	}
}

void Server::serverLoop()
{
	while (true)
	{
		int ret = poll(_fds.data(), _fds.size(), 0);
		checkActivity();
		if (ret == 0)
			continue;
		if (ret < 0)
		{
			std::cerr << "Error: poll() went wrong" << std::endl;
			break;
		}
		Server::iter_vec_pfds();
	}
}

void Server::cleanup()
{
	for (auto &fd : _fds)
	{
		if (fd.fd != -1)
			close(fd.fd);
	}
	close(_sock);
}

void Server::run()
{
	serverLoop();
	cleanup();
}

void Server::welcomeMessage()
{
	_clients[_iter].set_user_whole_str(_clients[_iter].get_nick() + "!" + _clients[_iter].get_user() + "@" + _clients[_iter].get_addr());
	sendRplErr(_clients[_iter], SERVERNAME, "001", RPL_WELCOME);
	sendRplErr(_clients[_iter], SERVERNAME, "002", RPL_YOURHOST);
	sendRplErr(_clients[_iter], SERVERNAME, "003", RPL_CREATED);
	sendRplErr(_clients[_iter], SERVERNAME, "004", RPL_MYINFO);
	sendRplErr(_clients[_iter], SERVERNAME, "375", ":- " SERVERNAME " Message of the day - ");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-  _______________________________________");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- / Penguin Trivia #46:                   \\");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- |                                       |");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- | Animals who are not penguins can only |");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- | wish they were.                       |");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- |                                       |");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- \\ -- Chicago Reader 10/15/82            /");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-  ---------------------------------------");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":- \\                             .       .");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-  \\                           / `.   .' \" ");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-   \\                  .---.  <    > <    >  .---.");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-    \\                 |    \\  \\ - ~ ~ - /  /    |");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-          _____          ..-~             ~-..-~");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-         |     |   \\~~~\\.'                    `./~~~/");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-        ---------   \\__/                        \\__/");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-       .'  O    \\     /               /       \\  \"");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-      (_____,    `._.'               |         }  \\/~~~/");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-       `----.          /       }     |        /    \\__/");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-             `-.      |       /      |       /      `. ,~~|");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-                 ~-.__|      /_ - ~ ^|      /- _      `..-'");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-                      |     /        |     /     ~-.     `-. _  _  _");
	sendRplErr(_clients[_iter], SERVERNAME, "372", ":-                      |_____|        |_____|         ~ - . _ _ _ _ _>");
	sendRplErr(_clients[_iter], SERVERNAME, "376", ":End of MOTD command");
}
