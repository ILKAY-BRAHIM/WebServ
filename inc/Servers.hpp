#pragma once
#include <string>
#include <iostream>



class Servers
{
    private:
        std::string request;
        std::string responce;
        // int fd_body;
        // Responce responce;

    public:
        // Servers();
        Servers(std::string request);
        std::string get_request() const {return this->request;}
        void collect_req(std::string &request);
        // Servers(const Responce &responce);
        // Servers(const Servers &other);
        // is_ready();
        // check_crlf();


};