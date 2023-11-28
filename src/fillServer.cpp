#include "Parse.hpp"

std::vector<t_server> fillServer(void)
{
    std::vector<t_server> servers;

    t_server server;
    server.name = "localhost";
    server.port = "80";
    server.host = "127.0.0.1";
    server.root = "/Users/rrasezin/Desktop/ServerData/src";
    server.index = "index.html";
    server.allow_methods.push_back("GET");
    server.allow_methods.push_back("POST");
    server.allow_methods.push_back("DELETE");
    server.index = "index.html";
    server.error_page.first = "404";
    server.error_page.second = "error.html";
    server.locations.push_back(t_location());
    server.locations[0].path = "/";
    // server.locations[0].root = "./www";
    // server.locations[0].index = "index.html";
    server.locations[0].allow_methods.push_back("GET");
    server.locations[0].allow_methods.push_back("POST");
    server.locations[0].allow_methods.push_back("DELETE");
    server.locations[0].error_page = "404 error_pages/404.html";
    servers.push_back(server);
    return (servers);
}