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
                    while(1)
                    {
                        int new_socket = accept(server_fd, (struct sockaddr *)&this->address, (socklen_t*)&addrlen);
                        if (new_socket  < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                perror("In accept");
                                exit(1);
                            }
                            break;
                        }
                        FD_SET(new_socket, &master_set);
                        if (new_socket > max_fd)
                            max_fd = new_socket;
                    }
                }
                else
                {
                    int close_conn = 0;
                    std::string buffer_str = "";
                    char buffer[2];
                    while(1)
                    {
                        int a = recv(i, buffer, 1, 0);
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
                            break;
                        }
                        if (a == 0)
                        {
                            printf("  Connection closed\n");
                            close_conn = 1;
                            break;
                        }
                        buffer_str += buffer;
                    }

                    std::cout << "buffer: " << buffer_str << std::endl;
                    // char *h = (char *)buffer_str.c_str();
                    // const char *g = hello.c_str();
                    // parse(h, i);
                    // test_cgi(h, i);
                    buffer_str = "";
                    if (close_conn )
                    {
                        close(i);
                        FD_CLR(i, &master_set);
                        if (i == max_fd)
                        {
                            while (FD_ISSET(max_fd, &master_set) == 0)
                                max_fd -= 1;
                        }
                    }
                }
            }
        }
    }
}

Server::~Server()
{
}
