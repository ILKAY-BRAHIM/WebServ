#include "Server_.hpp"


Server::Server(std::vector<t_server> &server, Response &resp)
{
    this->server = server;
    this->resp = resp;
}

Server::Server(const Server &other)
{
    *this = other;
}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
        this->server = other.server;
    return (*this);
}

void Server::start_server()
{
    int server_fd;
    int opt = 1;

    FD_ZERO(&this->master_set);
            int index = 0;
    for (unsigned long i = 0; i < this->server.size(); i++)
    {
        std::vector<int>::iterator it = server[i].port.begin();
        for(; it !=  server[i].port.end(); it++)
        {
            // std::cout << *it << std::endl;
            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            {
                std::cout << "socket failed" << std::endl;
                return;
            }
            if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            {
                std::perror("setsockopt");
                return;
            }
            if (ioctl(server_fd, FIONBIO, (char *)&opt) < 0)
            {
                perror("ioctl");
                exit(EXIT_FAILURE);
            }
            this->address.sin_family = AF_INET;
            this->address.sin_addr.s_addr = INADDR_ANY;
            // this->address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            this->address.sin_port = htons(*it);
            memset(address.sin_zero, '\0', sizeof this->address.sin_zero);

            if (bind(server_fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
            {
                std::cout << "bind failed" << std::endl;
                return;
            }
            if (listen(server_fd, 20) < 0)
            {
                std::cout << "listen" << std::endl;
                return;
            }

            std::cout << "\033[93mServer is running on port \033[92m" << *it << "\033[39m"<<std::endl;

            this->fds.push_back(server_fd);
            // std::cout << "fds: " << this->fds[index] << std::endl;
            FD_SET(fds[index], &this->master_set);
            index++;
        }
    }
    return;
}

void Server::run()
{
    int max_fd = this->fds.back();
    int addrlen = sizeof(this->address);
    timeout.tv_sec  = 60;
    timeout.tv_usec = 0;
    int opt = 1;
    FD_ZERO(&this->write_set1);
    FD_ZERO(&this->write_set2);
    while(1)
    {
        memcpy(&this->working_set, &this->master_set, sizeof(this->master_set));
        memcpy(&this->write_set2, &this->write_set1, sizeof(this->write_set1));
        int activity = select(max_fd + 1, &this->working_set, &this->write_set2, NULL, &this->timeout);
        if (activity < 0)
        {
            std::cout << "select error" << std::endl;
            std::cout << "errno: " << errno << std::endl;
            std::perror("select");
            return;
        }
        if (activity == 0)
        {
            for(std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end();)
            {
                if (std::clock() -  it->second.get_start() >= 6000)
                {
                    std::cout << "\033[91mconnection close by server... \033[39m" << std::endl;
                    close(it->first);
                    FD_CLR(it->first, &this->master_set);
                    if (it->first == max_fd)
                    {
                        while (FD_ISSET(max_fd, &this->master_set) == 0)
                            max_fd -= 1;
                    }
                    this->serv2.erase(it);
                    if(this->serv2.empty())
                        break;
                }
                else
                    it++;
            }
        }
        int i = 0;
        while(i++ <= 1024 && activity > 0)
        {
            if (FD_ISSET(i, &this->working_set))
            {
                activity--;
                unsigned long y = find(this->fds.begin(), this->fds.end(), i) - this->fds.begin();
                if (y < fds.size() && i == this->fds[y])
                {
                    int new_socket = accept(this->fds[y], (struct sockaddr *)&this->address, (socklen_t*)&addrlen);
                    if (ioctl(new_socket, FIONBIO, (char *)&opt) < 0)
                    {
                         perror("ioctl");
                        exit(EXIT_FAILURE);
                    }

                    if(setsockopt(new_socket, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) < 0)
                    {
                        std::perror("setsockopt");
                        return;
                    }
                    if (new_socket  < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("In accept");
                            std::cout << "accept error" << std::endl;
                            exit(1);
                        }
                    }
                    if (this->serv.size() >= 1)
                    {
                        std::vector<std::pair<int, Servers> >::iterator ittt = this->serv2.end();
                        for (std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end(); it++)
                        {
                            if (it->first == new_socket)
                            {
                                ittt = it;
                                break;
                            }
                        }
                        if (ittt == this->serv2.end())
                        {
                            FD_SET(new_socket, &this->master_set);
                            if (new_socket > max_fd)
                                max_fd = new_socket;
                            Servers s("");
                            s.set_start(std::clock());
                            s.set_redirection(0);
                            this->serv2.push_back(std::pair<int, Servers>(new_socket, s));
                        }
                    }
                    else
                    {
                        FD_SET(new_socket, &this->master_set);
                        if (new_socket > max_fd)
                            max_fd = new_socket;
                        Servers s("");
                        s.set_redirection(0);
                        s.set_start(std::clock());
                        this->serv2.push_back(std::pair<int, Servers>(new_socket, s));
                    }
                }
                else
                {
                    // char buffer[1];
                    // int a = recv(i, buffer, 1, 0);
                    // // buffer[a] = '\0';
                    // // std::cout << buffer << std::endl;
                    // if(a <= 0)
                    // {
                    //     std::cout << "\033[91mconnection close by peer... \033[39m" << std::endl;
                    //     if (a == 0)
                    //         std::cout << "socket " << i << " hung up" << std::endl;
                    //     close(i);
                    //     FD_CLR(i, &this->master_set);
                    //     if (i == max_fd)
                    //     {
                    //         while (FD_ISSET(max_fd, &this->master_set) == 0)
                    //             max_fd -= 1;
                    //     }
                    // }
                    
                    for(std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end(); it++)
                    {
                        if (it->first == i && it->second.get_redirection() == 0)
                        {
                            char buffer[1];
                            int a = recv(i, buffer, 1, 0);
                             if(a <= 0)
                            {
                                std::cout << "\033[91mconnection close by peer... \033[39m" << std::endl;
                                if (a == 0)
                                    std::cout << "socket " << i << " hung up" << std::endl;
                                close(i);
                                FD_CLR(i, &this->master_set);
                                if (i == max_fd)
                                {
                                    while (FD_ISSET(max_fd, &this->master_set) == 0)
                                        max_fd -= 1;
                                }
                            }
                            it->second.collect_req(buffer, a);
                            it->second.set_start(std::clock());
                            if (it->first == i &&  it->second.get_request().find(  "\r\n\r\n") != std::string::npos)
                            {
                                Message *resw =  this->resp.checkHeader(it->second.get_request()); // incoming changed..
                                it->second.set_responce_class(resw);
                                // std::cout << it->second.get_responce_class()->getStatus() << std::endl;
                                if(it->second.get_responce_class()->getContentLength() != 0)
                                    it->second.set_redirection(1);
                                else
                                {
                                    
                                    this->resp.generateResponse(it->second.get_responce_class());
                                    it->second.set_responce(it->second.get_responce_class()->getResponse());
                                    std::cout << it->second.get_responce() << std::endl;
                                    FD_SET(it->first, &this->write_set1);
                                    FD_CLR(it->first, &this->master_set);
                                    this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                                    this->serv2.erase(it);
                                }
                            }
                            break;
                        }
                        else if (it->first == i && it->second.get_redirection() == 1)
                        {
                            char buffer[60000];
                            int a = recv(i, buffer, 60000, 0);
                            if(a <= 0)
                            {
                                std::cout << "\033[91mconnection close by peer... \033[39m" << std::endl;
                                if (a == 0)
                                    std::cout << "socket " << i << " hung up" << std::endl;
                                close(i);
                                FD_CLR(i, &this->master_set);
                                if (i == max_fd)
                                {
                                    while (FD_ISSET(max_fd, &this->master_set) == 0)
                                        max_fd -= 1;
                                }
                            }
                            it->second.collect_body(buffer, a);
                            it->second.set_start(std::clock());
                            it->second.set_total_body(a);
                            if(it->second.get_total_body() == (unsigned long)it->second.get_responce_class()->getContentLength())
                            {
                                it->second.set_responce(it->second.get_responce_class()->getResponse());
                                it->second.set_redirection(0);
                                FD_SET(it->first, &this->write_set1);
                                FD_CLR(it->first, &this->master_set);
                                this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                                this->serv2.erase(it);
                            }
                            break;
                        }
                    }
                    // for (std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end();)
                    // {
                    //     if (it->first == i &&  it->second.get_request().find(  "\r\n\r\n") != std::string::npos)
                    //     {
                    //         Message *resw =  this->resp.generateResponse(it->second.get_request());
                    //         it->second.set_responce_class(resw);
                    //         it->second.set_responce(it->second.get_responce_class()->getResponse());
                    //         std::cout << this->resp.checkHeader(it->second.get_request()) << std::endl;
                    //         unsigned long  index = it->second.get_request().find("\r\n\r\n");
                    //         if (index != std::string::npos)
                    //         {
                    //             index = it->second.get_request().find("\0", index + 4);
                    //             if (index != std::string::npos)
                    //             {
                    //                 std::cout << it->second.get_request() << std::endl;
                    //                 FD_SET(it->first, &this->write_set1);
                    //                 FD_CLR(it->first, &this->master_set);
                    //                 this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                    //                 this->serv2.erase(it);
                    //                 if(serv2.empty() || serv2.size() == 0)
                    //                     break;
                    //             }
                    //             else
                    //                 it++;
                    //         }

                    //     }
                    //     else
                    //         it++;
                    // }
                }
            }
            if (FD_ISSET(i, &this->write_set2))
            {
                for (std::map<int, Servers>::iterator it = this->msg.begin(); it != this->msg.end();it++)
                {
                    if (it->first == i)
                    {
                        if (it->second.get_total() == 0)
                        {
                            int j = send(i, it->second.get_responce().c_str(), it->second.get_responce().length(), 0);
                            if (j <= 0)
                            {
                                std::cout << "\033[91mconnection close by peer2... \033[39m" << std::endl;
                                close(i);
                                FD_CLR(i, &this->write_set1);
                                if (i == max_fd)
                                {
                                    while (FD_ISSET(max_fd, &this->master_set) == 0)
                                        max_fd -= 1;
                                }
                                this->msg.erase(it);
                                break;
                            }
                            else
                                it->second.set_partis(j);
                        }
                        else
                        {
                            int j = send(it->first, it->second.get_responce().c_str() + it->second.get_total(), it->second.get_responce().length() - it->second.get_total(), 0);
                            if (j <= 0 )
                            {
                                std::cout << "\033[91mconnection close by peer1... \033[39m" << std::endl;
                                close(i);
                                FD_CLR(i, &this->write_set1);
                                if (i == max_fd)
                                {
                                    while (FD_ISSET(max_fd, &this->master_set) == 0)
                                        max_fd -= 1;
                                }
                                this->msg.erase(it);
                                break;
                            }
                            else
                                it->second.set_partis(j);
                        }
                        it->second.set_total(it->second.get_partis());
                        std::cout << "\033[92msend " << it->second.get_total() << " bytes to client " << it->first << " total of data is " << it->second.get_responce().length() << " bytes\033[39m" <<std::endl;
                        if (it->second.get_total() == it->second.get_responce().length())
                        {
                            FD_CLR(it->first, &this->write_set1);
                            FD_SET(it->first, &this->master_set);
                            it->second.rset_total(0);
                            it->second.set_responce("");
                            it->second.set_request("");
                            serv2.push_back(std::pair<int, Servers>(it->first, it->second));
                            this->msg.erase(it);
                        }
                        else
                            it->second.set_start(std::clock());
                        if(this->msg.empty())
                            break;
                    }
                }
            }
        }
    }
}

Server::~Server()
{
}
