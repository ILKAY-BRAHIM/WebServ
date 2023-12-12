#include "Servers.hpp"

Servers::Servers(std::string request)
{
    // this->request = request;
    (void)request;
    request.clear();
    this->total = 0;
    partis = 0;
    // std::cout << "request: " << this->request << std::endl;
}

// Servers::Server(const Responce &responce)
// {
//     this->responce = responce;
// }

void Servers::collect_req(char* request)
{
    // this->request += request;
    this->request.append(request, 1);
    // std::cout << "request: " << this->request << std::endl;
    return;
}

// void Servers::set_responce(Message *responce)
// {
//     this->responce = responce;
//     return;
// }