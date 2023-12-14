#pragma once

class Cgi {
    private:
        char **env;
    public:
        Cgi();
        ~Cgi();
        void    setEnv(cahr **env) { this->env = env;}
        void   runCgi();
};
