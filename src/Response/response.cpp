
#include "response.hpp"
#include <sys/stat.h>

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

request parseRequest(std::string buffer)
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
    while (std::getline(ss, line, '\n'))
    {
        if (line.find(':') != std::string::npos)
            getHeader(req, line);
        else
            break;
    }
    return (req);
}


Response::Response(void){};

Response::Response(std::vector<t_server> servS)
{
    this->servS = servS;
}

// Response::Response(const Response& copy){};

std::string generateStatusCode(int status)
{
    std::string message;

    if (status >= 200 && status < 300)
    {
        switch(status) {
        case 200:
            message =  std::to_string(status) + " OK";
            break;
        case 201:
            message =  std::to_string(status) + " Created";
        }
    }
    else if (status >= 300 && status < 400)
    {
        switch(status) {
        case 301:
            message =  std::to_string(status) + " Moved Permanently";
            break;
        case 302:
            message =  std::to_string(status) + " Found";
        }
    }
    else if(status >= 400 && status < 500)
    {
        switch(status) {
        case 400:
            message =  std::to_string(status) + " Bad Request";
            break;
        case 403:
            message =  std::to_string(status) + " Forbidden";
            break;
        case 404:
            message =  std::to_string(status) + " Not Found";
            break;
        case 405:
            message =  std::to_string(status) + " Method Not Allowed";
        }
    }
    else if (status >= 500 && status < 600)
    {
        switch(status) {
        case 502:
            message =  std::to_string(status) + " Bad Gateway";
            break;
        case 505:
            message =  std::to_string(status) + " HTTP Version Not Supported";
        }
    }
    return (message);
}

void	Response::checkMethode() // ->status_code & ->server 
{
    if (!(this->req.method == "GET" || this->req.method == "POST" || this->req.method == "DELETE"))
    {
        generateBodyError(501);
        throw (501);
    }
    if (this->req.httpVertion != "HTTP/1.1")
    {
        generateBodyError(505);
        throw (505);
    }
    this->respMessage.statusCode = "200 OK"; // is possible to chage
	this->respMessage.Server = "Not nginx/0.0.0 (macOS)";
}

void    Response::redirect(std::string path)
{
    // most send a response with redirect code status
    std::cout << "redirection function" << std::endl;
    this->respMessage.statusCode = generateStatusCode(302);
    this->respMessage.Location = path + '/';
    return ;
}

int     Response::getLocation(std::string url)
{
    std::vector<t_location>::iterator it = this->server.locations.begin();

    while (it != this->server.locations.end())
    {
        if (url == it->path)
        {
            this->location = *it;
            return (1);
        }
        it++;
    }
    return (0);
}

void     Response::generateBodyError(int error)
{
    this->respMessage.Content_Type = "text/html";
    this->respMessage.body += "<!DOCTYPE html>\n<html lang=\"en\">\n";
    this->respMessage.body += "<html>\n";
    this->respMessage.body += ("<head><title>" + generateStatusCode(error) + "</title></head>\n");
    this->respMessage.body += "<body>\n";
    this->respMessage.body += ("<center><h1>" + generateStatusCode(error) + "</h1></center>\n");
    this->respMessage.body += ("<hr><center>" + this->respMessage.Server + "</center>\n");
    this->respMessage.body += "</body>\n";
    this->respMessage.body += "</html>";
    this->respMessage.Content_Lenght = std::to_string(this->respMessage.body.length());

}

void    Response::isDirectory(std::string path, std::string url)
{
    std::cout << "Is directory" << std::endl;
    std::string index_;
    (void)path;
    // check if the directory in the log
    if (url != "/")
        url.erase(url.end() - 1);
    if (getLocation(url))
    {
        if (this->location.index.size() != 0)
            index_ = get_index(this->location, path, 0);
        else 
        {
            if (this->server.index.size() != 0)
                index_ = get_index(this->server, path, 0);
            else 
                index_ = get_index(this->server, path, 1);
        }
        if (index_.length() != 0)
            generateBody(path + index_);
        else
        {
            generateBodyError(403);
            throw (403);
        }
    }
    else if (url == "/")
    {
        index_ = get_index(this->server, path, 1);
        if (index_.length() != 0)
            generateBody(path + index_);
        else
        {
            generateBodyError(403);
            throw (403);
        }

    }
    else
    {
        generateBodyError(403);
        throw (403);
    }

    // check if there is an autoindex
    // generate body || send an error
    return ;
}

