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


typedef struct s_env
{
    std::string FCGI_ROLE;
    std::string QUERY_STRING;
    std::string REQUEST_METHOD;
    std::string CONTENT_TYPE;
    std::string CONTENT_LENGTH;
    std::string SCRIPT_NAME;
    std::string REQUEST_URI;
    std::string DOCUMENT_URI;
    std::string DOCUMENT_ROOT;
    std::string SERVER_PROTOCOL;
    std::string REQUEST_SCHEME;
    std::string GATEWAY_INTERFACE;
    std::string SERVER_SOFTWARE;
    std::string REMOTE_ADDR;
    std::string REMOTE_PORT;
    std::string REMOTE_USER;
    std::string SERVER_ADDR;
    std::string SERVER_PORT;
    std::string SERVER_NAME;
    std::string REDIRECT_STATUS;
    std::string SCRIPT_FILENAME;
    std::string HTTP_HOST;
    std::string HTTP_USER_AGENT;
    std::string HTTP_ACCEPT;
    std::string HTTP_ACCEPT_LANGUAGE;
    std::string HTTP_ACCEPT_ENCODING;
    std::string HTTP_CONTENT_TYPE;
    std::string HTTP_CONTENT_LENGTH;
    std::string HTTP_ORIGIN;
    std::string HTTP_CONNECTION;
    std::string HTTP_REFERER;
    std::string HTTP_UPGRADE_INSECURE_REQUESTS;
    std::string LANG;
    std::string PATH;
    std::string HOME;
    std::string LOGNAME;
    std::string USER;
    std::string INVOCATION_ID;
    std::string JOURNAL_STREAM;
    std::string SYSTEMD_EXEC_PID;
    std::string DAEMON_OPTS;
}       t_env;