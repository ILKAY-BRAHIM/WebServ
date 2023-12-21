
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

    std::string parameters;
	std::string path;
    std::string url;
	size_t found = req.path.find('?');
	if (found != std::string::npos)
	{
		parameters = req.path.substr(found+1);
        // std::cout << "parameters : " << parameters << std::endl;
		url = req.path.substr(0, found);
	}
	else
		url = req.path;
    req.path = url;
    req.headers["Query-String"] = parameters;
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

Response::Response(std::vector<t_server> servS, char **env)
{
    this->servS = servS;
    this->env = env;
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
            break;
        case 413:
            message =  std::to_string(status) + " Payload Too Large";
            break;
        case 415:
            message =  std::to_string(status) + " Unsupported Media Type";
            break;
        case 414:
            message =  std::to_string(status) + " URI Too Long";
            break;
        }
    }
    else if (status >= 500 && status < 600)
    {
        switch(status) {
        case 500:
            message =  std::to_string(status) + " Internal Server Error";
            break;
        case 501:
            message =  std::to_string(status) + " Not Implemented";
            break;
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
    this->respMessage.Content_Lenght = "0";
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
    std::string fullPath;
    // check if the directory in the log
    if (url != "/")
        url.erase(url.end() - 1);
    if (getLocation(url))
    {
        if (this->location.cgi_index.size() != 0)
        {
            std::vector<std::string>::iterator it = this->location.cgi_index.begin();
            while (it != this->location.cgi_index.end())
            {
                fullPath = path + (*it);
                if (access((fullPath).c_str(), F_OK | X_OK) == 0)
                {
                    index_ = *it;
                    break;
                }
                it++;
            }
        }
        else if (this->location.index.size() != 0)
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

std::string file_type(std::vector<std::map<std::string, std::vector<std::string> > > media, std::string ext)
{
    std::vector<std::map<std::string, std::vector<std::string> > >::iterator type = media.begin();

    while (type != media.end())
    {
        std::map<std::string, std::vector<std::string> >::iterator it = type->begin();
        while (it != type->end())
        {
            std::vector<std::string>::iterator itt = it->second.begin();
            while (itt != it->second.end())
            {
                if (*itt == ext)
                    return (it->first);
                itt++;
            }
            it++;
        }
        type++;
    }
    return "";
}

std::string getType(std::vector<t_types> types, std::string ext)
{
    if (ext.size() == 0)
        return ("application/octet-stream");
    if (ext.size() != 0)
    {
        if (ext == "php" || ext == "py")
            return (ext);
    }
    std::vector<t_types>::iterator it = types.begin();
    std::vector<std::map<std::string, std::vector<std::string> > > media;
    std::string type ;
    while (it != types.end())
    {
        media.push_back(it->text);
        media.push_back(it->application);
        media.push_back(it->image);
        media.push_back(it->video);
        media.push_back(it->audio);
        type = file_type(media, ext);
        if (type.size() != 0)
            return (type);
        it++;    
        media.clear();
    }
    return "application/octet-stream";
}

int checkMimeType(std::vector<t_types> types, std::string type)
{
    std::vector<t_types>::iterator it = types.begin();
    std::vector<std::map<std::string, std::vector<std::string> > > media;
    std::string tmp;
    while (it != types.end())
    {
        media.push_back(it->text);
        media.push_back(it->application);
        media.push_back(it->image);
        media.push_back(it->video);
        media.push_back(it->audio);
        std::vector<std::map<std::string, std::vector<std::string> > >::iterator type_ = media.begin();
        while (type_ != media.end())
        {
            std::map<std::string, std::vector<std::string> >::iterator itt = type_->begin();
            while (itt != type_->end())
            {
                if (itt->first == type)
                {
                    std::cout << "type : " << itt->first << std::endl;
                    return (1);
                }
                itt++;
            }
            type_++;
        }
        it++;
        media.clear();
    }
    return (0);
}

// std::string generate

void    Response::uploadFile()
{
    if (checkMimeType(this->server.types, this->req.headers["Content-Type"]))
    {
        //must generate a generator name
        std::string tmpFile;
        int found = this->req.headers["Content-Type"].find("/");
        std::string ext = this->req.headers["Content-Type"].substr(found + 1);
        tmpFile = "./upload/tmp." + ext;
        int fd = open(tmpFile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
        write(fd, this->body.c_str(), this->body.length());
        close(fd);
    }
    else
    {
        generateBodyError(415);
        throw (415);
    }
}

void    Response::generateBody(std::string path)
{

    std::string ext = get_extension(path);
    this->respMessage.Content_Type = getType(this->server.types, ext);

    std::ifstream fd(path.c_str(), std::ios::binary);
    if (fd.is_open() == false)
    {
        generateBodyError(403);
        throw (403);
        return ;
    }
    if (this->location.cgi_index.size() != 0 && (this->respMessage.Content_Type == "py" || this->respMessage.Content_Type == "php"))
    {
        this->r_env.push_back("SCRIPT_FILENAME=" + path);
        char **envp = new char*[this->r_env.size() + 1];
        int i = 0;
        for (std::vector<std::string>::iterator it = this->r_env.begin(); it != this->r_env.end(); it++)
        {
            envp[i] = new char[it->length() + 1];
            strcpy(envp[i], it->c_str());
            i++;
        }
        envp[i] = NULL;
        Cgi cgi_(envp, this->body);
        cgi_.runCgi();
        this->respMessage.body = cgi_.get_response();
        this->respMessage.Content_Type = "text/html";
        this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());
        this->respMessage.statusCode = generateStatusCode(200);
        while (envp[i] != NULL)
        {
            delete envp[i];
            i++;
        }
        delete[] envp;
    }
    else if (this->req.method == "POST" && this->req.headers.find("Content-Type") != this->req.headers.end())
    {
        std::cout << "i'm here" << std::endl;
        if (this->location.client_body_size.size() != 0)
        {
            if ((int)(this->req.headers["Content-Length"].size()) > atoi(this->location.client_body_size.c_str()))
            {
                generateBodyError(413);
                throw (413);
            }
            uploadFile();
        }
        else
        {
            generateBodyError(404);
            throw (404);
        }
    }
    else
    {
        if (this->respMessage.Content_Type == "py" || this->respMessage.Content_Type == "php")
            this->respMessage.Content_Type = "application/octet-stream";
        std::string line(std::istreambuf_iterator<char>(fd), (std::istreambuf_iterator<char>()));
        this->respMessage.body = line;
        this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());
        this->respMessage.statusCode = generateStatusCode(200);
    }
    fd.close();
    return ;
}

std::string extractLocation(std::string url)
{

    std::string::iterator it = url.end() - 1;
    std::string original_url;

    while (it != url.begin())
    {
        if (*it == '/')
            break;
        it--;
    }
    original_url = url.substr(0, it - url.begin() + 1);
    if (original_url != "/")
        original_url.erase(original_url.end() - 1);
    return (original_url);
}

void	Response::urlRegenerate() // ->status_code & ->Location 
{
	std::string path;
    std::string url;

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
            getLocation(extractLocation(url));
            generateBody(path);
        }
    }   
    else
    {
        std::cout << "Error : " << strerror(errno) << std::endl;
        generateBodyError(404);
        throw (404);
    }
}

