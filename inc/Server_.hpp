#pragma once

#include "Parse.hpp"

#include "server.h"
#include "Servers.hpp"
#include "response.hpp"
#include "Message.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdbool>


class Server
{
    private:
        std::vector<t_server> server;
        Response resp;
        struct sockaddr_in address;
        struct timeval      timeout;
        fd_set master_set, working_set, write_set1, write_set2, status1, status2;
        std::vector<int> fds;
        std::map<int,Servers> serv;
        std::vector<std::pair<int, Servers> > serv2;
        std::map<int,Servers> msg;
        // std::vector<std::pair<int, Servers> > timeout_client;
        std::map<int,Servers> timeout_client;

        Server();
    public:
        Server(std::vector<t_server> &server, Response &resp);
        Server(const Server &other);
        Server &operator=(const Server &other);
        void run();
        void start_server();
        void print_log(const std::string& str, const std::string& color, int count, unsigned long total, int content_length);
        ~Server();
};