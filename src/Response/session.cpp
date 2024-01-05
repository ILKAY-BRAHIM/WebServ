#include "response.hpp"

void    Response::setSession(int fd)
{
    size_t i = 0;

    while (i < this->sessions.size())
    {
        if (fd == this->sessions[i].first)
            return ;
        i++;
    }
    this->sessions.push_back(std::make_pair(fd, "false"));
}

void    Response::unsetSession(int fd)
{
    size_t i = 0;

    while (i < this->sessions.size())
    {
        if (fd == this->sessions[i].first)
        {
            this->sessions.erase(this->sessions.begin() + i);
            break;
        }
        i++;
    }
}

void    Response::printSession()
{
    size_t i = 0;

    while (i < this->sessions.size())
    {
        i++;
    }
}