t_server    Response:: fillServer(request req)
{
    std::string	port;
    std::string	server;
    std::string	host;
	size_t		found;
	std::vector<t_server>::iterator it;
	std::vector<int>::iterator itt;
    t_server    tmp_server;

	host = req.headers["Host"];
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
                    tmp_server.port.clear();
                    tmp_server.port.push_back(*itt);
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

    this->location.path.clear();
    this->location.root.clear();
    this->location.alias.clear();
    this->location.index.clear();
    this->location.proxy_pass.clear();
    this->location.rewrite.clear();
    this->location.allow_methods.clear();
    this->location.denny.clear();
    this->location.try_files.clear();
    this->location.expires.clear();
    this->location.access_log.clear();
    this->location.error_page.clear();
    this->location.limite_rate.clear();
    this->location.limite_except.clear();
    this->location.client_body_size.clear();
    this->location.proxy_set_header.clear();
    this->location.redirect.clear();
    this->location.autoindex.clear();
    this->location.cgi_path.clear();
    this->location.cgi_ext.clear();
    this->location.cgi_index.clear();


    this->body.clear();


}

void    Response::parseBody(Message *mes)
{
    (void)mes;
    std::string part;
    std::string tmp;
    std::vector<std::string> parts;

    // this->req.boundary += "\r\n";
    size_t found = this->req.body.find((this->req.boundary));
    while (found != std::string::npos)
    {
        tmp = this->req.body.substr(0, found + this->req.boundary.length());
        // tmp.erase(tmp.end() - 2, tmp.end());
        parts.push_back(tmp);
        this->req.body.erase(0, found + this->req.boundary.length());
        found = this->req.body.find(this->req.boundary);
    }
    // for(std::vector<std::string>::iterator it = parts.begin(); it != parts.end(); it++)
    // {
        
    //     std::cout << "part:\n" << *it << std::endl;
    // }
}

