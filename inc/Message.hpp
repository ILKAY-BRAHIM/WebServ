#pragma once
#include <iostream>
# include "response.hpp"
# include "test.hpp"



class   Message
{
    private :
        char **env;
        request req; // to be used in the response generator
        t_server server; // used in the check & response generator
        int status;
        std::string mess;
        int     content_length;
        std::string body;
    public :
        Message();
        void    setResponse(std::string mess);
        void    setRequest(request req);
        void    setServer(t_server server);
        int     getContentLength();
        void    setContentLength(int content_length);
        void    setBody(std::string body);
        std::string getBody();
        void    setEnv(char **env);
        char **getEnv();
        t_server getServer();
        void    setStatus(int status);
        int     getStatus();
        std::string getResponse();
        request getRequest();
        // int getStatus;
        ~Message();
};
