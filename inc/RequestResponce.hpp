#ifndef REQUESTRESPONCE_HPP
# define REQUESTRESPONCE_HPP

# include "server.h"
# include <stdio.h>
# include <iostream>
# include <sys/socket.h>
# include <unistd.h>
# include <fstream>
# include <stdlib.h>
# include <netinet/in.h>
# include <string.h>
# include <fcntl.h>
# include <map>
# include <sstream>
# include "Parse.hpp"
# include <sys/stat.h>
# include <sys/types.h>

# define PORT 80

typedef struct t_request
{
    std::string method;
    std::string path;
    std::string httpVertion;
    std::map<std::string, std::string> headers;
    std::string body;
}        request;

int get_request(std::vector<t_server> servers);
std::vector<t_server> fillServer(void);
void    printServers(std::vector<t_server> servers);
void    print_new_request(request req);
void sendResponse(std::vector<t_server> servers , request req, int k);

#endif