#include "Message.hpp"
#include <fstream>

Message::Message(){};

std::string Message::getResponse()
{
    std::ifstream file("/Users/bchifour/Desktop/WEBSERV/src/The_server/fill.txt");
    if (!file.is_open())
    {
        // std::cout << "file not open" << std::endl;
        std::perror("file not open");
        exit(1);
    }
    std::string hello;
    std::string line;
    while (getline(file, line))
    {
        hello += line;
    }
    file.close();
    std::string res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\nContent-Length: "+ std::to_string(hello.length()+20)+"\r\n\r\n<html><body><h1>"+hello + "</h1></body></html>";


    //  std::ifstream imageFile("/Users/bchifour/Desktop/WebServer/bchifour.jpeg", std::ios::binary);
    // std::string imageData((std::istreambuf_iterator<char>(imageFile)), std::istreambuf_iterator<char>());
    // std::string response = "HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: " + std::to_string(imageData.size()) + "\n\n" + imageData;
    return (res);
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