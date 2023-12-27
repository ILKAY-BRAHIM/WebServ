#include "cgi.hpp"

int Cgi::runCgi()
{
    char bufferr[30000];
    int status;
    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << "Pipe failed\n";
        this->valid = -1;
        return (-1);
    }
    pid_t pid = fork();
    if (pid == -1) {
        this->valid = -1;
        std::cerr << "Fork failed\n";
        return (-1);
    }
    else if (pid == 0) 
    {
        std::string fill = "fill";
        std::ofstream outputFile(fill, std::ios::out | std::ios::trunc);
        outputFile << this->body << std::endl;
        int fd_p = open(fill.c_str(), O_RDONLY);
        if((dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1 || dup2(fd_p, 0) == -1))
        {
            this->valid = -1;
            close(fd_p);
            throw "dup2 failed";
        }
        char *argv[] = {get_path(), NULL};
        execve(get_path(), argv, this->env);
        close(fd_p);
        throw "CGI failed\n";
    } 
    else 
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            this->valid = -1;
            close(fd[0]);
            close(fd[1]);
            return (-1);
        }
        close(fd[1]);

        while (read(fd[0], bufferr, 3000) > 0)
        {
            this->response += bufferr;
            memset(bufferr, 0, 3000);
        }
        close(fd[0]);
    }
    return (1);
}