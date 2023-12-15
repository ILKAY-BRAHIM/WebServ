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
# include "test.hpp"
// class   Message ;
# define PORT 81

# define ISDIR 0
# define ISFILE 1
# define NOTFONDE 2
# define CRLF "\r\n"

class Message;

typedef struct t_response
{
    std::string http_version;
    std::string	statusCode;
    std::string Access_Controle_Allow_Origin;
    std::string Cache_Control;
    std::string Content_Type;
    std::string Content_Lenght;
    std::string ETag;
    std::string Last_Modified;
    std::string Location; //+
    std::string Set_Cookie;
    std::string Server;
	std::string query_String; //+
    std::string body;
}       resp;


class Response 
{
    private :
        std::vector<t_server> servS;
        t_server    server;
        t_location  location;
        resp		respMessage;
		request		req;
        std::string resp;
        t_server    fillServer(request req);
		void	checkMethode();
		void	urlRegenerate();
        void    generateBody(std::string path);
        void    isDirectory(std::string path, std::string url);
        int     getLocation(std::string url);
        void     generateBodyError(int error);
        std::string generateMessage();
        void    clearResponse();
        // void    readPath();
        void        redirect(std::string path);
    public :
        Response();
        Response(std::vector<t_server> servS);
        void	generateResponse(Message* mes);
        Message*    checkHeader(std::string req);
        // char **getEnv();
        // std::string getResponse();
        // Response(const Response &copy);
        // Response& operator=(Response& asignement);
        ~Response();
};


int get_request(std::vector<t_server> servers);

template <typename T>
std::string get_index(T& location, std::string path, int noIndex)
{
    std::string fullPath;
    if (noIndex)
    {
        fullPath = path + "index.html";
        if (access((fullPath).c_str(), F_OK | R_OK) == 0)
            return ("index.html");
        else
            return "";
    }
    if (location.index.size() == 0)
        return "";
    std::vector<std::string >::iterator it = location.index.begin();
    while (it != location.index.end())
    {
        fullPath = path + (*it);
        if (access((fullPath).c_str(), F_OK | R_OK) == 0)
            return (*it);
        it++;
    }
    return "";
}

std::string get_extension(std::string path);