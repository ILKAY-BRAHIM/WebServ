#pragma once
#include <iostream>
# include "response.hpp"

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

class   Message
{
    private :
        // char **env;
        // int status;
        std::string mess;
    public :
        Message();
        void    setResponse(std::string mess);
        // void    setenv(request req);
        // void    setStatus();
        std::string getResponse();
        char **getEnv();
        int getStatus;
        ~Message();
};
