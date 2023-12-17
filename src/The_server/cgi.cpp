#include "cgi.hpp"

void cgi::runCgi()
{
    char bufferr[30000];
    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << "Pipe failed\n";
        exit(1);
    }
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed\n";
        exit(1);
    } 
    else if (pid == 0) {
        if((dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1) || dup2(fd, 0))
        {
            std::cerr << "dup2 failed\n";
            exit(1);
        }
        char *argv[] = {get_path(), NULL};
        // char *envp[] = {"check=hi i'am hir", NULL};
        execve(get_path(), argv, this->env);
        exit(0);
    } 
    else 
    {
        waitpid(pid, NULL, 0);
        close(fd[1]);
        if (read(fd[0], bufferr, 3000) == -1)
        {
            std::cerr << "read failed\n";
            exit(1);
        }
        close(fd[0]);
    }
}