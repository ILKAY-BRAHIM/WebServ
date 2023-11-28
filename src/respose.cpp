#include "RequestResponce.hpp"

int pathCheck(std::string root)
{
    struct stat sb;
    if (stat(root.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        return (1);
    return (0);
}


void sendResponse(std::vector<t_server> servers , request req, int k)
{
    std::cout << "-------------------------RESPONSE-------------------------" << std::endl;
    (void)servers;
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
