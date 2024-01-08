#include "response.hpp"

Response::Response(void){};

Response::Response(std::vector<t_server> servS, char **env)
{
    this->servS = servS;
    this->env = env;
}

bool    allowedMethod(std::vector<std::string> methods, std::string method)
{
    std::vector<std::string>::iterator it = methods.begin();

    while (it != methods.end())
    {
        if (*it == method)
            return (true);
        it++;
    }
    return (false);
}

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
            break;
        case 304:
            message =  std::to_string(status) + " Not Modified";
            break;
        case 307:
            message =  std::to_string(status) + " Temporary Redirect";
            break;
        case 308:
            message =  std::to_string(status) + " Permanent Redirect";
            break;
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
            message =  std::to_string(status) + " Content Too Large";
            break;
        case 414:
            message =  std::to_string(status) + " URI Too Long";
            break;
        case 415:
            message =  std::to_string(status) + " Unsupported Media Type";
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

void	Response::checkMethode()
{
    std::vector<std::string> tmp;
    if (!(this->req.method == "GET" || this->req.method == "POST" || this->req.method == "DELETE"))
        throw (501);
    if (this->req.httpVertion != "HTTP/1.1")
        throw (505);
	this->respMessage.Server = "Not nginx/0.0.0 (macOS)";
}

void    Response::redirect(std::string path, int status)
{
    this->respMessage.statusCode = generateStatusCode(status);
    this->respMessage.Location = path;
    this->respMessage.Content_Lenght = "0";
    return ;
}

int     Response::getLocation(std::string url)
{
    std::vector<t_location>::iterator it = this->server.locations.begin();
    if (url != "/")
    {
        std::string::iterator itt = url.end() - 1;
        if (*itt == '/')
            url.erase(itt);
    }
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
    this->respMessage.body += ("<head><title>" + generateStatusCode(error) + "</title>\n");
    this->respMessage.body += "<style>\n";
    this->respMessage.body += "  * { \n";
    this->respMessage.body += "       font-family: 'Courier New', Courier, monospace;\n";
    this->respMessage.body += "       font-size: 1.2rem;\n";
    this->respMessage.body += "       background-color: rgb(246, 246, 246);\n";
    this->respMessage.body += "   }\n";
    this->respMessage.body += "</style>\n";
    this->respMessage.body += "</head>\n";
    this->respMessage.body += "<body>\n";
    this->respMessage.body += ("<center><h1>" + generateStatusCode(error) + "</h1></center>\n");
    this->respMessage.body += ("<hr><center>" + this->respMessage.Server + "</center>\n");
    this->respMessage.body += "</body>\n";
    this->respMessage.body += "</html>";
    this->respMessage.Content_Lenght = std::to_string(this->respMessage.body.length());

}

int    Response::isDirectory(std::string path)
{
    std::string index_;
    std::string fullPath;
    if (this->location.path.size())
    {
        if (this->location.internal == true)
        {
            
            return (403);
        }
        if (this->location.redirect.size() != 0)
        {
            redirect(this->location.redirect, 301);
            return (1);
        }
        else if (this->location.cgi_index.size() != 0)
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
        else if (this->location.autoindex == true)
        {
            return (generateAutoindexBody());
        }
        else
        {
            if (this->server.index.size() != 0)
                index_ = get_index(this->server, path, 0);
            else 
                index_ = get_index(this->server, path, 1);
        }
        if (index_.length() != 0)
        {
            this->path = path + index_;
            return (0);
        }
        else
        {
            this->path = "";
            if (this->req.method == "POST" || this->req.method == "DELETE")
                return (0);
            return(403);
        }
    }
    else
    {
        if (this->server.index.size() != 0)
            index_ = get_index(this->server, path, 0);
        else if (this->server.autoindex == true)
            return (generateAutoindexBody());
        else        
            index_ = get_index(this->server, path, 1);
        if (index_.length() != 0)
        {
            this->path = path + index_;
            return (0);
        }
        else
        {
            if (this->req.method == "POST" || this->req.method == "DELETE")
                return (0);
            this->path = "";
            return(403);
        }
    }
    return (0);
}

