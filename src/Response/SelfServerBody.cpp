#include "response.hpp"

int   Response::generateUploadDeleteBody(std::string method)
{
    this->respMessage.Content_Type = "text/html";
    this->respMessage.body += "<!DOCTYPE html>\n<html lang=\"en\">\n";
    this->respMessage.body += "<html>\n";
    this->respMessage.body += ("<head><title> "+ method +" is succesful </title>\n");
    this->respMessage.body += "<style>\n";
    this->respMessage.body += "  * { \n";
    this->respMessage.body += "       font-family: 'Courier New', Courier, monospace;\n";
    this->respMessage.body += "       font-size: 1.2rem;\n";
    this->respMessage.body += "       background-color: rgb(246, 246, 246);\n";
    this->respMessage.body += "   }\n";
    this->respMessage.body += "   center {\n";
    this->respMessage.body += "       margin: 1.2rem;\n";
    this->respMessage.body += "   }\n";
    this->respMessage.body += "</style>\n";
    this->respMessage.body += "</head>\n";
    this->respMessage.body += "<body >\n";
    this->respMessage.body += ("<center><h1> "+ method +" is succesful </h1></center>\n");
    if (method == "Upload")
        this->respMessage.body += "<center><a href=\"" + this->uploadedFile + "\" download=\""+ this->uploadedFile + "\"> " +" FILE</a></center>";
    if (this->server.index.size() != 0)
        this->respMessage.body += ("<center><a href=\"./" + this->server.index[0] + "\">" + " back to home" + "</a></center>\n");
    this->respMessage.body += "</body>\n";
    this->respMessage.body += "</html>";
    this->respMessage.Content_Lenght = std::to_string(this->respMessage.body.length());
    this->respMessage.statusCode = generateStatusCode(200);
    return (0);
}

int    Response::generateAutoindexBody()
{
    std::string path;
    bool size, time, type;
    if (this->location.path.size() != 0)
    {
        if (this->location.root.size() == 0)
            path = this->server.root + this->location.path;
        else
            path = this->location.root + this->location.path;
        size = this->location.autoindex_exact_size;
        time = this->location.autoindex_localtime;
        type = this->location.autoindex_format;
    }
    else
    {
        path = this->server.root + '/';
        size = this->server.autoindex_exact_size;
        time = this->server.autoindex_localtime;
        type = this->server.autoindex_format;
    }
    try 
    {
        t_Dir_Data data = readDirectory(path);
        if (data.directory.size() == 0 && data.file.size() == 0)
            throw (500);
        this->respMessage.Content_Type = "text/html";
        this->respMessage.body += "<!DOCTYPE html>\n<html lang=\"en\">\n";
        this->respMessage.body += "<html>\n";
        this->respMessage.body += ("<head>\n<title>Index of " + this->req.path + "</title>\n");
        this->respMessage.body += "<style>\n";
        this->respMessage.body += "  * { \n";
        this->respMessage.body += "       font-family: 'Courier New', Courier, monospace;\n";
        this->respMessage.body += "       font-size: 0.9rem;\n";
        this->respMessage.body += "       background-color: rgb(246, 246, 246);\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "   .container {\n";
        this->respMessage.body += "       display: flex;\n";
        this->respMessage.body += "       flex-wrap: wrap;\n";
        this->respMessage.body += "       justify-content: center;\n";
        this->respMessage.body += "       gap: 0.3rem;\n";
        this->respMessage.body += "       width: 90%;\n";
        this->respMessage.body += "       margin: 2rem auto;\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "   .align-horizontal {\n";
        this->respMessage.body += "       display: flex;\n";
        this->respMessage.body += "       justify-content: space-between;\n";
        this->respMessage.body += "       width: 100%;\n";
        this->respMessage.body += "       background-color: rgb(235, 235, 235);\n";
        this->respMessage.body += "       padding: 0.5rem;\n";
        this->respMessage.body += "       border-radius: 0.3rem;\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "   .align-horizontal p{\n";
        this->respMessage.body += "       margin: 0.2rem;\n";
        this->respMessage.body += "       background-color: rgb(235, 235, 235);\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "  a{\n";
        this->respMessage.body += "       text-decoration: none;\n";
        this->respMessage.body += "       width: 20rem;\n";
        this->respMessage.body += "       background-color: rgb(235, 235, 235);\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "   .sp{\n";
        this->respMessage.body += "       width: 20rem;\n";
        this->respMessage.body += "       word-wrap: break-word;\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "   .sd{\n";
        this->respMessage.body += "       width: 10rem;\n";
        this->respMessage.body += "       text-align: right;\n";
        this->respMessage.body += "       word-wrap: break-word;\n";
        this->respMessage.body += "   }\n";
        this->respMessage.body += "</style>\n";
        this->respMessage.body += "</head>\n";        
        this->respMessage.body += "<body>\n";
        this->respMessage.body += ("<center><h1>Index of " + this->req.path + "</h1></center>\n");
        this->respMessage.body += "<hr>\n";
        this->respMessage.body += "<div class=\"container\">\n";
        std::multimap<std::string, t_info>::iterator it = data.directory.begin();
        while (it != data.directory.end())
        {
            this->respMessage.body += "<div class=\"align-horizontal\">";
            this->respMessage.body += ("<p class=\"sp\"><a href=\"" + it->second.name + "\">" + it->second.name + "</a></p>\n");
            if(time)
                this->respMessage.body += "<p>"  + it->second.date +  "</p>\n";
            if(size)
                this->respMessage.body += "<p>"  + it->second.size +  "</p>\n";
            if (type)
                this->respMessage.body += "<p class=\"sd\">"  + it->second.type +  "</p>\n";
            this->respMessage.body += "</div>";
            it++;
        }
        it = data.file.begin();
        while (it != data.file.end())
        {
            this->respMessage.body += "<div class=\"align-horizontal\">";
            this->respMessage.body += ("<p class=\"sp\"><a href=\"" + it->second.name + "\">" + it->second.name + "</a></p>\n");
            if (time)
                this->respMessage.body += "<p>"  + it->second.date +  "</p>\n";
            if (size)
                this->respMessage.body += "<p>"  + it->second.size +  "</p>\n";
            if (type)
                this->respMessage.body += "<p class=\"sd\">"  + it->second.type +  "</p>\n";
            this->respMessage.body += "</div>";
            it++;
        }
        this->respMessage.body += "</div>\n";
        this->respMessage.body += ("<center>" + this->respMessage.Server + "</center>\n");
        this->respMessage.body += "</body>\n";
        this->respMessage.body += "</html>";
        this->respMessage.Content_Lenght = std::to_string(this->respMessage.body.length());
    }
    catch(int error)
    {
        return (error);
    }
    return (1);
}
