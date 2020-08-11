#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <string>
#include <spdlog/spdlog.h>

class TCPClient
{
public:
    TCPClient(std::string, int);
    
    bool setup();

    int Send(std::string data);

    int Send(char* data);

    std::string receive(int size = 40960);

    void closed();

private:
    int sockfd;
    int port;
    std::string address;
    struct sockaddr_in server_address;
};

#endif