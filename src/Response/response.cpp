
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
    // std::cout << "-------------------------REQUEST-------------------------" << std::endl;
    // std::cout << buffer << std::endl;
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
    // print_new_request(req);
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
    // std::cout << "redirection function" << std::endl;
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
    if (this->req.path == "/favicon.ico")
    {
        this->respMessage.Content_Lenght = "0";
        return ;
    }
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
    // std::cout << "Is directory" << std::endl;
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

// std::string getType(std::vector<t_types> types, std::string ext)
// {
//     std::vector<t_types>::iterator it = types.begin();
//     while (it != types.end())
//     {
//         std::map<std::string, std::vector<std::string> >::iterator type = it->text.begin();
//         while (type != it->text.end())
//         {
//             std::vector<std::string>::iterator extension = type->second.begin();
//             while (extension != type->second.end())
//             {

//             }
//         }

//         // std::map<std::string, std::vector<std::string> >::iterator jj = it->text.begin();
//         pause();
//     }
// }

void    Response::generateBody(std::string path)
{
    // check permition & read & generate body 
    std::string ext = get_extension(path);
    // ssize_t     bytesRead;

    if (ext.size() != 0)
    {
        if (ext == "html" || ext == "css" || ext == "js")
            this->respMessage.Content_Type = "text/" + ext;
        else if (ext == "png" || ext == "jpeg" || ext == "jpg")
            this->respMessage.Content_Type = "image/" + ext;
        else
            this->respMessage.Content_Type = "application/octet-stream";
    }
    else
        this->respMessage.Content_Type = "application/octet-stream";
    // int fd = open(path.c_str(), O_RDONLY);
    // int   bufferSize = 1000;
    // char* buffer = new char [bufferSize];
    std::ifstream fd(path.c_str());
    // if (fd == -1)
    // {
    //     generateBodyError(403);
    //     throw (403);
    //     return ;
    // }
    std::string line;
    // while ((bytesRead = read(fd, buffer, bufferSize)) > 0)
    //     this->respMessage.body.append(buffer, bytesRead);
    while (getline(fd, line))
        this->respMessage.body += line;
    this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());

    // close (fd);
    fd.close();
    // delete[] buffer;
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

t_server    Response:: fillServer(std::string req, request reqq)
{
    (void) req;
    std::string	port;
    std::string	server;
    std::string	host;
	size_t		found;
	std::vector<t_server>::iterator it;
	std::vector<int>::iterator itt;
    t_server    tmp_server;

    // this->req = parseRequest(req);
	host = reqq.headers["Host"];
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
					tmp_server = *it;
					found = 1;
					break;
				}
				itt++;
			}
		}
		it++;
	}
    return (tmp_server);
}


std::string Response::generateMessage()
{
    std::string mess;

    mess += this->respMessage.http_version;
    mess += ' ';
    mess += this->respMessage.statusCode;
    mess += std::string(CRLF);
    if (this->respMessage.Content_Type.size() != 0)
    {
        mess += std::string("Content-Type: ");
        mess += this->respMessage.Content_Type;
        mess += std::string(CRLF);
    }
    if (this->respMessage.Content_Lenght.size() != 0)
    {
        mess += std::string("Content-Length: ");
        mess += this->respMessage.Content_Lenght;
        mess += std::string(CRLF);
    }
    if (this->respMessage.Location.size() != 0)
    {
        mess += std::string("Location: ");
        mess += this->respMessage.Location;
        mess += std::string(CRLF);
    }
    // add other headrs 
    //  ...
    mess += std::string(CRLF);
    if (this->respMessage.body.size() != 0)
        mess += this->respMessage.body;
    return mess;
}

void    Response::clearResponse()
{
    this->respMessage.http_version.clear();
    this->respMessage.statusCode.clear();
    this->respMessage.Access_Controle_Allow_Origin.clear();
    this->respMessage.Cache_Control.clear();
    this->respMessage.Content_Type.clear();
    this->respMessage.Content_Lenght.clear();
    this->respMessage.ETag.clear();
    this->respMessage.Last_Modified.clear();
    this->respMessage.Location.clear();
    this->respMessage.Set_Cookie.clear();
    this->respMessage.Server.clear();
    this->respMessage.query_String.clear();
    this->respMessage.body.clear();

    this->req.method.clear();
    this->req.path.clear();
    this->req.httpVertion.clear();
    this->req.headers.clear();
    this->req.body.clear();

}

void    Response::generateResponse(Message* mes)
{
    std::string line;
    this->server =  mes->getServer();
    this->req = mes->getRequest();
    mes->setStatus(1);
    this->respMessage.http_version = "HTTP/1.1";
	try
	{
		checkMethode();
        urlRegenerate();
	}
	catch(int m)
	{
        mes->setStatus(-1);
		this->respMessage.statusCode = generateStatusCode(m);
        // std::cout << this->respMessage.statusCode << std::endl;
		// exit(1);
	}
    
    mes->setResponse(generateMessage());
    // std::cout << "Content-Length: " << mes->getContentLength() << std::endl;
    clearResponse();
    // http version
    // method type allowed & type component
    // url syntax & sources permission 
    // std::cout << mes->getResponse() << std::endl;
    // return (mes);

}

Message* Response::checkHeader(std::string request_)
{
    Message *mes = new Message();

    mes->setRequest(parseRequest(request_));
    mes->setServer(fillServer(request_, mes->getRequest()));
    // t_server    tmp_server = fillServer(request_);
    int content_length = 0;

    unsigned long  index1 = request_.find("Content-Length: ");
    if (index1 != std::string::npos)
    {
        unsigned long  index2 = request_.find("\r", index1);
        if ( index2 != std::string::npos)
        {
            std::string str = request_.substr(index1 + 16 , index2 - index1 - 16);
            content_length = atoi(str.c_str()); // must be deleted
            mes->setContentLength(atoi(str.c_str()));
        }
    }
    else
    {
        content_length = 0; // must be deleted
        mes->setContentLength(0);
    }
    
    

    // return (content_length); // must be return a message
    return (mes);
}

Response::~Response(){};

    