#pragma once

#include "Parse.hpp"

#include "server.h"

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string>
#include <stdlib.h>
#include <cstdbool>
#include <vector>


class Server
{
    private:
        std::vector<t_server> server;
        struct sockaddr_in address;
        struct timeval      timeout;
        fd_set master_set, working_set;
        std::vector<int> fds;
        int addrlen;
        Server();
    public:
        Server(std::vector<t_server> server);
        Server(const Server &other);
        Server &operator=(const Server &other);
        void run();
        void start_server();
        ~Server();
};