std::string get_extension(std::string path)
{
    std::string::iterator it = path.end() - 1;
    std::string extension;

    while (it != path.begin() && *it != '/')
    {
        if (*it != '.')
        {
            extension = *it + extension;
            it--;
        }
        else
        {
            extension = '.' + extension;
            break;
        }
    }
    if (*extension.begin() == '.')
    {
        extension.erase(extension.begin());
        return extension;
    }
    return "";
}

void    Response::generateBody(std::string path)
{
    // check permition & read & generate body 
    std::string ext = get_extension(path);
    ssize_t     bytesRead;
    if (ext.size() != 0)
    {
        if (ext == "html" || ext == "css" || ext == "js")
            this->respMessage.Content_Type = "text/" + ext;
        else if (ext == "png" || ext == "jpeg" || ext == "jpg")
            this->respMessage.Content_Type = "image/" + ext;
        else
            this->respMessage.Content_Type = "application/octet-stream";
    }
    int fd = open(path.c_str(), O_RDONLY);
    int   bufferSize = 1000;
    char* buffer = new char [bufferSize];

    if (fd == -1)
    {
        generateBodyError(403);
        throw (403);
        return ;
    }
    
    while ((bytesRead = read(fd, buffer, bufferSize)) > 0)
        this->respMessage.body.append(buffer, bytesRead);
    this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());
    close (fd);
    delete[] buffer;
    this->respMessage.statusCode = generateStatusCode(200);
    return ;
}


void	Response::urlRegenerate() // ->status_code & ->Location 
{
	//get Path
	std::string parameters;
	std::string path;
    std::string url;
	size_t found = this->req.path.find('?');
	if (found != std::string::npos)
	{
		parameters = req.path.substr(found);
		url = req.path.substr(0, found);
	}
	else
		url = req.path;
    path = this->server.root + url;
    
    if (access(path.c_str(), F_OK) == 0)
    {
        struct stat fileStat;
        if (!(stat(path.c_str(), &fileStat) == 0))
        {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            if (*(path.end() - 1) != '/')
                redirect(url);
            else
                isDirectory(path, url);
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            generateBody(path);
        }
    }   
    else
    {
        generateBodyError(404);
        throw (404);
    }
    // else if ((this->req.headers).find("Referer") != this->req.headers.end())
    // {
        
    //     {
            
    //     }

    // }
    // GET / HTTP/1.1
    // GET /index.html HTTP/1.1
    // GET /index.html/ HTTP/1.1



	// check Path


    // pause();

    // std::cout << "Parameters : " << parameters << std::endl;
    // std::cout << "Path : " << path << std::endl;

    // pause();
	//encode parameters
}

void    Response::fillServer(std::string req)
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


std::string Response::generateMessage()
{
    std::string mess;

    mess += this->respMessage.http_version;
    mess += ' ';
    mess += this->respMessage.statusCode;
    mess += CRLF;
    if (this->respMessage.Content_Type.size() != 0)
    {
        mess += "Content-Type: ";
        mess += this->respMessage.Content_Type;
        mess += CRLF;
    }
    if (this->respMessage.Content_Lenght.size() != 0)
    {
        mess += "Content-Lenght: ";
        mess += this->respMessage.Content_Lenght;
        mess += CRLF;
    }
    if (this->respMessage.Location.size() != 0)
    {
        mess += "Location: ";
        mess += this->respMessage.Location;
        mess += CRLF;
    }
    // add other headrs 
    //  ...
    mess += CRLF;
    if (this->respMessage.body.size() != 0)
        mess += this->respMessage.body;
    return mess;
}


Message*    Response::generateResponse(std::string req)
{
    Message *mes = new Message();
    
    fillServer(req);
    this->respMessage.http_version = "HTTP/1.1";
	try
	{
		checkMethode();
        urlRegenerate();
	}
	catch(int m)
	{
		this->respMessage.statusCode = generateStatusCode(m);
        std::cout << this->respMessage.statusCode << std::endl;
		// exit(1);
	}
    
    mes->setResponse(generateMessage());
    // http version
    // method type allowed & type component
    // url syntax & sources permission 
    std::cout << mes->getResponse() << std::endl;
    return (mes);

}

Response::~Response(){};




































// int pathCheck(std::string root)
// {
//     struct stat sb;
//     if (stat(root.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
//         return (1);
//     return (0);
// }

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



    