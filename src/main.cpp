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
	// std::string h("hello");
	// Message *mes = resp.generateResponse(h);
	// std::cout << mes->getResponse() << std::endl;
	//continue above this line
	// printServers(servers);
	Server serv(servers, resp);
	serv.start_server();
	serv.run();

	// get_request(servers);
}
