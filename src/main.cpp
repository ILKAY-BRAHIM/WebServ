#include "Parse.hpp"
#include "Server_.hpp"
#include "response.hpp"

int main (int ac, char **av, char **env)
{
	std::vector<t_server> servers;
	if (ac != 2)
		return (std::cout << "Error : no config file\n", 1);
	servers = Parse::read_parse(ac, av);
	if (!servers.size())
		return (1);
	
	Response resp(servers, env);
	try{

		Server serv(servers, resp);
		serv.start_server();
		serv.run();
	}
	catch(const char *e)
	{
		std::cerr << "\nErrorr: " << e << std::endl;
		return (1);
	}
	return (0);
}
