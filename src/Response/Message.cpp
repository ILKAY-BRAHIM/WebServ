#include "Message.hpp"

Message::Message(){};

std::string Message::getResponse()
{
    std::string mes("Hello World -_-");
    int size = mes.length();
    return ("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + mes);
}

char **Message::getEnv()
{
    return (NULL);
}

int getStatus()
{
    return (1);
}

Message::~Message(){};