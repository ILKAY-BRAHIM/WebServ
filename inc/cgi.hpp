#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <signal.h>


class Cgi {
    private:
        char **env;
        std::string body;
        std::string response;
        int valid;
    public:
        Cgi(char **env, std::string body) : env(env), body(body), response(""), valid(1){}
        ~Cgi() {}
        char **getEnv() { return this->env;}
        char *get_path() {for (int i = 0; this->env[i]; i++) {if (strncmp(this->env[i], "SCRIPT_FILENAME=", 16) == 0) {return this->env[i] + 16;}} return NULL;}
        std::string get_response() { return this->response;}
        int   runCgi();
        int    getValid() { return this->valid;}
};
