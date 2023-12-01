#include "Server_.hpp"

Server::Server(std::vector<t_server> servers)
{
    this->server = servers;
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
