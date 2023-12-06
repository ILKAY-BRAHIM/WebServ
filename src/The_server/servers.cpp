#include "Servers.hpp"

Servers::Servers(std::string request)
{
    this->request = request;
    // std::cout << "request: " << this->request << std::endl;
}

// Servers::Server(const Responce &responce)
// {
//     this->responce = responce;
// }

void Servers::collect_req(std::string &request)
{
    this->request += request;
    // std::cout << "request: " << this->request << std::endl;
    return;
}