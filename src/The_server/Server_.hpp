#pragma once

#include "Parse.hpp"
#include "RequestResponce.hpp"


class Server
{
    private:
        std::vector<t_server> server;
        Server();
    public:
        Server(std::vector<t_server> server);
        Server(const Server &other);
        Server &operator=(const Server &other);
        ~Server();
        void run();
};