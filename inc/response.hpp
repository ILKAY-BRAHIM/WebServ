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

# define PORT 81

typedef struct t_response
{
    std::string http_version;
    int			statusCode;
    std::string Access_Controle_Allow_Origin;
    std::string Cache_Control;
    std::string Content_Type;
    std::string Content_Lenght;
    std::string ETag;
    std::string Last_Modified;
    std::string Location;
    std::string Set_Cookie;
    std::string Server;
	std::string query_String;
    std::string body;
}       resp;

typedef struct t_request
{
    std::string method;
    std::string path;
    std::string httpVertion;
    std::map<std::string, std::string> headers;
    std::string body;
}        request;

class Response 
{
    private :
        std::vector<t_server> servS;
        t_server    server;
        resp		respMessage;
		request		req;
        std::string resp;
        void    fillServer(char *req);
		void	checkMethode();
		void	urlRegenerate();
        // void    readPath();
    public :
        Response();
        Response(std::vector<t_server> servS);
        Message*	generateResponse(std::string &req);
        // char **getEnv();
        // std::string getResponse();
        // Response(const Response &copy);
        // Response& operator=(Response& asignement);
        ~Response();
};

// class   Message
// {
//     private :
//         char **env;
//         int status;
//         char *mess;
//     public :
//         Message();
//         Message(char *req);
//         std::string getResponse();
//         char **getEnv();
//         ~Message();
// };

int get_request(std::vector<t_server> servers);