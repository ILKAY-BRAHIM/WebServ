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
    std::map<std::string, std::string> headers;
    std::string body;
}        request;
