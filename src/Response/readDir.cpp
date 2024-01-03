#include "response.hpp"

void    get_size(t_info &info, size_t size)
{
    double s;
    if (size >= GB)
    {
        s = size / GB;
        info.unity = "GB";
    }
    else if (size >= MB)
    {
        s = size / MB;
        info.unity = "MB";
    }
    else if (size >= KB)
    {
        s = size / KB;
        info.unity = "KB";
    }
    else 
    {
        s = size;
        info.unity = "B";
    }
    std::ostringstream ss;
    ss << s;
    info.size = ss.str();
    return ;
}

t_Dir_Data   readDirectory(std::string path)
{
    t_Dir_Data   data;
    DIR *dir;
    struct dirent *r;
    std::string ini_dir(path);
    dir = opendir(ini_dir.c_str());
    if (dir == NULL)
        throw (500);
    while (1)
    {
        r = readdir(dir);
        if (r != NULL)
        {
            t_info tmp;
            tmp.name = r->d_name;
            if (tmp.name == "." || tmp.name == "..")
                continue;
            std::string tmp_f;
            tmp_f = ini_dir ;
            if (tmp_f[tmp_f.size() - 1] != '/')
                tmp_f += '/';
            tmp_f += tmp.name;
            struct stat tmp_st;
            if (stat(tmp_f.c_str(), &tmp_st) == 0)
            {
                if (S_ISDIR(tmp_st.st_mode))
                    tmp.type = "Folder";
                else if (S_ISREG(tmp_st.st_mode))
                {
                    std::string ext = get_extension(tmp_f);
                    for (size_t i = 0; i < ext.size(); i++)
                        ext[i] = std::toupper(ext[i]);
                    tmp.type = ext;
                    tmp.type += " File";
                }
                else
                    continue;
                struct tm *time;
                char strTime[100];
                time = localtime(&tmp_st.st_mtime);
                strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", time);
                tmp.date = std::string(strTime);
                size_t size = (size_t)tmp_st.st_size;
                get_size(tmp, size);
                tmp.size += (" " + tmp.unity);
            }
            if (tmp.type.size() != 0)
            {
                if (tmp.type == "Folder")
                    data.directory.insert(std::make_pair(tmp.date, tmp));
                else
                    data.file.insert(std::make_pair(tmp.date, tmp));
            }
        }
        else 
            break ;
    }
    closedir(dir);
    return (data);
}