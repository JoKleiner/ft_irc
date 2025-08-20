
#include "Server.hpp"

int main(int argc, char **argv)
{
	try
	{
		Server::start(argc, argv);
		Server::run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
