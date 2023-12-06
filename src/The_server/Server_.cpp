#include "Server_.hpp"

Server::Server(std::vector<t_server> server)
{
    this->server = server;
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
            std::cout << *it << std::endl;
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
            this->address.sin_port = htons(*it);
            memset(address.sin_zero, '\0', sizeof this->address.sin_zero);

            if (bind(server_fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
            {
                std::cout << "bind failed" << std::endl;
                return;
            }
            if (listen(server_fd, 3) < 0)
            {
                std::cout << "listen" << std::endl;
                return;
            }

            std::cout << "Server is running on port " << *it << std::endl;

            this->fds.push_back(server_fd);
            std::cout << "fds: " << this->fds[index] << std::endl;
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
    timeout.tv_sec  = 1 * 60;
    timeout.tv_usec = 0;
    unsigned long ress;
    ress = 0;
    

    while(1)
    {
        memcpy(&this->working_set, &this->master_set, sizeof(this->master_set));
        int activity = select(max_fd + 1, &this->working_set, NULL, NULL, &this->timeout);
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
                int y = find(this->fds.begin(), this->fds.end(), i) - this->fds.begin();
                int server_fd = this->fds[y];
                if (i == server_fd)
                {
                    // while(1)
                    // {
                        std::cout << "new connection" << std::endl;
                        int new_socket = accept(server_fd, (struct sockaddr *)&this->address, (socklen_t*)&addrlen);
                        // serv.insert(std::pair<int, servers>(new_socket, ""));
                        // serv.push_back(std::map<int, Servers>(new_socket, servers()));
                        if (new_socket  < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                perror("In accept");
                                exit(1);
                            }
                            // break;
                        }
                        FD_SET(new_socket, &master_set);
                        if (new_socket > max_fd)
                            max_fd = new_socket;
                    // }
                }
                else
                {
                    int close_conn = 0;
                    std::string buffer_str;
                    char buffer[2];
                    // while(1)
                    // {
                        int a = recv(i, buffer, 1, 0);
                        buffer_str = buffer;
                        // std::cout << "buffer: " << buffer_str << std::endl;
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
                        if (check)
                            serv.insert(std::pair<int, Servers>(i, Servers(buffer_str)));
                        // std::cout << "size: " << serv.size() << std::endl;
                        for (it = serv.begin(); it != serv.end();)
                        {
                            // std::cout << "fd->: " << it->first << std::endl;
                            // std::cout << "request->: " << it->second.get_request() << std::endl;
                            if (it->second.get_request().find(  "\r\n\r\n") != std::string::npos)
                            {
                                // std::cout << "find" << std::endl;
                                // std::cout << "request: " << it->second.get_request() << std::endl;
                                // std::cout << "fd: " << it->first << std::endl;
                                // parse(it->second.get_request(), it->first);
                                // test_cgi(it->second.get_request(), it->first);
                                // it->second.get_request() = "";
                                // serv.erase(it);


                                std::cout << "hiiiiiiiiiiii" << std::endl;
                                std::ifstream file("/Users/bchifour/Desktop/WEBSERV/src/The_server/fill.txt");
                                if (!file.is_open())
                                {
                                    // std::cout << "file not open" << std::endl;
                                    std::perror("file not open");
                                    return;
                                }
                                std::string hello;
                                std::string line;
                                while (getline(file, line))
                                {
                                    hello += line;
                                }
                                file.close();


                                std::string res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>"+hello + "</h1></body></html>";




                                unsigned long h;
                                if (ress == 0)
                                      h = send(it->first, res.c_str(), res.length(), 0);
                                else
                                      h = send(it->first, res.c_str() + ress, res.length() - ress, 0);
                                ress += h;
                                std::cout << "hello: " << res.length() << " chank "<< h << " total "<< ress <<" fd " << it->first << " size " << serv.size() <<std::endl;
                                if (ress == res.length())
                                {

                                    close(it->first);
                                    FD_CLR(it->first, &master_set);
                                    if (it->first == max_fd)
                                    {
                                        while (FD_ISSET(max_fd, &master_set) == 0)
                                            max_fd -= 1;
                                    }
                                    serv.erase(it);
                                }
                                if(serv.empty())
                                    break;
                            }
                            else
                                it++;

                        
                        }

                        if (a < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                perror("  recv() failed");
                                close_conn = 1;
                            }
                            else if (errno == ECONNRESET)
                            {
                                printf("  Connection reset by peer\n");
                                close_conn = 1;
                            }
                            close_conn = 1;
                            // break;
                        }
                        if (a == 0)
                        {
                            printf("  Connection closed\n");
                            // break;
                        }
                            // close_conn = 1;
                        // buffer_str += buffer;
                    // }

                    // std::cout  << buffer;
                    // char *h = (char *)buffer_str.c_str();
                    // const char *g = hello.c_str();
                    // parse(h, i);
                    // test_cgi(h, i);
                    // buffer_str = "";
                    // if (a )
                    // {
                    //     close(i);
                    //     FD_CLR(i, &master_set);
                    //     if (i == max_fd)
                    //     {
                    //         while (FD_ISSET(max_fd, &master_set) == 0)
                    //             max_fd -= 1;
                    //     }
                    // }
                }
            }
        }
    }
}

Server::~Server()
{
}
