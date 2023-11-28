#include "Parse.hpp"

void    printServers(std::vector<t_server> servers)
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        std::cout << "-------------------------SERVER-------------------------" << std::endl;
        !(servers[i].name.empty()) && std::cout << "Name : " << servers[i].name << std::endl;
        !(servers[i].port.empty()) && std::cout << "Port: " << servers[i].port << std::endl;
        !(servers[i].host.empty()) && std::cout << "Host: " << servers[i].host << std::endl;
        !(servers[i].root.empty()) && std::cout << "Root: " << servers[i].root << std::endl;
        !(servers[i].index.empty()) && std::cout << "Index: " << servers[i].index << std::endl;
        !(servers[i].error_page.first.empty()) && std::cout << "Error Page: " << servers[i].error_page.first << " " << servers[i].error_page.second << std::endl;
        if (servers[i].allow_methods.size() != 0)
        {
            std::cout << "Allow Methods: ";
            for (unsigned long j = 0; j < servers[i].allow_methods.size(); j++)
                std::cout << servers[i].allow_methods[j] << "  |  ";
            std::cout << std::endl;
        }
        for (unsigned long j = 0; j < servers[i].locations.size(); j++)
        {
            std::cout << " => Locations: " << std::endl;
            std::cout << "    Path: " << servers[i].locations[j].path << std::endl;
            !(servers[i].locations[j].root.empty()) && std::cout << "    Root: " << servers[i].locations[j].root << std::endl;
            !(servers[i].locations[j].alias.empty()) && std::cout << "    Alias: " << servers[i].locations[j].alias << std::endl;
            !(servers[i].locations[j].index.empty()) && std::cout << "    Index: " << servers[i].locations[j].index << std::endl;
            !(servers[i].locations[j].proxy_pass.empty()) && std::cout << "    Proxy Pass: " << servers[i].locations[j].proxy_pass << std::endl;
            !(servers[i].locations[j].rewrite.empty()) && std::cout << "    Rewrite: " << servers[i].locations[j].rewrite << std::endl;
            if (servers[i].locations[j].allow_methods.size() != 0)
            {
                std::cout << "    Allow Methods: " ;
                for (unsigned long k = 0; k < servers[i].locations[j].allow_methods.size(); k++)
                    std::cout << servers[i].locations[j].allow_methods[k] << " |  ";
                std::cout << std::endl;
            }
            !(servers[i].locations[j].denny.empty()) && std::cout << "    Denny: " << servers[i].locations[j].denny << std::endl;
            !(servers[i].locations[j].try_files.empty()) && std::cout << "    Try Files: " << servers[i].locations[j].try_files << std::endl;
            !(servers[i].locations[j].expires.empty()) && std::cout << "    Expires: " << servers[i].locations[j].expires << std::endl;
            !(servers[i].locations[j].access_log.empty()) && std::cout << "    Access Log: " << servers[i].locations[j].access_log << std::endl;
            !(servers[i].locations[j].error_page.empty()) && std::cout << "    Error Page: " << servers[i].locations[j].error_page << std::endl;
            !(servers[i].locations[j].limite_rate.empty()) && std::cout << "    Limite Rate: " << servers[i].locations[j].limite_rate << std::endl;
            !(servers[i].locations[j].limite_except.empty()) && std::cout << "    Limite Except: " << servers[i].locations[j].limite_except << std::endl;
            !(servers[i].locations[j].client_body_buffer_size.empty()) && std::cout << "    Client Body Buffer Size: " << servers[i].locations[j].client_body_buffer_size << std::endl;
            !(servers[i].locations[j].proxy_set_header.empty()) && std::cout << "    Proxy Set Header: " << servers[i].locations[j].proxy_set_header << std::endl;
            !(servers[i].locations[j].redirect .empty()) && std::cout << "    Redirect: " << servers[i].locations[j].redirect << std::endl;
            !(servers[i].locations[j].autoindex.empty()) && std::cout << "    Autoindex: " << servers[i].locations[j].autoindex << std::endl;
            if (servers[i].locations[j].cgi_path.size() != 0)
            {
                std::cout << "    Cgi Path: " << std::endl;
                for (unsigned long k = 0; k < servers[i].locations[j].cgi_path.size(); k++)
                    std::cout << servers[i].locations[j].cgi_path[k] << std::endl;
            }
            if (servers[i].locations[j].cgi_ext.size() != 0)
            {
                std::cout << "    Cgi Ext: " << std::endl;
                for (unsigned long k = 0; k < servers[i].locations[j].cgi_ext.size(); k++)
                    std::cout << servers[i].locations[j].cgi_ext[k] << std::endl;
            }
        }
    }
}