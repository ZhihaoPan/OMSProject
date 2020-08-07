#include "TCPServer.h"

std::vector<SocketMessage*> TCPServer::newsockfd;
std::vector<SocketMessage*> TCPServer::Message;
char TCPServer::msg[MAXPACKSIZE];
bool TCPServer::is_onlined = false;
int TCPServer::last_closed_id = -1;
int TCPServer::num_client = 0;
std::mutex TCPServer::mt;

void* TCPServer::Task(void* arg)
{
    int res;
    struct SocketMessage* socket_message = (struct SocketMessage*)arg;
    pthread_detach(pthread_self());

    while (1)
    {
        // 发送接收的超时时间都是在setsockopt中设置的
        res = recv(socket_message->sockfd, msg, MAXPACKSIZE, 0);
        // 每次达到超时时间就发送一个心跳包，去判断客户端连接是否还在，设计一个超时的sockfd链表，链表中的都是超时的sockfd,对这些fd在应用层上做心跳包的检测
        if (res != -1)
        {
            // TODO:socket退出应该新建立一个函数处理，因为是客户端那边申请取消连接，没办法通过res = 0来判断断开连接
            // 需要判断是不是已经存在在列表中的socket,如果已经存在表示是旧用户重新连上的服务端
            // 看一下recv和send能不能正确区分出用户信息 
            // 如果sockfd已经存在在newsockfd中，那么处理呢，之前Task线程是不是需要挂掉重开
            if (res == 0)
            {
                last_closed_id = socket_message->id;
                close(socket_message->sockfd);
                is_onlined = false;

                int id = last_closed_id;
                {// 设lck为局部变量
                    std::lock_guard<std::mutex> lck(mt);
                    auto new_end = std::remove_if(newsockfd.begin(), newsockfd.end(), [id](SocketMessage* so){
                        return so->id == id;
                    });

                    newsockfd.erase(new_end, newsockfd.end());
                    if (num_client > 0)
                    {
                        num_client--;
                    }
                }
                break;
            }
            else
            {   // 改成二进制
                // msg[res] = '\0';
                // socket_message->message.assign(msg,res);
                spdlog::info("[TCPServer] show message" + std::string(msg) + std::to_string((std::string(msg)).size()) + std::to_string(sizeof(msg)));
                socket_message->message = std::string(msg);
                is_onlined = true;
                {
                    std::lock_guard<std::mutex> lck(mt);
                    Message.push_back(socket_message);// 
                }
            }
        }
        usleep(600);
    }

    // 线程退出释放资源
    if (socket_message != NULL)
    {
        free(socket_message);
    }
    pthread_exit(NULL);
    return 0;
}

int TCPServer::setup(int port, std::string ip, std::map<int, int> opts)
{
    is_onlined = false;
    last_closed_id = -1;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        spdlog::error("[TCPServer] sockfd < 0");
        return -1;
    }
    
    bzero(&server_address,sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
    
    for (auto iter : opts)
    {
        if (setsockopt(sockfd, SOL_SOCKET, iter.first, (char*)&iter.second, sizeof(iter.second)) < 0)
        {
            spdlog::error("[TCPServer] set socket option failed.");
            return -1;
        }
    }
    
    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        spdlog::error("[TCPServer] bind socket failed");
        return -1;
    }

    if (listen(sockfd,5) < 0)
    {
        spdlog::error("[TCPServer] listen socket failed");
        return -1;
    }
    
    num_client = 0;

    is_onlined = true;

    return 0;
}

int TCPServer::accepted()
{
    // 需要建立长连接！！！！
    bzero(&client_address, sizeof(client_address));
    socklen_t client_addrlength   = sizeof(client_address);
    SocketMessage *socket_message = new SocketMessage;
    spdlog::info(inet_ntoa(client_address.sin_addr));
    socket_message->sockfd        = accept(sockfd, (struct sockaddr*)&client_address, &client_addrlength);
    socket_message->ip            = inet_ntoa(client_address.sin_addr);
    socket_message->id            = num_client;

    if (socket_message->sockfd < 0)
    {
        spdlog::error("[TCPServer] Connecetion failed.");
        return -1;
    }
    newsockfd.push_back(socket_message);

    // 对方异常断开处理
    pthread_create(&server_thread[num_client], NULL, &Task, (void*)newsockfd[num_client]);
    is_onlined = true;
    num_client++;

    return 0;
}

int TCPServer::Send(std::string buff, int id)
{
    SocketMessage *socket_message = newsockfd[id];
    send(socket_message->sockfd, buff.c_str(), buff.length(), 0);
    return 0;
}

void TCPServer::detach(int id)
{
    close(newsockfd[id]->sockfd);
    newsockfd[id]->sockfd = -1;
    newsockfd[id]->ip = "";
    newsockfd[id]->id = -1;
    newsockfd[id]->message = "";
}

void TCPServer::clean(int id)
{
    Message[id]->message = "";
    memset(msg, 0, MAXPACKSIZE);
}

bool TCPServer::is_online()
{
    return is_onlined;
}

std::vector<SocketMessage*> TCPServer::getMessage()
{
    std::lock_guard<std::mutex> lck(mt);
    return Message;
} 

std::string TCPServer::get_ip_addr(int id)
{
    return newsockfd[id]->ip;
}

int TCPServer::get_last_closed_socket()
{
    return last_closed_id;
}

