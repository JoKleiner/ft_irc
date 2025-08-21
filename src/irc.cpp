
#include "../inc/irc.hpp"
#include "../inc/Client.hpp"

static void connect_new_client(int sock, std::vector<pollfd> &vec_pfds, std::vector<Client> &vec_client)
{
	int client = accept(sock, nullptr, nullptr);
	std::cout << "New client connected (FD: " << client << ")" << std::endl;
	vec_pfds.push_back({client, POLLIN, 0});
	Client client_class;
	vec_client.push_back(client_class);
}

static std::string read_client_message(std::vector<pollfd> vec_pfds, size_t i)
{
	std::string client_mssg;
	ssize_t bytes = 1;

	while (client_mssg.find('\n') == std::string::npos && bytes > 0)
	{
		char buff[1024] = {0};
		bytes = read(vec_pfds[i].fd, buff, sizeof(buff) - 1);
		client_mssg = client_mssg + buff;
	}
	return (client_mssg);
}

static std::vector<std::string> token_message(std::string client_mssg)
{
	std::vector<std::string> vec_token;
	std::istringstream ss(client_mssg);
	std::string token;

	while (ss >> token)
		vec_token.push_back(token);
	return (vec_token);
}

static void client_message(std::vector<pollfd> &vec_pfds, std::vector<Client> &vec_client, size_t i)
{
	std::string client_mssg = read_client_message(vec_pfds, i);

	std::cout << "Client (FD " << vec_pfds[i].fd << "): " << client_mssg << std::flush;
	send(vec_pfds[i].fd, "Message received\n", sizeof("Message received\n"), 0);

	if (client_mssg.empty())
	{
		vec_client[i].kick_user(i, vec_pfds);
		i--;
	}
	else
		message_handling(vec_client, vec_pfds, i, token_message(client_mssg));
}

static void iter_vec_pfds(int sock, std::vector<pollfd> &vec_pfds, std::vector<Client> &vec_client)
{
	for (size_t i = 0; i < vec_pfds.size(); i++)
	{
		if (vec_pfds[i].revents)
		{
			if (vec_pfds[i].fd == sock)
				connect_new_client(sock, vec_pfds, vec_client);
			else
				client_message(vec_pfds, vec_client, i);
		}
	}
}

/*
	poll: Wait for events in vec_pfds (new client connect or message received)
	ret = 0, when no change in vec_pfds
*/
void run_irc(int sock)
{
	std::vector<Client> vec_client;
	std::vector<pollfd> vec_pfds;

	vec_pfds.push_back({sock, POLLIN, 0});

	while (true)
	{
		int ret = poll(vec_pfds.data(), vec_pfds.size(), 0);
		if (ret < 0)
		{
			std::cerr << "Error: poll() went wrong" << std::endl;
			break;
		}
		if (ret == 0)
			continue;
		iter_vec_pfds(sock, vec_pfds, vec_client);
	}
	for (auto pfds = vec_pfds.begin(); pfds != vec_pfds.end(); pfds++)
		if (pfds->fd != -1)
			close(pfds->fd);
}
