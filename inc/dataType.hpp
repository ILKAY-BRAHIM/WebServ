#pragma once

# include "server.h"
# include "Parse.hpp"

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
    std::multimap<std::string, t_info> file;
    std::multimap<std::string, t_info> directory;
}    t_Dir_Data;