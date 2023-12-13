#pragma once
#include <string>
#include <iostream>
#include "response.hpp"



class Servers
{
    private:
        std::string request;
        std::string _responce;
        std::string host;
        // int fd_body;
        int partis;
        std::clock_t start;
        // std::clock_t end;
        unsigned long  total;
        Message *responce;
        std::vector<int> m9bara; 

    public:
        // Servers();
        Servers(std::string request);
        void collect_req(char* request);

        std::string get_request() const {return this->request;}
        std::string get_responce() const {return this->_responce;}
        Message *get_responce_class() const {return this->responce;}
        unsigned long  get_total() const {return this->total;}
        int get_partis() const {return this->partis;}
        std::clock_t get_start() const {return this->start;}
        std::vector<int> get_ma9bara() const {return this->m9bara;}

        void set_responce_class(Message *responce){this->responce = responce;}
        void set_total(int total) {this->total += total;}
        void rset_total(int total) {this->total = total;}
        void set_partis(int partis) {this->partis = partis;}
        void set_responce(std::string responce) {this->_responce = responce;}
        void set_start(std::clock_t start) {this->start = start;}
        void set_ma9bara(int fd) {this->m9bara.push_back(fd);}
        void set_request(std::string request) {this->request = request;}
        // Servers(const Responce &responce);
        // Servers(const Servers &other);
        // is_ready();
        // check_crlf();


};
