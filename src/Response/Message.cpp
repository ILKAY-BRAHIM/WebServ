#include "Message.hpp"

Message::Message(){};

std::string Message::getResponse()
{
    std::string mes("Hello World -_-");
    int size = mes.length();
    return ("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + mes);
}

// void    Message::setenv(request req)
// {
//     std::string envirement[25];

//     envirement[0] = "SERVER_NAME";
//     envirement[1] = "SERVER_SOFTWARE";
//     envirement[2] = "GATEWAY_INTERFACE";
//     envirement[3] = "SERVER_PROTOCOL";
//     envirement[4] = "SERVER_PORT";
//     envirement[5] = "REQUEST_METHOD";
//     envirement[6] = "PATH_INFO";
//     envirement[7] = "PATH_TRANSLATED";
//     envirement[8] = "SCRIPT_NAME";
//     envirement[9] = "QUERY_STRING";
//     envirement[10] = "AUTH_TYPE";
//     envirement[11] = "REMOTE_USER";
//     envirement[12] = "REMOTE_IDENT";
//     envirement[13] = "CONTENT_TYPE";
//     envirement[14] = "CONTENT_LENGTH";
//     envirement[15] = "REMOTE_ADDR";
//     envirement[16] = "REQUEST_URI";
//     envirement[17] = "PATH";
//     envirement[18] = "SERVER_ADDR";
//     envirement[19] = "SERVER_NAME";
//     envirement[20] = "SERVER_ADMIN";
//     envirement[21] = "HTTP_ACCEPT";
//     envirement[22] = "HTTP_ACCEPT_LANGUAGE";
//     envirement[23] = "HTTP_COOKIE";
//     envirement[24] = "HTTP_REFERER";
//     envirement[25] = "HTTP_USER_AGENT";

// }

char **Message::getEnv()
{
    return (NULL);
}

int getStatus()
{
    return (1);
}

Message::~Message(){};