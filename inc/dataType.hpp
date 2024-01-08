#pragma once

#include <algorithm>
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
# include <utility>
# include "Message.hpp"
# include <unistd.h>

typedef struct t_request
{
    std::string method;
    std::string path;
    std::string httpVertion;
    std::string boundary;
    std::map<std::string, std::string> headers;
    std::string body;
}        request;

typedef struct s_info
{
    std::string name;
    std::string type;
    std::string size;
    std::string unity;
    std::string date;
}       t_info;

typedef struct s_Dir_Data
{
    // typedef std::multimap<std::string, t_info> f_map;
    // typedef std::multimap<std::string, t_info> d_map;
    std::multimap<std::string, t_info> file;
    std::multimap<std::string, t_info> directory;
}    t_Dir_Data;