void    Response::generateResponse(Message* mes)
{
    // check if there is a body and handle it [!] ..................... [!]
    std::string line;
    this->server =  mes->getServer();
    this->req = mes->getRequest();
    this->body = mes->getBody();
    this->r_env = mes->getEnv();
    // std::cout << this->body << std::endl;
    this->respMessage.http_version = "HTTP/1.1";
	try
	{
        if (mes->getStatus() != 0)
        {
            std::cout << "Error : " << mes->getStatus() << std::endl;
            throw (mes->getStatus());
        }
		checkMethode();
        urlRegenerate();
        mes->setStatus(1);
	}
	catch(int m)
	{
        mes->setStatus(-1);
		this->respMessage.statusCode = generateStatusCode(m);
        // std::cout << this->respMessage.statusCode << std::endl;
		// exit(1);
	}
    
    mes->setResponse(generateMessage());
    // std::cout << "Response : " << std::endl;
    // std::cout << mes->getResponse() << std::endl;
    clearResponse();
}

std::vector<std::string>   cgi_env(request req, t_server server, char **env_system)
{
    std::vector<std::string> env;

    env.push_back("CONTENT_LENGTH=" + req.headers["Content-Length"]); // OK ----------------
    env.push_back("CONTENT_TYPE=" + req.headers["Content-Type"]); // OK ----------------
    env.push_back("GATEWAY_INTERFACE=CGI/1.1"); // OK ----------------
    env.push_back("DOCUMENT_URI=" + req.path); // OK ----------------
    env.push_back("DOCUMENT_ROOT=" + server.root); // OK ----------------
    env.push_back("QUERY_STRING=" + req.headers["Query-String"]); // OK ----------------
    env.push_back("REMOTE_ADDR="); // OK
    env.push_back("REMOTE_HOST="); // OK ----------------
    env.push_back("REMOTE_IDENT="); // OK ----------------
    env.push_back("REMOTE_USER="); // OK ---------------- 
    env.push_back("REQUEST_METHOD=" + req.method); // OK ----------------
    env.push_back("REQUEST_URI=" + req.path); // OK ----------------
    env.push_back("SCRIPT_NAME=" + req.path); // OK ----------------
    env.push_back("SERVER_NAME=" + req.headers["Host"]); // OK ----------------
    env.push_back("SERVER_PORT=" + std::to_string(server.port[0])); // OK -----
    env.push_back("SERVER_PROTOCOL=" + req.httpVertion); // OK ----------------
    env.push_back("SERVER_SOFTWARE=" + std::string("webserv/1.0")); // OK ----------------
    env.push_back("REDIRECT_STATUS=" + std::to_string(200)); // -------------------- MOST BE CHANGED ---------------- [!]
    if (env_system != NULL && env_system[0] != NULL)
    {
        std::string tmp;
        for (int i = 0; env_system[i] != NULL; i++)
        {
            tmp = env_system[i];
            if (tmp.find("PATH=") != std::string::npos)
                env.push_back("PATH=" + std::string(env_system[i] + 5));
            else if (tmp.find("LANG=") != std::string::npos)
                env.push_back("LANG=" + std::string(env_system[i] + 5));
            else if (tmp.find("HOME=") != std::string::npos)
                env.push_back("HOME=" + std::string(env_system[i] + 5));
            else if (tmp.find("LOGNAME=") != std::string::npos)
                env.push_back("LOGNAME=" + std::string(env_system[i] + 8));
            else if (tmp.find("USER=") != std::string::npos)
                env.push_back("USER=" + std::string(env_system[i] + 5));
        }
    }
    return (env);
}

