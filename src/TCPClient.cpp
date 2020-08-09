#include "TCPClient.h"

TCPClient::TCPClient(std::string address, int port)
{
    this->sockfd = -1;
    this->port = port;
    this->address = address;
}

bool TCPClient::setup()
{
    if (sockfd == -1)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            spdlog::error("[TCPClient] create socket failed");
            return -1;
        }
    }
    
    if ((signed)inet_addr(address.c_str()) == -1)
    {
        bzero(&server_address, sizeof(server_address));

        struct hostent *he;
        struct in_addr **addr_list;
        if (( he = gethostbyname(this->address.c_str()) ) == NULL)
        {
            spdlog::error("[TCPClient] gethostbyname failed." + this->address);
            return false;
        }
        addr_list = (struct in_addr**)he->h_addr_list;
        for (int i = 0; addr_list[i] != NULL; i++)
        {
            server_address.sin_addr = *addr_list[i];
            break;
        }
    }
    else
    {
        inet_pton(AF_INET, this->address.c_str(), &server_address.sin_addr);
    }
    
    
    server_address.sin_family = PF_INET;
    server_address.sin_port   = htons(port);

    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        spdlog::error("[TCPClient] connection failed.");
        return false;
    }
    
    return true;
}

int TCPClient::Send(std::string data)
{
    if (sockfd != -1)
    {
        if (send(sockfd, data.c_str(), data.length(), 0) < 0)
        {
            spdlog::error("[TCPClient] send data failed.");
            return -1;
        }
    }
    else
    {
        spdlog::error("[TCPClient] send data failed because sockfd == -1.");
        return -1;
    }
    return 0;
}

int TCPClient::Send(char* data)
{
    if (sockfd != -1)
    {
        if (send(sockfd, data, sizeof(data), 0) < 0)
        {
            spdlog::error("[TCPClient] send data failed.");
            return -1;
        }
    }
    else
    {
        spdlog::error("[TCPClient] send data failed because sockfd == -1.");
        return -1;
    }
    return 0;
}

std::string TCPClient::receive(int size)
{
    char buff[size];
    // memset(buff, 0, size);

    std::string res;
    int length = recv(sockfd, buff, size, 0);
    if (length < 0)
    {
        spdlog::error("[TCPClient] recv failed.");
        return "";
    }
    buff[length] = '0';
    res = std::string(buff);
    // spdlog::info("[TCPClient] recv msg: " + res + std::to_string(length));
    return res;
}

void TCPClient::closed()
{
    close(sockfd);
}