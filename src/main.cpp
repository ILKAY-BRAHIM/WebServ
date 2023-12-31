#include "Parse.hpp"
#include "Server_.hpp"
#include "response.hpp"

void	printServer(std::vector<t_server > serv)
{
	for(size_t i = 0, j = 0; i < serv.size(); i++)
	{
		std::cout << "server " << i << std::endl;
		std::cout << "	name: " << serv[i].name << std::endl;
		std::cout << "	host: " << serv[i].host << std::endl;
		std::cout << "	root: " << serv[i].root << std::endl;
		std::cout << "	port: ";
		for (j = 0; j < serv[i].port.size(); j++)
			std::cout << serv[i].port[j] << " ";
		std::cout << std::endl;
		std::cout << "	index: ";
		for (j = 0; j < serv[i].index.size(); j++)
			std::cout << serv[i].index[j] << " ";
		std::cout << std::endl;
		{
			std::cout << "	error_page: ";
			size_t h = 0;
			while (h < serv[i].error_page.first.size())
			std::cout << serv[i].error_page.first[h++] << " ";
			std::cout << serv[i].error_page.second << std::endl;
		}
		std::cout << "	redirect: " << serv[i].redirect << std::endl;
		std::cout << "	timeout: " << serv[i].timeout << std::endl;
		std::cout << "	allow_methods: ";
		for (j = 0; j < serv[i].allow_methods.size(); j++)
			std::cout << serv[i].allow_methods[j] << " ";
		std::cout << std::endl;
		std::cout << "	location: " << std::endl;
		for (j = 0; j < serv[i].locations.size(); j++)
		{
			std::cout << "	location " << j << std::endl;
			std::cout << "		path: " << serv[i].locations[j].path << std::endl;
			std::cout << "		root: " << serv[i].locations[j].root << std::endl;
			std::cout << "		alias: " << serv[i].locations[j].alias << std::endl;
			std::cout << "		index: ";
			for (size_t k = 0; k < serv[i].locations[j].index.size(); k++)
				std::cout << serv[i].locations[j].index[k] << " ";
			std::cout << std::endl;
			std::cout << "		alllow_methods: ";
			for (size_t k = 0; k < serv[i].locations[j].allow_methods.size(); k++)
				std::cout << serv[i].locations[j].allow_methods[k] << " ";
			std::cout << std::endl;
			std::cout << "		cgi_index: ";
			for (size_t k = 0; k < serv[i].locations[j].cgi_index.size(); k++)
				std::cout << serv[i].locations[j].cgi_index[k] << " ";
			std::cout << std::endl;
			std::cout << "		redirect: " << serv[i].locations[j].redirect << std::endl;
			if (serv[i].locations[j].internal == true)
				std::cout << "		internal: on" << std::endl;
			else
				std::cout << "		internal: off" << std::endl;

		}
		std::cout << std::endl;
	}
}

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
	printServer(servers);
	// pause();
	try{

		Server serv(servers, resp);
		serv.start_server();
		serv.run();
	}
	catch(const char *e)
	{
		// std::cout << "Error: " << e << std::endl;

		std::cerr << "\nErrorr: " << e << std::endl;
		return (1);
	}

	// get_request(servers);
}
