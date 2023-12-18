#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>


class Cgi {
    private:
        char **env;
        std::string body;
        std::string response;
    public:
        Cgi(char **env, std::string body) : env(env), body(body), response("") {}
        ~Cgi() {}
        char **getEnv() { return this->env;}
        char *get_path() {for (int i = 0; this->env[i]; i++) {if (strncmp(this->env[i], "SCRIPT_FILENAME=", 16) == 0) {return this->env[i] + 16;}} return NULL;}
        std::string get_response() { return this->response;}
        // void    setEnv(cahr **env) { this->env = env;}
        void   runCgi();
};