void    Response::checkUrl()
{
    ;
}

t_location  fillLocation(t_server &serv, request& req)
{
    t_location loc;
    std::string path = extractLocation(req.path);
    std::vector<t_location>::iterator it = serv.locations.begin();
    while (it != serv.locations.end())
    {
        if (path == it->path)
        {
            loc = *it;
            break ;
        }
        it++;
    }
    return loc;
}

size_t  getSize(std::string size)
{
    size_t  s;
    if (size.size() != 0)
    {
        s = atoi(size.c_str());
        if (s != 0 && s <= (CLIENT_MAX_BODY_SIZE))
            return (s);
    }
    return(CLIENT_MAX_BODY_SIZE);
}

int    checkUrlSyntax(request &req)
{
    std::string path = req.path;
    std::string transfer_encoding = req.headers["Transfer-Encoding"];
    if (transfer_encoding.size() != 0 && transfer_encoding != "chunked")
        return 501;
    if (req.method == "POST" && req.headers["Content-Length"].length() == 0 && transfer_encoding.length() == 0)
        return 400;
    if (req.path.length() > 2048)
        return 414;
    // int i = 0;
    
    // while (i < (int)(path.length() - 2))
    // {
    //     if (path[i] == '%' && !(path[i + 1] >= '1' && path[i + 1] <= '9') && !(path[i] >= 'a' && path[i] <= 'f'))
    //         return 400;
    //     i++;
    // }
    return 0;
}

Message* Response::checkHeader(std::string request_)
{
    Message *mes = new Message();
    mes->setStatus(0);
    request tmp_request = parseRequest(request_);
    int url = checkUrlSyntax(tmp_request);
    if (url != 0)
    {
        mes->setStatus(url);
        return mes;
    }
    t_server    tmp_server = fillServer(tmp_request);
    t_location tmp_location = fillLocation(tmp_server, tmp_request);
    size_t     tmp_size;
    if (tmp_location.path.size() != 0)
    {
        mes->setRequest(tmp_request);
        mes->setServer(tmp_server);
        mes->setLocation(tmp_location);

        size_t content_length = 0;
        unsigned long  index1 = request_.find("Content-Length: ");
        if (index1 != std::string::npos)
        {
            unsigned long  index2 = request_.find("\r", index1);
            if ( index2 != std::string::npos)
            {
                std::istringstream ss( request_.substr(index1 + 16 , index2 - index1 - 16));
                if (ss >> content_length)
                {
                    tmp_size = getSize(tmp_location.client_body_size);
                    if (content_length <= tmp_size)
                        mes->setContentLength(content_length);
                    else
                    {
                        mes->setStatus(413);
                        mes->setContentLength(0);
                    }
                }
                else
                {
                    mes->setStatus(500);
                }
            }
        }
        else
        {
            content_length = 0; // must be deleted
            mes->setContentLength(0);
        }
        mes->setEnv(cgi_env(mes->getRequest(), mes->getServer(), this->env));
    }
    else
    {
        mes->setStatus(404);
    }

    return (mes);
}

Response::~Response(){};

    
