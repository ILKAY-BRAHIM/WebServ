#include "Parse.hpp"
#include "The_server/Server_.hpp"
#include "response.hpp"

int main (int ac, char **av)
{
	
	std::vector<t_server> servers;
	if (ac != 2)
		return (std::cout << "Error : no config file\n", 1);
	servers = Parse::read_parse(ac, av);
	if (!servers.size())
		return (1);
	

	//continue above this line
	// Response resp(servers);
	// Server serv(servers);
	// serv.start_server();
	// serv.run();

	get_request(servers);
}
