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

/*void Server::run_1()
{
    int max_fd = this->fds.back();
    int addrlen = sizeof(this->address);
    timeout.tv_sec  = 1 * 60;
    timeout.tv_usec = 0;
    unsigned long ress;
    FD_ZERO(&this->write_set1);
    FD_ZERO(&this->write_set2);

    ress = 0;
    

    while(1)
    {
                                            
        memcpy(&this->working_set, &this->master_set, sizeof(this->master_set));
        memcpy(&this->write_set2, &this->write_set1, sizeof(this->write_set1));
        int activity = select(max_fd + 1, &this->working_set, &this->write_set2, NULL, &this->timeout);
        if (activity < 0)
        {
            std::cout << "select error" << std::endl;
            return;
        }
        if (activity == 0)
        {
            std::cout << "timeout" << std::endl;
            return;
        }
        int i = 0;
        while(i++ <= max_fd && activity > 0)
        {
            if (FD_ISSET(i, &this->working_set))
            {
                activity--;
                unsigned long y = find(this->fds.begin(), this->fds.end(), i) - this->fds.begin();
                // std::cout << "y: " << y << " "<< fds.size() << std::endl;
                // if(y >= fds.size())
                //     y = fds.size() - 1;  
                int server_fd = this->fds[y];
                if (i == server_fd)
                {
                        std::cout << "new connection" << std::endl;

                        int new_socket = accept(server_fd, (struct sockaddr *)&this->address, (socklen_t*)&addrlen);
                        if (new_socket  < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                perror("In accept");
                                std::cout << "accept error" << std::endl;
                                exit(1);
                            }
                        }


                        std::cout << "**********" << new_socket <<std::endl;
                        if (serv.size() >= 1)
                        {
                            std::map<int, Servers>::iterator itt;
                            for(std::map<int, Servers>::iterator it = serv.begin(); it != serv.end(); it++)
                            {
                                if (it->first == new_socket)
                                {
                                    itt = it;
                                    break;
                                }
                            }
                            if (itt == serv.end())
                            {
                                FD_SET(new_socket, &master_set);
                                if (new_socket > max_fd)
                                    max_fd = new_socket;
                                Servers s("hi");
                                s.set_start(std::clock());
                                serv.insert(std::pair<int, Servers>(new_socket, s));
                                // fds.push_back(new_socket);
                            }

                        }
                        else
                        {
                            FD_SET(new_socket, &master_set);
                            if (new_socket > max_fd)
                                max_fd = new_socket;
                            Servers s("hi");
                            s.set_start(std::clock());
                            serv.insert(std::pair<int, Servers>(new_socket, s));
                            // fds.push_back(new_socket);
                        }
                }
                else
                {
                    std::string buffer_str;
                    char buffer[2];
                        int a = 0;
                        for(std::map<int, Servers>::iterator it = serv.begin(); it != serv.end(); it++)
                        {
                                a = recv(i, buffer, 1, 0);
                        }
                        if (a < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                perror("  recv() failed");
                            }
                            else if (errno == ECONNRESET)
                            {
                                printf("  Connection reset by peer\n");
                            }
                        }
                        buffer_str = buffer;
                        std::map<int, Servers>::iterator it = serv.begin();
                        int check = 1;
                        for (; it != serv.end(); it++)
                        {
                            if (it->first == i)
                            {

                                    it->second.collect_req(buffer_str);
                                check = 0;
                                break;
                            }

                        }
                        for (it = serv.begin(); it != serv.end();it++)
                        {
                            if (it->second.get_request().find(  "\r\n\r\n") != std::string::npos)
                            {
                            // std::cout << it->second.get_request() << std::endl; 
                                Message *resw =  this->resp.generateResponse(it->second.get_request());
                                it->second.set_responce_class(resw);
                                it->second.set_responce(it->second.get_responce_class()->getResponse());
                                FD_SET(it->first, &this->write_set1);
                                FD_CLR(it->first, &this->master_set);
                                if (it->first == max_fd)
                                {
                                    while (FD_ISSET(max_fd, &master_set) == 0)
                                        max_fd -= 1;
                                }
                            // std::cout << "++++++++++++++ "<<it->second.get_total() << " " << it->second.get_responce().length() << std::endl;
                                // if (it->second.get_total() == 0)
                                // {
                                //         int j = send(it->first, it->second.get_responce().c_str(), it->second.get_responce().length(), 0);
                                //         if (j < 0)
                                //         {
                                //             std::perror("sendd");
                                //         }
                                //         else
                                //             it->second.set_partis(j);
                                // }
                                // else
                                // {
                                //         int j = send(it->first, it->second.get_responce().c_str() + it->second.get_total(), it->second.get_responce().length() - it->second.get_total(), 0);
                                //         if (j < 0)
                                //         {

                                //             std::perror("sendd");
                                //         }
                                //         else
                                //             it->second.set_partis(j);
                                // }
                                // it->second.set_total(it->second.get_partis());
                                // std::cout << "hello: " << it->second.get_responce().length() << " chank "<< it->second.get_partis() << " total "<< it->second.get_total() <<" fd " << it->first << " size " << serv.size() <<std::endl;
                            // if (it->second.get_total() == it->second.get_responce().length())
                            // {
                            //     close(it->first);
                            //     FD_CLR(it->first, &master_set);
                            //     if (it->first == max_fd)
                            //     {
                            //         while (FD_ISSET(max_fd, &master_set) == 0)
                            //             max_fd -= 1;
                            //     }
                            //     it->second.set_total(0);
                            //     serv.erase(it);
                            // }
                            // if(serv.empty())
                            //     break;
                            }
                            // else
                            //     it++;
                        }
                }
            }
             if(FD_ISSET(i, &this->write_set1))
            {
                std::cout << "write_set1" << std::endl;
            }
        }
    }
} */



