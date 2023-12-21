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
        int redirection;
        std::string body;
        int partis;
        std::clock_t start;
        // std::clock_t end;
        unsigned long  total;
        unsigned long  total_body;
        Message *responce;
        std::vector<int> m9bara;
        int bypass;
        unsigned long pass_total;
        unsigned long content_length;

    public:
        // Servers();
        Servers(std::string request);
        void collect_req(char* request, int size);
        void collect_body(char* body, int size) {this->body.append(body, size);}

        std::string get_request() const {return this->request;}
        std::string get_responce() const {return this->_responce;}
        std::string get_body() const {return this->body;}
        Message *get_responce_class() const {return this->responce;}
        unsigned long  get_total() const {return this->total;}
        unsigned long  get_total_body() const {return this->total_body;}
        int get_partis() const {return this->partis;}
        std::clock_t get_start() const {return this->start;}
        std::vector<int> get_ma9bara() const {return this->m9bara;}
        int get_redirection() const {return this->redirection;}
        int get_bypass() const {return this->bypass;}
        unsigned long get_pass_total() const {return this->pass_total;}
        unsigned long get_content_length() const {return this->content_length;}

        void set_responce_class(Message *responce){this->responce = responce;}
        void set_total(int total) {this->total += total;}
        void set_total_body(int total_body) {this->total_body += total_body;}
        void rset_total(int total) {this->total = total;}
        void rset_total_body(int total_body) {this->total_body = total_body;}
        void rset_total_pass(int total_pass) {this->pass_total = total_pass;}
        void set_partis(int partis) {this->partis = partis;}
        void set_responce(std::string responce) {this->_responce = responce;}
        void set_start(std::clock_t start) {this->start = start;}
        void set_ma9bara(int fd) {this->m9bara.push_back(fd);}
        void set_request(std::string request) {this->request = request;}
        void set_body(std::string body) {this->body = body;}
        void set_redirection(int redirection) {this->redirection = redirection;}
        void set_bypass(int bypass) {this->bypass = bypass;}
        void set_pass_total(int total_pass) {this->pass_total += total_pass;}
        void set_content_length(unsigned long len) {this->content_length = len;}
        // Servers(const Responce &responce);
        // Servers(const Servers &other);
        // is_ready();
        // check_crlf();


};
