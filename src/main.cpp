#include "Parse.hpp"
#include "RequestResponce.hpp"

int main (int ac, char **av)
{
	std::vector<t_server> servers;
	if (ac != 2)
		return (std::cout << "Error : no config file\n", 1);
	servers = Parse::read_parse(ac, av);
	if (!servers.size())
		return (1);
	//continue above this line
	
	// printServers(servers);

	get_request(servers);
}