std::string get_extension(std::string path)
{
    if (path.size() == 0)
        return ("");
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

std::string get_cgi_ext(std::vector<std::string> cgi_ext, std::string ext)
{
    std::vector<std::string>::iterator it = cgi_ext.begin();
    while (it != cgi_ext.end())
    {
        if (*it == ext)
            return (ext);
        it++;
    }
    return ("");
}

std::string defaultType(std::string ext)
{
    if (ext == "html")
        return ("text/html");
    else if (ext == "css")
        return ("text/css");
    else if (ext == "txt")
        return ("text/plain");
    else if (ext == "jpg" || ext == "jpeg")
        return ("image/jpeg");
    else if (ext == "png")
        return ("image/png");
    else if (ext == "gif")
        return ("image/gif");
    else if (ext == "ico")
        return ("image/x-icon");
    else if (ext == "mp4")
        return ("video/mp4");
    else if (ext == "mp3")
        return ("audio/mpeg");
    else if (ext == "js")
        return ("application/javascript");
    else if (ext == "pdf")
        return ("application/pdf");
    else if (ext == "zip")
        return ("application/zip");
    else if (ext == "tar")
        return ("application/x-tar");
    else if (ext == "gz")
        return ("application/x-gzip");
    else if (ext == "xml")
        return ("application/xml");
    else if (ext == "json")
        return ("application/json");
    else
        return ("application/octet-stream");
}

std::string getType(std::vector<t_types> types, std::string ext)
{
    if (ext.size() == 0)
        return ("application/octet-stream");
    std::vector<t_types>::iterator it = types.begin();
    std::vector<std::map<std::string, std::vector<std::string> > > media;
    std::string type ;
    if (types.size() == 0)
        return (defaultType(ext));
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

std::string generateName()
{
    std::string name;
    std::string tmp;
    int i = 0;
    while (i < 10)
    {
        tmp = std::to_string(rand() % 10);
        name += tmp;
        i++;
    }
    return (name);
}

int    Response::uploadFile()
{
    std::string tmpFile;
    std::string ext;
    int found;
    int fd;
    t_location loc;
    for (size_t i = 0; i < this->server.locations.size(); i++)
    {
        if (this->server.locations[i].path == "/properDataBase")
        {
            loc = this->server.locations[i];
            if (loc.root.size() != 0)
            {
                if (loc.root[loc.root.size() - 1] == '/')
                    tmpFile = loc.root + "properDataBase";
                else
                    tmpFile = loc.root + "/properDataBase";
            }
            else
            {
                if (this->server.root[this->server.root.size() - 1] == '/')
                    tmpFile = this->server.root + "properDataBase";
                else
                    tmpFile = this->server.root + "/properDataBase";
            }
            break;
        }
    }
    if (tmpFile.size() == 0 || access(tmpFile.c_str(), F_OK | W_OK) != 0)
        return (500);
    found = this->req.headers["Content-Type"].find("/");
    ext = this->req.headers["Content-Type"].substr(found + 1);
    if (checkMimeType(this->server.types, this->req.headers["Content-Type"])  || defaultType(ext) != "application/octet-stream")
    {
        tmpFile += '/' + generateName() + '.' + ext;
        fd = open(tmpFile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (fd == -1)
            throw (500);
        if (write(fd, this->body.c_str(), this->body.length()) == -1)
        {
            close(fd);
            return (500);
        }
        this->uploadedFile = tmpFile;
        close(fd);
    }
    else
        return (415);
    return 0;
}

void    Response::parseCGI_body(std::string body)
{
    std::string line;
    std::istringstream ss(body);
    std::string tmp_body;
    std::vector<std::string> tmp_header;
    std::vector<std::pair<std::string, std::string> > cgi_headers;

    while (1)
    {
        std::getline(ss, line, '\n');
        if (line.find('\r') != std::string::npos)
        {
            line.erase(line.find('\r'));
            if (line.size() != 0)
                tmp_header.push_back(line);
        }
        else
        {
            tmp_body = line;
            while (std::getline(ss, line, '\n'))
            {
                tmp_body += '\n';
                tmp_body += line;
            }
            break;
        }
    }
    for (std::vector<std::string>::iterator it = tmp_header.begin(); it != tmp_header.end(); it++)
        cgi_headers.push_back(std::make_pair(it->substr(0, it->find(':')), it->substr(it->find(':') + 1)));

    for (std::vector<std::pair<std::string, std::string> >::iterator it = cgi_headers.begin(); it != cgi_headers.end(); it++)
    {
        std::string tmp_key = it->first;
        for (size_t i = 0; i < tmp_key.size(); i++)
            tmp_key[i] = std::tolower(tmp_key[i]);
        if (tmp_key == "content-type")
            this->respMessage.Content_Type = it->second;
        else if (tmp_key == "cache-control")
            this->respMessage.Cache_Control = it->second;
        else if (tmp_key == "connection")
            this->respMessage.Connection = it->second;
        else if (tmp_key == "status")
            this->respMessage.statusCode = it->second;
        else
            this->header.push_back(it->first + ": " + it->second);
    }
    this->respMessage.body = tmp_body;
}

int    Response::generateBody(std::string path)
{
    bool cgi = false;
    int  cgi_return;
    if (this->location.path.size() != 0)
    {
        if (this->location.internal == true)
            return (403);
        if (this->location.redirect.size() != 0)
        {
            redirect(this->location.redirect, 301);
            return (1);
        }
    }
    std::string ext = get_extension(path);
    this->respMessage.Content_Type = getType(this->server.types, ext);
    if (this->respMessage.Content_Type == "application/octet-stream")
    {
        if (this->location.path.size() != 0 && this->location.cgi_ext.size() != 0)
        {
            this->respMessage.Content_Type = get_cgi_ext(this->location.cgi_ext, ext);
            if (this->respMessage.Content_Type == "")
                this->respMessage.Content_Type = "application/octet-stream";
            else
                cgi = true;
        }
    }
    std::ifstream fd(path.c_str(), std::ios::binary);
    if (fd.is_open() == false)
        return (403);
    if (this->location.path.size() != 0 && this->location.cgi_index.size() != 0 && cgi == true)
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
        cgi_return = cgi_.runCgi();
        if (cgi_return != -1)
        {
            this->respMessage.body = cgi_.get_response();
            parseCGI_body(this->respMessage.body);
            if (this->respMessage.Content_Type == "application/octet-stream" || this->respMessage.Content_Type.size() == 0)
                this->respMessage.Content_Type = "text/html";
            this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());
            if (this->respMessage.statusCode.size() == 0)
                this->respMessage.statusCode = generateStatusCode(200);
        }
        i = 0;
        while (envp[i] != NULL)
        {
            delete envp[i];
            i++;
        }
        delete[] envp;
        if (cgi_return == -1)
            return (500);
    }
    else
    {
        if (cgi == true)
            this->respMessage.Content_Type = "application/octet-stream";
        std::string line(std::istreambuf_iterator<char>(fd), (std::istreambuf_iterator<char>()));
        this->respMessage.body = line;
        this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());
        if (this->respMessage.statusCode.size() == 0)
            this->respMessage.statusCode = generateStatusCode(200);
    }
    fd.close();
    return (0);
}

