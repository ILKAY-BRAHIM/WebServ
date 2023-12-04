#pragma once
#include "RequestResponce.hpp"
#include <algorithm>

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
}       resp;



class Response 
{
    private :
        std::vector<t_server>	servers;
        resp					respMessage;
		request					req;
        void	generateResponse(std::string ServerName, std::string port);
		void	checkMethode();
		void	urlRegenerate();
    public :
        Response();
        Response(std::vector<t_server> servS);
        // Response(const Response &copy);
        // Response& operator=(Response& asignement);
        ~Response();
};
