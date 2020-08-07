#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <vector>
#include <map>
#include <sys/socket.h>
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

#define MAXPACKSIZE 40960
#define MAX_CLIENT 1000

// 用socketMessage来存放所有accept的客户端sockfd
struct SocketMessage
{
    int sockfd = -1;
    std::string ip = "";
    int id = -1;
    std::string message = ""; 
    bool is_running = false;
    // TODO: 加入当前客户端发送的信息的唯一性递增编号;用于之后进行信息重传
    // TODO: 加入用于判断当前用户是否在线的字段，用于资源回收。
};

class TCPServer
{
public:
    
    /*
    初始化socket，同时通过opt来setsockopt
    开始监听socket
    */
    int setup(int port, std::string ip, std::map<int, int> opt = std::map<int, int>());
    
    /*
    调用accept方法来得到客户端的sockfd,存入newsockfd中
    */    
    int accepted();
    
    /*
    发送buff中的数据到对应id的sockfd中
    */
    int Send(std::string buff, int id);

    /*
    关闭对应id的socket
    */
    void detach(int id);

    void clean(int id);

    bool is_online();

    std::vector<SocketMessage*> getMessage();

    std::string get_ip_addr(int id);

    int get_last_closed_socket();

    void* heart_handle(void* arg);
    
    void closed();

private:
    int sockfd, n, pid;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t server_thread[MAX_CLIENT];

    static std::vector<SocketMessage*> newsockfd; // 存放接入的客户端信息
    static char  msg[MAXPACKSIZE];
    static std::vector<SocketMessage*> Message; // 存放客户端发来的消息

    static bool is_onlined;
    static int last_closed_id;
    static int num_client;
    static std::mutex mt;
    static void* Task(void* argv);
};


#endif