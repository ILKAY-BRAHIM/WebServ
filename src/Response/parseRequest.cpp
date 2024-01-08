#include "response.hpp"

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
    return (req);
}

void encodingPath(std::string &str)
{
    std::string temp;
    for(size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '%')
        {
            if (i + 2 >= str.length())
                break;
            else
            {
                temp = str.substr(i, 3);
                if (temp == "%20")
                    str.replace(i, 3, " ");
            }
        }
    }
}

int    checkUrlSyntax(request &req)
{
    std::string path = req.path;
    std::string tmp = req.headers["Query-String"];
    int tmp_size = 0;

    if (tmp.size() != 0)
        tmp_size = tmp.size() + 1;

    std::string transfer_encoding = req.headers["Transfer-Encoding"];
    if (transfer_encoding.size() != 0 && transfer_encoding != "chunked")
        return 501;
    std::string content_length = req.headers["Content-Length"];
    if (req.method == "POST" && (content_length == "0") && (req.headers["Transfer-Encoding"].size() == 0))
        return 400;
    if ((req.path.length() + tmp_size) > 2048)
        return 414;
    encodingPath(req.path);
    return 0;
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
		if (it->host == server || (it->name == server && it->name == "localhost"))
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
    if (tmp_server.port.size() == 0)
        tmp_server = this->servS[0];
    return (tmp_server);
}

std::vector<std::string>   cgi_env(request req, t_server server, char **env_system)
{
    std::vector<std::string> env;

    env.push_back("CONTENT_LENGTH=" + req.headers["Content-Length"]);
    env.push_back("CONTENT_TYPE=" + req.headers["Content-Type"]);
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("DOCUMENT_URI=" + req.path);
    env.push_back("DOCUMENT_ROOT=" + server.root);
    env.push_back("QUERY_STRING=" + req.headers["Query-String"]);
    env.push_back("REMOTE_ADDR="); 
    env.push_back("REMOTE_HOST=");
    env.push_back("REMOTE_IDENT=");
    env.push_back("REMOTE_USER="); 
    env.push_back("REQUEST_METHOD=" + req.method);
    env.push_back("REQUEST_URI=" + req.path);
    env.push_back("SCRIPT_NAME=" + req.path);
    env.push_back("SERVER_NAME=" + req.headers["Host"]);
    env.push_back("SERVER_PORT=" + std::to_string(server.port[0])); 
    env.push_back("SERVER_PROTOCOL=" + req.httpVertion);
    env.push_back("SERVER_SOFTWARE=" + std::string("webserv/1.0"));
    env.push_back("REDIRECT_STATUS=" + std::to_string(200));
    env.push_back("HTTP_COOKIE=" + req.headers["Cookie"]);
    if (env_system != NULL && env_system[0] != NULL)
    {
        std::string tmp;
        for (int i = 0; env_system[i] != NULL; i++)
        {
            tmp = env_system[i];
            if (tmp.find("PATH=") != std::string::npos)
                env.push_back(std::string(env_system[i]));
            else if (tmp.find("LANG=") != std::string::npos)
                env.push_back(std::string(env_system[i]));
            else if (tmp.find("HOME=") != std::string::npos)
                env.push_back(std::string(env_system[i]));
            else if (tmp.find("LOGNAME=") != std::string::npos)
                env.push_back(std::string(env_system[i]));
            else if (tmp.find("USER=") != std::string::npos)
                env.push_back(std::string(env_system[i]));
        }
    }
    return (env);
}

Message* Response::checkHeader(std::string request_)
{
    Message *mes = new Message();
    request tmp_request = parseRequest(request_);
    int url = checkUrlSyntax(tmp_request);

    mes->setStatus(0);
    mes->setContentLength(0);
    if (url != 0)
    {
        mes->setStatus(url);
        return mes;
    }
    t_server    tmp_server = fillServer(tmp_request);
    mes->setRequest(tmp_request);
    mes->setServer(tmp_server);
    size_t content_length = 0;

    std::string transfer = tmp_request.headers["Transfer-Encoding"];
    if (transfer.size() != 0)
    {
        mes->setTransfer_Encoding(true);
        if (transfer != "chunked")
            mes->setStatus(501);
    }
    else
        mes->setTransfer_Encoding(false);
    std::string tmp_content = tmp_request.headers["Content-Length"];
    if (transfer.size() == 0 && tmp_content.size() != 0)
    {
        std::istringstream ss(tmp_content);
        if (ss >> content_length)
        {
            if (content_length > (CLIENT_MAX_BODY_SIZE))
                mes->setStatus(413);
            mes->setContentLength(content_length);
        }
        else
            mes->setStatus(500);
    }
    mes->setEnv(cgi_env(mes->getRequest(), mes->getServer(), this->env));
    return (mes);
}
