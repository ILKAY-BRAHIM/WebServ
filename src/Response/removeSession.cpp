#include "response.hpp"
#include <sys/stat.h>
#include <ctime>

void    clearDirectory(std::string ini_dir)
{
    DIR *dir;
    struct dirent *r;
    dir = opendir(ini_dir.c_str());
    if (dir == NULL)
        return ;
    std::string name;
    
    while (1)
    {
        r = readdir(dir);
        if (r != NULL)
        {
            name = r->d_name;
            if (name == "." || name == "..")
                continue;
            struct stat fileStat;
            name = ini_dir + name;
            if (stat(name.c_str(), &fileStat) == 0)
            {
                time_t now = time(0);
                time_t last = fileStat.st_atime;
                if (now - last > 60)
                    remove(name.c_str());
            }
        }
        else
            break;
    }
    closedir(dir);
}
