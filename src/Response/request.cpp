// Server side C program to demonstrate HTTP Server programming

// #include "RequestResponce.hpp"
#include "response.hpp"

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

        Response resp(servers);
        std::string r(requestt);
        resp.generateResponse(r);
        

        // request req = parseRequest(requestt);

        // sendResponse(servers, req, new_socket);
        // sleep(2);
        close(new_socket);
    }
    return 0;
}