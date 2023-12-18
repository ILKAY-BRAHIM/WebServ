#include "cgi.hpp"

void Cgi::runCgi()
{
    char bufferr[30000];
    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << "Pipe failed\n";
        exit(1);
    }
    pid_t pid = fork();
    std::string fill = "fill";
    std::ofstream outputFile(fill, std::ios::out | std::ios::trunc);
    outputFile << this->body << std::endl;
    int fd_p = open(fill.c_str(), O_RDONLY);
    if (pid == -1) {
        std::cerr << "Fork failed\n";
        exit(1);
    }
    else if (pid == 0) {
        if((dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1 || dup2(fd_p, 0) == -1))
        {
            std::cerr << "dup2 failed\n";
            exit(1);
        }
        // std::string tmp_path = "php";
        // char cmd[] = "/usr/bin/php";
        char *argv[] = {get_path(), NULL};
        // char *envp[] = {"check=hi i'am hir", NULL};
        execve(get_path(), argv, this->env);
        std::cerr << "execve failed\n";
        exit(0);
    } 
    else 
    {
        waitpid(pid, NULL, 0);
        close(fd[1]);

        while (read(fd[0], bufferr, 3000) > 0)
        {
            // std::cout << bufferr;
            this->response += bufferr;
            memset(bufferr, 0, 3000);
        }
        // if (read(fd[0], bufferr, 3000) == -1)
        // {
        //     std::cerr << "read failed\n";
        //     exit(1);
        // }
        close(fd[0]);
    }
}