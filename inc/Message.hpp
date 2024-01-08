#pragma once
#include <iostream>
# include "response.hpp"
# include "dataType.hpp"



class   Message
{
    private :
        std::vector<std::string> env;
        request req; // to be used in the response generator
        t_server server; // used in the check & response generator
        t_location location; // used in the check & response generator
        int status;
        std::string mess;
        size_t content_length;
        std::string body;
        bool    transfer_encoding;
    public :
        Message();
        void    setResponse(std::string mess);
        void    setRequest(request req);
        void    setServer(t_server server);
        size_t  getContentLength();
        void    setContentLength(size_t content_length);
        void    setBody(std::string body);
        std::string getBody();
        void    setLocation(t_location location);
        t_location getLocation();
        void    setEnv(std::vector<std::string> env);
        std::vector<std::string> getEnv();
        t_server getServer();
        void    setStatus(int status);
        int     getStatus();
        std::string getResponse();
        request getRequest();
        void    setTransfer_Encoding(bool s);
        bool    getTransfer_Encoding(void);
        // int getStatus;
        ~Message();
};
