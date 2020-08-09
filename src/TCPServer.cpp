#include "TCPServer.h"

std::vector<SocketMessage*> TCPServer::newsockfd;
std::vector<SocketMessage*> TCPServer::Message;
std::set<SocketMessage*> TCPServer::timeoutfd;
char TCPServer::msg[MAXPACKSIZE];
bool TCPServer::is_onlined = false;
int TCPServer::last_closed_id = -1;
int TCPServer::num_client = 0;
pthread_mutex_t TCPServer::mt_newsock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t TCPServer::mt_timeout = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t TCPServer::mt_message = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t TCPServer::cond = PTHREAD_COND_INITIALIZER;

void* TCPServer::Task(void* arg)
{
    int res;
    struct SocketMessage* socket_message = (struct SocketMessage*)arg;
    pthread_detach(pthread_self());

    while (1)
    {
        // 发送接收的超时时间都是在setsockopt中设置的
        // TODO:找下怎么处理大量连接，避免创建大量线程
        // TODO:其中一个客户端断开，马上重连会出现core dump
        // 问题出在第二次发送的时候sockfd是一样的，因此就从同一个recv进来，放入同一个socket_message中此时isrunning == true;
        // socket_message是指向同一个地址空间的内容，如果说Message传到后面去
        res = recv(socket_message->sockfd, msg, MAXPACKSIZE, 0);
        // 每次达到超时时间就发送一个心跳包，去判断客户端连接是否还在，设计一个超时的sockfd队列，链表中的都是超时的sockfd,对这些fd在应用层上做心跳包的检测
        if (res > 0)
        {
            // 正常处理数据
            msg[res] = '0';
            // socket_message->message.assign(msg,res);
            spdlog::info("[TCPServer] (id: " + std::to_string(socket_message->id) + ")show message" + std::string(msg) );
            socket_message->message = std::string(msg);
            is_onlined = true;
            
            if (socket_message->is_running == false)
            {
                pthread_mutex_lock(&mt_message);
                Message.push_back(socket_message);// 
                pthread_mutex_unlock(&mt_message);
            }
            else
            {
                socket_message->is_running = false;
            }
            
        }
        else
        {
            // recv超时，将socket放入timeoutfd中，
            if (res < 0 && errno == EAGAIN)
            {
                // TODO:心跳包改成定时发 线程安全
                pthread_mutex_lock(&mt_timeout);
                timeoutfd.insert(socket_message);
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mt_timeout);
            }
            
            if (res == 0)
            {
                spdlog::info("[TCPServer] Client try to cloes socket: " + std::to_string(socket_message->id));
                // 客户端申请正常断开连接，这里res就会为0
                last_closed_id = socket_message->id;
                close(socket_message->sockfd);
                is_onlined = false;

                int id = last_closed_id;
                
                pthread_mutex_lock(&mt_newsock);
                auto new_end = std::remove_if(newsockfd.begin(), newsockfd.end(), [id](SocketMessage* so){
                    return so->id == id;
                });
                newsockfd.erase(new_end, newsockfd.end());
                pthread_mutex_unlock(&mt_newsock);

                if (num_client > 0)
                {
                    num_client--;
                }
                
                break;
            }
            else
            {
                spdlog::error("[TCPServer] recv failed.");
                break;
            }
        }  // if res
        
        usleep(600);
    }  // while 

    // 线程退出释放资源
    // if (socket_message != NULL)
    // {
    //     free(socket_message);
    // }
    pthread_exit(NULL);
    return 0;
}

int TCPServer::setup(int port, std::string ip, std::map<int, int> opts)
{
    is_onlined = false;
    last_closed_id = -1;
    pthread_mutex_init(&mt_newsock, NULL);
    pthread_mutex_init(&mt_timeout, NULL);
    pthread_mutex_init(&mt_message, NULL);
    pthread_cond_init(&cond, NULL);


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
    // 需要建立长连接
    bzero(&client_address, sizeof(client_address));
    socklen_t client_addrlength   = sizeof(client_address);
    SocketMessage *socket_message = new SocketMessage;
    spdlog::info(inet_ntoa(client_address.sin_addr));
    socket_message->sockfd        = accept(sockfd, (struct sockaddr*)&client_address, &client_addrlength);
    socket_message->ip            = inet_ntoa(client_address.sin_addr);
    socket_message->id            = num_client;
    socket_message->is_running    = false;

    if (socket_message->sockfd < 0)
    {
        spdlog::error("[TCPServer] Connecetion failed.");
        return -1;
    }
    newsockfd.push_back(socket_message);

    spdlog::info("[TCPServer] accept new client socket: " + std::to_string(socket_message->sockfd));
    
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
    newsockfd[id]->sockfd  = -1;
    newsockfd[id]->ip      = "";
    newsockfd[id]->id      = -1;
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
    pthread_mutex_lock(&mt_message);
    auto temp_message = Message;
    pthread_mutex_unlock(&mt_message);
    return temp_message;
} 


std::set<SocketMessage*> TCPServer::getTimeoutfd()
{
    pthread_mutex_lock(&mt_timeout);
    auto temp_timeout = timeoutfd;
    pthread_mutex_unlock(&mt_timeout);
    return temp_timeout;
}

std::string TCPServer::get_ip_addr(int id)
{
    return newsockfd[id]->ip;
}

int TCPServer::get_last_closed_socket()
{
    return last_closed_id;
}
