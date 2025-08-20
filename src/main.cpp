
#include "Server.hpp"

int main(int argc, char **argv)
{
	try
	{
		Server server(argc, argv);
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
