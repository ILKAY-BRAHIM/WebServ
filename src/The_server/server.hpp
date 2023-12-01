#include "Parse.hpp"
#include "RequestResponce.hpp"

class Server
{
    private:
        std::vector<t_server> servers;
        Server();
    public:
        Server(std::vector<t_server> servers);
        Server(const Server &other);
        Server &operator=(const Server &other);
        ~Server();
        void run();
};