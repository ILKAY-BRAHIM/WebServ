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
    std::string str;

    FD_ZERO(&this->master_set);
            int index = 0;
    for (unsigned long i = 0; i < this->server.size(); i++)
    {
        std::vector<int>::iterator it = server[i].port.begin();
        for(; it !=  server[i].port.end(); it++)
        {
            int valid = 0;
            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            {
                std::cout << "socket failed" << std::endl;
                valid = 1;
            }
            if(valid == 0 && setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            {
                std::perror("setsockopt");
                valid = 1;
            }
            if (valid == 0 && fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
            {
                perror("ioctl");
                valid = 1;
            }
            this->address.sin_family = AF_INET;
            this->address.sin_addr.s_addr = INADDR_ANY;
            // this->address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            this->address.sin_port = htons(*it);
            memset(address.sin_zero, '\0', sizeof this->address.sin_zero);

            if (valid == 0 && bind(server_fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
            {
                std::endl(std::cout);
                std::cout << "\033[33mbind failed in port " << *it << std::endl;
                valid = 1;
            }
            if (valid == 0 && listen(server_fd, 20) < 0)
            {
                std::cout << "listen" << std::endl;
                valid = 1;
            }

            if (valid == 0)
            {
                if(str == "")
                    str = "\033[93mServer is running on ports " + std::to_string(*it);
                else
                    str += ", " + std::to_string(*it);
                std::cout << '\r' << std::string(80, ' ');
                std::cout << '\r' << str << "\033[39m" << std::flush;
                this->fds.push_back(server_fd);
                FD_SET(fds[index], &this->master_set);
                index++;
            }
        }
    }
    std::cout << std::endl;
    return;
}

void Server::run()
{

    if (this->fds.size() == 0)
        throw "empty server";
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
            throw "error in server ";
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
                    int valid = 0;
                    if (new_socket < 0)
                    {
                        std::cout << "accept error" << std::endl;
                        valid = 1;
                    }
                    if (valid == 0 && fcntl(new_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
                    {
                        perror("ioctl");
                        valid = 1;
                    }

                    if(valid == 0 && setsockopt(new_socket, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) < 0)
                    {
                        std::perror("setsockopt");
                        valid = 1;
                    }
                    if (valid == 0 && this->serv.size() >= 1)
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
                    else if (valid == 0)
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
                    for(std::vector<std::pair<int, Servers> >::iterator it = this->serv2.begin(); it != this->serv2.end(); it++)
                    {
                        if (it->first == i && it->second.get_redirection() == 0)
                        {
                            char buffer[1];
                            int a = 0;
                            a = recv(i, buffer, 1, 0);
                            if(a <= 0 )
                            {
                                std::cout << std::endl;
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
                                std::cout << it->second.get_request() << std::endl;
                                Message *resw =  this->resp.checkHeader(it->second.get_request()); // incoming changed..
                                it->second.set_responce_class(resw);
                                if(it->second.get_responce_class()->getContentLength() != 0)
                                    it->second.set_redirection(1);
                                else
                                {
                                    this->resp.generateResponse(it->second.get_responce_class());
                                    it->second.set_responce(it->second.get_responce_class()->getResponse());
                                    FD_SET(it->first, &this->write_set1);
                                    FD_CLR(it->first, &this->master_set);
                                    this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                                    this->serv2.erase(it);
                                }
                            }
                            break;
                        }
                        else if (it->first == i && it->second.get_redirection() == 1) // i need  indicate of error or not
                        {
                            char buffer[60000];
                            int a = recv(i, buffer, 60000, 0);
                            if(a <= 0)
                            {
                                std::cout << std::endl;
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
                            if (it->second.get_responce_class()->getStatus() == 0) // by pass body....
                            {
                                it->second.collect_body(buffer, a);
                                it->second.set_start(std::clock());
                                it->second.set_total_body(a);
                                static int count = 1;
                                print_log("recive " + std::to_string(it->second.get_total_body()) + " bytes from client " + std::to_string(it->first), "\033[92m", ++count, it->second.get_total_body(), it->second.get_responce_class()->getContentLength());
                            }
                            if(it->second.get_total_body() == (unsigned long)it->second.get_responce_class()->getContentLength()) // i need  indicate of chunked or not
                            {
                                it->second.get_responce_class()->setBody(it->second.get_body());
                                this->resp.generateResponse(it->second.get_responce_class());
                                it->second.set_responce(it->second.get_responce_class()->getResponse());
                                it->second.set_redirection(0);
                                std::cout << it->second.get_responce() << std::endl;
                                FD_SET(it->first, &this->write_set1);
                                FD_CLR(it->first, &this->master_set);
                                this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                                this->serv2.erase(it);
                            }
                            // if (it->second.get_body().find("\r\n\r\n") != std::string::npos) // i need  indicate of chunked or not
                            // {
                            //     // std::cout << "body: " << it->second.get_body() << std::endl;
                            //     it->second.get_responce_class()->setBody(it->second.get_body());
                            //     this->resp.generateResponse(it->second.get_responce_class());
                            //     it->second.set_responce(it->second.get_responce_class()->getResponse());
                            //     it->second.set_redirection(0);
                            //     std::cout << it->second.get_responce() << std::endl;
                            //     FD_SET(it->first, &this->write_set1);
                            //     FD_CLR(it->first, &this->master_set);
                            //     this->msg.insert(std::pair<int, Servers>(it->first, it->second));
                            //     this->serv2.erase(it);
                            // }
                            break;
                        }
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
                                std::cout << std::endl;
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
                                std::cout << std::endl;
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
                        std::cout << std::endl;
                        std::cout << "\033[92msend " << it->second.get_total() << " bytes to client " << it->first << " total of data is " << it->second.get_responce().length() << " bytes\033[39m" <<std::endl;
                        if (it->second.get_total() == it->second.get_responce().length())
                        {
                            FD_CLR(it->first, &this->write_set1);
                            FD_SET(it->first, &this->master_set);
                            it->second.rset_total(0);
                            it->second.set_responce("");
                            it->second.set_request("");
                            it->second.set_body("");
                            it->second.rset_total_body(0);
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


void Server::print_log(const std::string& str, const std::string& color, int count, unsigned long total, int content_length) {
    std::string str2 = str;
    (void)count;
    // int length = 0;
    // for (int i = 0; i < count; ++i) {
    //     if (i % 70 == 0)
    //     {
    //         str2 += "-";
    //         length++;
    //     }
    // }
    // str2 += ">";
    // int u = 74 - length;
    // for (int i = 0; i < u; ++i)
    // {
    //     str2 += " ";

    // }
    int j = (total * 100) / content_length;
    str2 += "  [";
    str2 += std::to_string(j);
    str2 += "%]";
    // Clear line by overwriting with spaces
    std::cout << '\r' << std::string(80, ' ');
    // Print the new message with color
    std::cout << '\r' << color << str2 << "\033[0m" << std::flush;
    // usleep(200);
}

Server::~Server()
{
}
