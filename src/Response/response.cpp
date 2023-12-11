
#include "response.hpp"

void    print_new_request(request req)
{
    std::cout << "Method: " << req.method << std::endl;
    std::cout << "Path: " << req.path << std::endl;
    std::cout << "HttpVertion: " << req.httpVertion << std::endl;
    std::cout << "Headers: " << std::endl;
    for (std::map<std::string, std::string>::iterator it=req.headers.begin(); it!=req.headers.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    // std::cout << "Body: " << req.body << std::endl;
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
    std::getline(st, line, '\r');
    req.httpVertion = line;
    // size_t found = req.httpVertion.find('\r');
    // if (found != std::string::npos)
    //     req.httpVertion.erase(found, 1);
    while (std::getline(ss, line, '\n'))
    {
        if (line.find(':') != std::string::npos)
            getHeader(req, line);
        else
            break;
    }
    // while (std::getline(ss, line, '\n'))
    // {
    //     req.body += line;
    //     req.body += "\n";
    // }
    // print_new_request(req);
    return (req);
}


Response::Response(void){};

Response::Response(std::vector<t_server> servS)
{
    this->servS = servS;
}



// Response::Response(const Response& copy){};

void	Response::checkMethode() // ->status_code & ->server 
{
    if (!(this->req.method == "GET" || this->req.method == "POST" || this->req.method == "DELETE"))
        throw (501);
    if (this->req.httpVertion != "HTTP/1.1")
        throw (505);
    this->respMessage.statusCode = 200; // is possible to chage
	this->respMessage.Server = "WebServer"; 
}

void	Response::urlRegenerate() // ->status_code & ->Location 
{
	//get Path
	std::string parameters;
	std::string path;
	size_t found = this->req.path.find('?');
	if (found != std::string::npos)
	{
		parameters = req.path.substr(found);
		path = req.path.substr(0, found);
	}
	else
		path = req.path;

	// check Path
    if ((this->req.headers).find("Referer") == this->req.headers.end())
    {
        path = this->server.root + path;
        std::cout << "path : " << path << std::endl;
    }


    // pause();

    // std::cout << "Parameters : " << parameters << std::endl;
    // std::cout << "Path : " << path << std::endl;


	//encode parameters
}

// void	Response::readPath() // ->content_linght & ->content_type & ->last_modification & ->status_code
// {

// }

void    Response::fillServer(char *req)
{
    std::string	port;
    std::string	server;
    std::string	host;
	size_t		found;
	std::vector<t_server>::iterator it;
	std::vector<int>::iterator itt;

    this->req = parseRequest(req);
	host = this->req.headers["Host"];
	found = host.find(':');
	if (found != std::string::npos)
	{
		port = host.substr(found+1);
		server = host.substr(0, found);
	}
	else
	{
		server = host;
		port = "80";
	}
	found = 0;
	it = this->servS.begin();
	while (it != this->servS.end() && found != 1)
	{
		if (it->name == server)
		{
			itt = it->port.begin();
			while (itt != it->port.end())
			{
				if (*itt == atoi(port.c_str()))
				{
					this->server = *it;
					found = 1;
					break;
				}
				itt++;
			}
		}
		it++;
	}
}

Message*    Response::generateResponse(std::string req)
{
    (void)req;
    Message *mes = new Message();

    // fillServer(req);

    // this->respMessage.http_version = "HTTP/1.1";
	// try
	// {
	// 	checkMethode();
    //     urlRegenerate();
    //     // readPath();
	// }
	// catch(int m)
	// {
	// 	std::cout << "status Code : " << m << std::endl;
	// 	exit(1);
	// }

    // http version
    // method type allowed & type component
    // url syntax & sources permission 

    return (mes);

}

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

// void sendResponse(std::vector<t_server> servers , request req, int k)
// {
//     std::cout << "-------------------------RESPONSE-------------------------" << std::endl;
//     (void)servers;
//     if (req.path == "/")
//     {
//         std::string p = servers[0].root + "/" + servers[0].index;
//         // std::string p = servers[0].root + "/index.html";
//         printf("path : %s\n", p.c_str());
//         int fd = open(p.c_str(), O_RDONLY);
//         if (fd == -1)
//         {
//             std::cout << "Error : open file" << std::endl;
//             return ;
//         }
//         char g[400000];
//         read( fd , g, 400000);
//         int size = strlen(g);
//         std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + g;
//         write(k , hello.c_str() , hello.length());
//         write(1 , hello.c_str() , hello.length());
//     }
//     else if (req.path == "/index.html")
//     {
//         // std::string p = servers[0].root + "/" + servers[0].index;
//         std::string p = servers[0].root + "/index.html";
//         printf("path : %s\n", p.c_str());
//         int fd = open(p.c_str(), O_RDONLY);
//         if (fd == -1)
//         {
//             std::cout << "Error : open file" << std::endl;
//             return ;
//         }
//         char g[400000];
//         read( fd , g, 400000);
//         int size = strlen(g);
//         std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + g;
//         write(k , hello.c_str() , hello.length());
//         write(1 , hello.c_str() , hello.length());
//     }
//     else if (req.path == "/src/form/index.html")
//     {
//         std::string p = servers[0].root + "/src/form/index.html";
//         printf("path : %s\n", p.c_str());
//         int fd = open(p.c_str(), O_RDONLY);
//         if (fd == -1)
//         {
//             std::cout << "Error : open file" << std::endl;
//             return ;
//         }
//         char g[400000];
//         read( fd , g, 400000);
//         int size = strlen(g);
//         std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(size) +"\r\n\r\n" + g;
//         write(k , hello.c_str() , hello.length());
//         write(1 , hello.c_str() , hello.length());
//     }
//     // pause();
//     // else
//     // {
        
//     // }
// }



    