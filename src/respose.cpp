#include "RequestResponce.hpp"
#include "response.hpp"

Response::Response(void){};

Response::Response(std::vector<t_server> servS)
{
    this->servers = servS;
}

// Response::Response(const Response& copy){};

// void	Response::checkMethode() // ->status_code & ->server 
// {
//     if (!(this->req.method == "GET" || this->req.method == "POST" || this->req.method == "DELETE"))
//         throw (501);
//     if (this->req.httpVertion != "HTTP/1.1")
//         throw (505);
//     this->respMessage.statusCode = 200; // is possible to chage
// 	this->respMessage.Server = "WebServer"; 
// }

// void	Response::urlRegenerate() // ->status_code & ->Location 
// {
// 	//get Path
// 	std::string parameters;
// 	std::string path;
// 	size_t found = req.path.find('?');
// 	if (found != std::string::npos)
// 	{
// 		parameters = req.path.substr(found);
// 		path = req.path.substr(0, found);
// 	}
// 	else
// 		path = req.path;

// 	// check Path

// 	//encode parameters
// }

// // void	readPath(); // ->content_linght & ->content_type & ->last_modification & ->status_code

// void    Response::generateResponse(std::string ServerName, std::string port)
// {
//     this->respMessage.http_version = "HTTP/1.1";
// 	try
// 	{
// 		checkMethode();

// 	}
// 	catch(int m)
// 	{
// 		std::cout << "status Code : " << m << std::endl;
// 		exit(1);
// 	}
//     // http version
//     // method type allowed & type component
//     // url syntax & sources permission 

// }

Response::~Response(){};




































int pathCheck(std::string root)
{
    struct stat sb;
    if (stat(root.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        return (1);
    return (0);
}

// generate a generat_StartLine that check the server & the location & the path ;
// retune a file descriptor to the there is a body or -1 if error
// generate a generateHeaders function;

// generateStartLine finction : it's fill a responseStruct part fo start_line;
    // check if the server exist
    // check if the location exist
    // check if the path exist
    // check if the path is a file or a directory
    // check the permition
    // check the method
       // => generate a StartLine

// generateHeader function :

void sendResponse(std::vector<t_server> servers , request req, int k)
{
    std::cout << "-------------------------RESPONSE-------------------------" << std::endl;
    (void)servers;
    if (req.path == "/")
    {
        std::string p = servers[0].root + "/" + servers[0].index;
        // std::string p = servers[0].root + "/index.html";
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
        std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + g;
        write(k , hello.c_str() , hello.length());
        write(1 , hello.c_str() , hello.length());
    }
    else if (req.path == "/index.html")
    {
        // std::string p = servers[0].root + "/" + servers[0].index;
        std::string p = servers[0].root + "/index.html";
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
        std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + g;
        write(k , hello.c_str() , hello.length());
        write(1 , hello.c_str() , hello.length());
    }
    else if (req.path == "/src/form/index.html")
    {
        std::string p = servers[0].root + "/src/form/index.html";
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
        std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + g;
        write(k , hello.c_str() , hello.length());
        write(1 , hello.c_str() , hello.length());
    }
    // pause();
    // else
    // {
        
    // }
}



    