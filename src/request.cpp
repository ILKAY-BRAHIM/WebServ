// Server side C program to demonstrate HTTP Server programming

#include "RequestResponce.hpp"

void    print_new_request(request req)
{
    std::cout << "Method: " << req.method << std::endl;
    std::cout << "Path: " << req.path << std::endl;
    std::cout << "HttpVertion: " << req.httpVertion << std::endl;
    std::cout << "Headers: " << std::endl;
    for (std::map<std::string, std::string>::iterator it=req.headers.begin(); it!=req.headers.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    std::cout << "Body: " << req.body << std::endl;
}

void    getHeader(request &req, std::string line)
{
    std::istringstream ss(line);
    std::string value;

    std::getline(ss, line, ':');
    std::getline(ss, value, '\r');
    value.erase(value.begin());
    req.headers[line] = value;
}

request parseRequest(char *buffer)
{
    request req;
    std::string line;
    std::string value(buffer);
    std::istringstream ss(value);
    std::getline(ss, line, '\n');
    
    //get start line :
    std::istringstream st(line);
    std::getline(st, line, ' ');
    req.method = line;
    std::getline(st, line, ' ');
    req.path = line;
    std::getline(st, line, ' ');
    req.httpVertion = line;

    while (std::getline(ss, line, '\n'))
    {
        if (line.find(':') != std::string::npos)
            getHeader(req, line);
        else
            break;
    }
    while (std::getline(ss, line, '\n'))
    {
        req.body += line;
        req.body += "\n";
    }
    // print_new_request(req);
    return (req);
}


void sendResponse(std::vector<t_server> servers , request req, int k)
{
    (void)servers;
    std::cout << "-------------------------RESPONSE-------------------------" << std::endl;
    if (req.path == "/")
    {
        std::string p = servers[0].root + "/" + servers[0].index;
        printf("path : %s\n", p.c_str());
        int fd = open(p.c_str(), O_RDONLY);
        if (fd == -1)
        {
            std::cout << "Error : open file" << std::endl;
            return ;
        }
        char g[400000];
        read( fd , g, 400000);
        int size = strlen(g);
        std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\n" + g;
        write(k , hello.c_str() , hello.length());
        // write(1 , hello.c_str() , hello.length());
    }
    // pause();
    // else
    // {
        
    // }
}



int get_request(std::vector<t_server> servers)
{
    (void)servers;
    // printServers(servers); // data from conf

    // running server :
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    // Set SO_REUSEADDR option
    int optval = 1;
    if (setsockopt(server_fd , SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char requestt[30000] = {0};
        valread = read( new_socket , requestt, 30000);
        printf("-------------------------REQUEST-------------------------\n");
        printf("%s\n", requestt);
        request req = parseRequest(requestt);
        sendResponse(servers, req, new_socket);
        // sleep(2);
        close(new_socket);
    }
    return 0;
}