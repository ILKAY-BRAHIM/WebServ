#include "Servers.hpp"

Servers::Servers(std::string request)
{
    (void)request;
    request.clear();
    this->total = 0;
    partis = 0;
    bypass = 0;
    pass_total = 0;
}

void Servers::collect_req(char* request, int size)
{
    this->request.append(request, size);
    return;
}