void Server::run()
{
    int max_fd = this->fds.back();
    int addrlen = sizeof(this->address);
    timeout.tv_sec  = 10;
    timeout.tv_usec = 0;
    int opt = 1;
    // unsigned long ress;
    FD_ZERO(&this->write_set1);
    FD_ZERO(&this->write_set2);
    // std::cout << "max_fd: " << max_fd << std::endl;

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
            std::cout << "timeout" << std::endl;
            // return;
        }
        int i = 0;
        while(i++ <= 1024 && activity > 0)
        {
            // std::cout << "iiiiiiiiiiii: " << std::endl;

            if (FD_ISSET(i, &this->working_set))
            {
                activity--;
                unsigned long y = find(this->fds.begin(), this->fds.end(), i) - this->fds.begin();
                if (y < fds.size() && i == this->fds[y])
                {
                    // std::cout << "new connection" << std::endl;
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
                    // std::cout << "**********" << new_socket <<std::endl;
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
                            // std::cout << "holaaaaaaa " << new_socket << std::endl;
                            FD_SET(new_socket, &this->master_set);
                            if (new_socket > max_fd)
                                max_fd = new_socket;
                            Servers s("");
                            s.set_start(std::clock());
                            this->serv2.push_back(std::pair<int, Servers>(new_socket, s));
                        }
                        // else if (ittt != this->serv2.end())
                        // {
                        //     std::cout << "holaaaaaaa_error " << new_socket <<std::endl;
                        //     // close(new_socket);
                        //     ittt->second.set_ma9bara(new_socket);
                        //     // FD_SET(new_socket, &this->master_set);
                        //     // if (new_socket > max_fd)
                        //     //     max_fd = new_socket;
                        //     // Servers s("hi");
                        //     // s.set_start(std::clock());
                        //     // this->serv.insert(std::pair<int, Servers>(new_socket, s));
                        // }
                    }
                    else
                    {
                        // std::cout << "holaaaaaaa " << new_socket <<std::endl;
                        FD_SET(new_socket, &this->master_set);
                        if (new_socket > max_fd)
                            max_fd = new_socket;
                        Servers s("");
                        s.set_start(std::clock());
                        this->serv2.push_back(std::pair<int, Servers>(new_socket, s));
                    }
                }
                else
                {
                    char buffer[1];
                    int a = recv(i, buffer, 1, 0);
                    if(a <= 0)
                    {
                        std::cout << "\033[91mconnection close by peer... \033[39m" << std::endl;
                        close(i);
                        FD_CLR(i, &this->master_set);
                        if (i == max_fd)
                        {
                            while (FD_ISSET(max_fd, &this->master_set) == 0)
                                max_fd -= 1;
                        }
                    }
                    for(std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end(); it++)
                    {
                        if (it->first == i)
                        {
                            // std::cout <<  std::endl;   
                            it->second.collect_req(buffer);
                            break;
                        }
                    }
                    for (std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end();)
                    {
                        if (it->first == i &&  it->second.get_request().find(  "\r\n\r\n") != std::string::npos)
                        {
                            Message *resw =  this->resp.generateResponse(it->second.get_request());
                            it->second.set_responce_class(resw);
                            it->second.set_responce(it->second.get_responce_class()->getResponse());
                            // std::cout << it->second.get_responce() << std::endl;
                            unsigned long  index = it->second.get_responce().find("\r\n\r\n");

                            if (index != std::string::npos)
                            {
                                // std::cout << "fondit......" << index <<std::endl;
                                index = it->second.get_responce().find("\0", index+4);
                                if (index != std::string::npos)
                                {
                                    FD_SET(it->first, &this->write_set1);
                                    // FD_SET(it->first, &this->status1);
                                    FD_CLR(it->first, &this->master_set);
                                    this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                            // std::cout << it->second.get_request() << std::endl;
                                    this->serv2.erase(it);
                                    
                                    if(serv2.empty() || serv2.size() == 0)
                                        break;
                                }
                                else
                                    it++;
                            }

                        }
                        else
                            it++;
                    }
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
                                std::cout << "\033[91mconnection close by peer... \033[39m" << std::endl;
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
                                std::cout << "\033[91mconnection close by peer... \033[39m" << std::endl;
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
                        // std::cout << "hello: " << it->second.get_responce().length() << " chank "<< it->second.get_partis() << " total "<< it->second.get_total() <<" fd " << it->first << " size " << this->msg.size() <<std::endl;
                        std::cout << "\033[92msend " << it->second.get_total() << " bytes to client " << it->first << " total of data is " << it->second.get_responce().length() << " bytes\033[39m" <<std::endl;
                        if (it->second.get_total() == it->second.get_responce().length())
                        {
                            // close(it->first);
                            FD_CLR(it->first, &this->write_set1);
                            FD_SET(it->first, &this->master_set);
                            // if (it->first == max_fd)
                            // {
                            //     while (FD_ISSET(max_fd, &this->master_set) == 0)
                            //         max_fd -= 1;
                            // }
                            it->second.rset_total(0);
                            it->second.set_responce("");
                            it->second.set_request("");
                            serv2.push_back(std::pair<int, Servers>(it->first, it->second));
                            this->msg.erase(it);
                            // if(this->timeout_client.empty())
                            //     this->timeout_client.insert(std::pair<int, Servers>(it->first, it->second));
                            // else
                            // {
                            //     if (timeout_client.find(it->first) == timeout_client.end())
                            //         this->timeout_client.insert(std::pair<int, Servers>(it->first, it->second));
                            //     else
                            //     {
                            //         std::map<int, Servers>::iterator itt = this->timeout_client.find(it->first);
                            //         this->timeout_client.erase(itt);
                            //         this->timeout_client.insert(std::pair<int, Servers>(it->first, it->second));
                            //     }
                            // }
                            // this->msg.erase(it);
                        }
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
