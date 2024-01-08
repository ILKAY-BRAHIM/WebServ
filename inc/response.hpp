#pragma once

# include <algorithm>
# include "server.h"
# include <fstream>
# include <fcntl.h>
# include <sstream>
# include <ctime>
# include <dirent.h>
# include <sstream>
# include "Parse.hpp"
# include <sys/stat.h>
# include <sys/types.h>
# include <utility>
# include "Message.hpp"
# include "dataType.hpp"
# include "cgi.hpp"


# define CRLF "\r\n"
# define CLIENT_MAX_BODY_SIZE 10485760 // equevalent of 10M
# define MEGABYTE 1048576
# define KB 1000
# define MB 1000000
# define GB 1000000000

class Message;

typedef struct t_response
{
    std::string http_version;
    std::string statusCode;
    std::string Cache_Control;
    std::string Set_Cookie;
    std::string Content_Type;
    std::string Content_Lenght;
    std::string Last_Modified;
    std::string Location;
    std::string Server;
    std::string query_String;
    std::string Connection;
    std::string body;
} resp;


void    clearDirectory(std::string ini_dir);
class Response
{
	private:
		char						**env;
		std::vector<t_server>		servS;
		std::vector<std::string>	header;
		t_server					server;
		t_location					location;
		resp						respMessage;
		request						req;
		std::string					body;
		std::string					path;
		std::string					uploadedFile;
		size_t						content_length;
		std::vector<std::string>	r_env;
		t_server fillServer(request req);
		void checkMethode();
		int urlRegenerate();
		int generateBody(std::string path);
		int isDirectory(std::string path);
		int getLocation(std::string url);
		void generateBodyError(int error);
		std::string generateMessage();
		int generateUploadDeleteBody(std::string method);
		void clearResponse();
		void redirect(std::string path, int status);
		int uploadFile();
		int postMethod();
		int deleteMethod();
		int specificErrorPage(int error_code);
		void unchunkeBody();
		std::string getRoot();
		int generateAutoindexBody();
		void parseCGI_body(std::string body);
	public:
		Response();
		Response(std::vector<t_server> servS, char **env);
		void generateResponse(Message *mes);
		Message *checkHeader(std::string req);
		void removeSession_Database();
		~Response();
};


template <typename T>
std::string get_index(T &location, std::string path, int noIndex)
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
    std::vector<std::string>::iterator it = location.index.begin();
    while (it != location.index.end())
    {
        fullPath = path + (*it);
        if (access((fullPath).c_str(), F_OK | R_OK) == 0)
        {
            return (*it);
        }
        it++;
    }
    return "";
}

std::string	get_extension(std::string path);
t_Dir_Data	readDirectory(std::string path);
std::string	generateStatusCode(int status);
