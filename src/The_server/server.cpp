#include "Parse.hpp"
#include "RequestResponce.hpp"
#include "Server.hpp"

Server::Server(std::vector<t_server> servers)
{
    this->servers = servers;
}

Server::Server(const Server &other)
{
    *this = other;
}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
        this->server = other.server;
    return (*this);
}

Server::~Server()
{
}