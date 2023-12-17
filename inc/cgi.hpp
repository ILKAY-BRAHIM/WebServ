#pragma once

class Cgi {
    private:
        char **env;
        std::string body;
    public:
        Cgi(char **env, std::string body) : env(env), body(body) {}
        ~Cgi();
        char **getEnv() { return this->env;}
        char *get_path() {for (int i = 0; this->env[i]; i++) {if (strncmp(this->env[i], "SCRIPT_FILENAME=", 16) == 0) {return this->env[i] + 16;}} return NULL;}
        // void    setEnv(cahr **env) { this->env = env;}
        void   runCgi();
};
