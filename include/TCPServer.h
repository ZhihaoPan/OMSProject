#ifndef TCP_SERVER_H
#define TCP_SERVER_H

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

class TCPServer
{
public:
    
    /*
    初始化socket，同时通过opt来setsockopt
    开始监听socket
    */
    int setup(int port, std::string ip);
    
    /*
    调用accept方法来得到客户端的sockfd,存入newsockfd中
    */    
    int accepted();
    
    /*
    关闭对应id的socket
    */
    void detach(int id);

    void clean(int id);

    bool is_online();

    std::vector<SocketMessage*> getMessage();

    std::set<SocketMessage*> getTimeoutfd();

    std::string get_ip_addr(int id);

    int get_last_closed_socket();

    void* heart_handle(void* arg);
    
    void closed();

    static pthread_mutex_t mt_newsock, mt_timeout, mt_message;
    static pthread_cond_t cond;
private:
    int sockfd, n, pid;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t server_thread[MAX_CLIENT];

    // 存放接入的客户端信息
    static std::vector<SocketMessage*> newsockfd; 
    // 存放超时的客户端信息 超时判断通过sockopt来设置超时时间
    static std::set<SocketMessage*> timeoutfd; 
    // 存放客户端发来的消息
    static std::vector<SocketMessage*> Message; 
    static char  msg[MAXPACKSIZE];
    // 条件变量用于提示服务端处理超时的socket
    static bool is_onlined;
    static int last_closed_id;
    static int num_client;
    static void* Task(void* argv);
};


#endif