#include "Message.hpp"
#include <fstream>

Message::Message(){};

std::string Message::getResponse()
{
    // std::string mes("Hello World -_-");
    // int size = mes.length();
    // return ("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + mes);
    return (this->mess);
}

// void    Message::setenv(request req)
// {
//     std::string envirement[25];



// }

int Message::getStatus()
{
    return (this->status);
}

void    Message::setStatus(int status)
{
    this->status = status;
}

int    Message::getContentLength()
{
    return (this->content_length);
}

void    Message::setContentLength(int content_length)
{
    this->content_length = content_length;
}

void    Message::setResponse(std::string mess)
{
    this->mess = mess;
}

char **Message::getEnv()
{
    return (NULL);
}

int getStatus()
{
    return (1);
}

void    Message::setServer(t_server server)
{
    this->server = server;
}

void   Message::setRequest(request req)
{
    this->req = req;
}

request Message::getRequest()
{
    return (this->req);
}

t_server Message::getServer()
{
    return (this->server);
}

Message::~Message(){};