#pragma once
#include <iostream>

class   Message
{
    private :
        // char **env;
        // int status;
        // std::string mess;
    public :
        Message();
        // void    setResponse();
        // void    setenv();
        // void    setStatus();
        std::string getResponse();
        char **getEnv();
        int getStatus;
        ~Message();
};