std::string extractLocation(std::string url, std::string root)
{
    std::string path = url;
    if (access(path.c_str(), F_OK) == 0)
    {
        struct stat fileStat;
        if (!(stat(path.c_str(), &fileStat) == 0))
            return ("");
        if (S_ISDIR(fileStat.st_mode))
        {
            return (url.substr(root.length() , url.length() - 1));
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            std::string::iterator it = url.end() - 1;
            std::string original_url;

            while (it != url.begin())
            {
                if (*it == '/')
                {
                    break;
                }
                it--;
            }
            original_url = url.substr(0, it - url.begin() + 1);
            if (original_url != "/")
                original_url.erase(original_url.end() - 1);
            if (root == original_url)
                return ("/");
            return (original_url.substr(root.length() , original_url.length() - 1));
        }
    }
    return ("");
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
    if (this->respMessage.Connection.size() != 0)
    {
        mess += std::string("Connection: ");
        mess += this->respMessage.Connection;
        mess += std::string(CRLF);
    }
    if (this->respMessage.Cache_Control.size() != 0)
    {
        mess += std::string("Cache-Control: ");
        mess += this->respMessage.Cache_Control;
        mess += std::string(CRLF);
    }
    if (this->respMessage.Set_Cookie.size() != 0)
    {
        mess += std::string("Set-Cookie: ");
        mess += this->respMessage.Set_Cookie;
        mess += std::string(CRLF);
    }
    for (std::vector<std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
    {
        mess += *it;
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

    this->header.clear();
    this->r_env.clear();

    this->location.clear();

    this->body.clear();
    this->server.clear();
    this->path.clear();
    this->uploadedFile.clear();
    this->content_length = 0;
}

t_location  fillLocation(t_server &serv, request& req)
{
    t_location loc;
    std::string path = extractLocation(req.path, serv.root);
    std::vector<t_location>::iterator it = serv.locations.begin();
    if (path.size() != 0)
    {
        while (it != serv.locations.end())
        {
            if (path == it->path)
            {
                loc = *it;
                break ;
            }
            it++;
        }
    }
    return loc;
}

size_t  getSize(std::string size)
{
    size_t  s;
    if (size.size() != 0)
    {
        s = atoi(size.c_str());
        s *= MEGABYTE;
        if (s != 0 && s <= (CLIENT_MAX_BODY_SIZE))
            return (s);
    }
    return(CLIENT_MAX_BODY_SIZE);
}

std::string Response::getRoot()
{
    std::string tmp_root;

    tmp_root = this->server.root + this->req.path;
    if (access(tmp_root.c_str(), F_OK) == 0)
        return (this->server.root);
    for(std::vector<t_location>::iterator it = this->server.locations.begin(); it != this->server.locations.end(); it++)
    {
        if (it->path.size() != 0)
        {
            tmp_root = it->root + this->req.path;
            if (access(tmp_root.c_str(), F_OK) == 0)
                return (it->root);
        }
    }
    return "";
}

int Response::specificErrorPage(int error_code)
{
    std::string tmp_root;
    std::vector<std::string> error_page;
    std::string page = std::to_string(error_code);
    std::string error_path;
    std::string tmp;
    bool found = false;

    if (this->server.error_page.first.size() != 0)
    {
        error_page = this->server.error_page.first;
        for (size_t i = 0; i < error_page.size(); i++)
        {
            if (error_page[i] == page)
            {
                if (*this->server.error_page.second.begin() == '/')
                    tmp = this->server.error_page.second.substr(1);
                else
                    tmp = this->server.error_page.second;
                page = page + tmp;
                found = true;
                break;
            }
        }
    }
    else
        return (1);
    if (found == false)
        return (1);
    for(std::vector<t_location>::iterator it = this->server.locations.begin(); it != this->server.locations.end(); it++)
    {
        if (it->path.size() != 0 && it->path == "/error")
        {
            if (it->root.size() != 0)
                tmp_root = it->root;
            else
                tmp_root = this->server.root;
        }
    }
    if (tmp_root.size() == 0)
        return (1);
    if (access((tmp_root + "/error/").c_str(), F_OK | R_OK) == 0)
    {
        if (access((tmp_root + "/error/" + page).c_str(), F_OK | R_OK) == 0)
            error_path = tmp_root + "/error/" + page;
    }
    if (error_path.size() == 0)
        return (1);
    else
    {
        if (get_extension(error_path) != "html")
            return 1;
        std::ifstream fd(error_path.c_str(), std::ios::binary);
        if (fd.is_open() == false)
            return (1);
        std::string line(std::istreambuf_iterator<char>(fd), (std::istreambuf_iterator<char>()));
        this->respMessage.body = line;
        this->respMessage.Content_Lenght = std::to_string((this->respMessage.body).length());
        fd.close();
        return 0;
    }
    return 1;
}

int	Response::urlRegenerate()
{
	std::string path;
    std::string url;
	url = req.path;
    std::string root = getRoot();

    if (root.size() != 0)
    {
        path = root + url;
        struct stat fileStat;
        if (!(stat(path.c_str(), &fileStat) == 0))
        {
            this->respMessage.Content_Lenght = "0";
            return (500);
            throw (500);
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            if (*(path.end() - 1) != '/')
            {
                if (this->req.method == "DELETE")
                    return (403);
                redirect((url + "/"), 308);
                return (1);
            }
            else
            {
                if (getLocation(url) != 1)
                    this->location.path = "";
                if (this->req.method == "DELETE")
                    return (403);
                return (isDirectory(path));
            }
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            std::string tmp_loc = extractLocation(path, root);
            if (tmp_loc.size() != 0)
                getLocation(tmp_loc);
            else
                this->location.path = "";
            this->path = path;
            return(0);
        }
    }   
    else
        return (404);
    return 0;
}

int   Response::postMethod()
{
    std::string tmp = this->req.headers["Content-Type"];
    int status;
    if (tmp.size() != 0 && this->location.cgi_index.size() == 0)
    {
        if (this->location.client_body_size.size() != 0)
        {
            size_t size = getSize(this->location.client_body_size);
            if (this->content_length > size)
                return (413);
        }
        else 
        {
            return (403);
        }
        status = uploadFile();
        if (status != 0)
            return (status);
        if (this->path.size() == 0 && this->location.redirect.size() == 0)
            return (generateUploadDeleteBody("Upload"));
    }
    return (generateBody(this->path));
}

int   Response::deleteMethod()
{
    if (this->req.headers["Content-Type"].size() != 0)
        return (501);
    if (remove(this->path.c_str()) != 0)
        return (403);
    if (this->location.redirect.size() == 0)
        return (generateUploadDeleteBody("Delete"));
    return (generateBody(this->path));
}

int hex_to_decimal(std::string hexString)
{
    int decimalValue;

    std::stringstream ss;
    ss << std::hex << hexString;
    ss >> decimalValue;
    return (decimalValue);
}

void    Response::unchunkeBody()
{
    std::string part;
    std::string size_hex;
    size_t pos;
    std::string body = this->body;
    std::string result;
    int size;

    while (1)
    {
        pos = body.find("\r\n");
        if (pos == std::string::npos)
            break;
        part = body.substr(0, pos);
        size =  hex_to_decimal(part);
        if (size == 0)
            break;
        result += body.substr(pos + 2, size);
        body.erase(0, 2 + size + 2 + part.size());
        size = 0;
    }
    this->body.clear();
    this->body = result;
}

void    Response::generateResponse(Message* mes)
{
    std::string line;
    int st;
    int state;
    this->respMessage.http_version = "HTTP/1.1";
	try
	{
        if (mes->getStatus() != 0)
        {
            this->respMessage.Server = "Not nginx/0.0.0 (macOS)";
            throw (mes->getStatus());
        }
        this->server =  mes->getServer();
        this->req = mes->getRequest();
        this->body = mes->getBody();
        if (mes->getTransfer_Encoding())
            unchunkeBody();
        this->r_env = mes->getEnv();
        this->content_length = mes->getContentLength();
		checkMethode();
        state = urlRegenerate(); // the location is filled here
        if (state != 0 && state != 1)
            throw (state);
        if (state == 0)
        {
            std::vector<std::string> tmp;
            if (this->location.path.size() && this->location.allow_methods.size())
                tmp = this->location.allow_methods;
            if (tmp.size() != 0)
            {
                if (allowedMethod(tmp, this->req.method) == false)
                    throw (405);
            }
            if (this->req.method == "GET")
                st = generateBody(this->path);
            else if (this->req.method == "POST")
                st = postMethod();
            else
                st = deleteMethod();
            if (st != 0 && st != 1)
                throw (st);
        }
        mes->setStatus(1);
	}
	catch(int m)
	{
        mes->setStatus(-1);
        if (specificErrorPage(m) == 1)
            generateBodyError(m);
		this->respMessage.statusCode = generateStatusCode(m);
	}
    if (this->respMessage.Content_Lenght.size() == 0)
        this->respMessage.Content_Lenght = "0";
    mes->setResponse(generateMessage());
    clearResponse();
}

void    Response::removeSession_Database()
{
    clearDirectory("./session/");
    clearDirectory("./properDataBase/");
}

Response::~Response(){};
