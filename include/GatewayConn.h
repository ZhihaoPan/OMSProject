#ifndef GATEWAYCONN_H
#define GATEWAYCONN_H

#include <vector>
#include <map>
#include <set>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <spdlog/spdlog.h>
#include "Lockers.h"
#include "Threadpool.h"

class GatewayConn
{
public:
    GatewayConn(){}
    ~GatewayConn(){}

public:
    // epollout 首先接受数据到缓冲区中，然后调用write发送数据。
    int write();
    // epollin 首先接受数据到缓冲区中，把该对象加入到线程中，线程中处理业务逻辑；同时将epollfd设置epollout
    int read();  
    // 处理业务逻辑的代码，从缓冲区中取出数据，通过protobuf转化成对象，然后处理业务信息。
    int Task();
    // 关闭该客户端连接
    int close_conn();
    // 初始化连接
    int init();
    int init(int sockfd, const struct sockaddr_in& addr);
    


public:
    std::string get_message();
    std::string get_ip();
    int get_report_index();
    int get_sockfd();
public:
    static int m_epollfd;
    static int m_client_count;
    static const int READPACKSIZE = 40960;
    static const int WRITEPACKSIZE = 40960;

private:
    int m_sockfd;
    std::string m_ip;
    struct sockaddr_in m_address;
    std::string m_message;
    int m_report_index;
    
    char read_buff[READPACKSIZE];
    char write_buff[WRITEPACKSIZE];
    int m_read_index;
    int m_write_index;
};

#endif