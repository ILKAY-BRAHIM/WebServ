#include "Message.hpp"
#include <fstream>

Message::Message(){};

std::string Message::getResponse()
{
    return (this->mess);
}

int Message::getStatus()
{
    return (this->status);
}

void    Message::setStatus(int status)
{
    this->status = status;
}

size_t    Message::getContentLength()
{
    return (this->content_length);
}

void    Message::setContentLength(size_t content_length)
{
    this->content_length = content_length;
}

void    Message::setResponse(std::string mess)
{
    this->mess = mess;
}

std::vector<std::string> Message::getEnv()
{
    return (this->env);
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

void    Message::setBody(std::string body)
{
    this->body = body;
}

std::string Message::getBody()
{
    return (this->body);
}

void    Message::setEnv(std::vector<std::string> env)
{
    this->env = env;
}

void    Message::setLocation(t_location location)
{
    this->location = location;
}

t_location Message::getLocation()
{
    return (this->location);
}

void    Message::setTransfer_Encoding(bool s)
{
    this->transfer_encoding = s;
}

bool    Message::getTransfer_Encoding(void)
{
    return(this->transfer_encoding);
}

Message::~Message(){};