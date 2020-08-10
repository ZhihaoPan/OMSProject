#include "../protobuf/Gateway.pb.h"
#include "../src/TCPServer.h"
#include "reply/LoginReply.h"
#include "reply/LogoutReply.h"
#include "reply/CancelOrderReply.h"
#include "reply/ExecutiveReport.h"
#include "reply/TransactionReport.h"
#include "reply/HeartBeatReport.h"
#include "User.h"
#include "config.h"
#include "Message.h"
#include <random>
#include <iostream>
#include <map>
#include <spdlog/spdlog.h>
using namespace std;

TCPServer tcp_server;
pthread_t send_thread[MAX_CLIENT];
int num_message = 0;

// TODO:通信过程中的中间信息放在哪?比如某个客户端当前处于什么状态 放在socketMessage中？

std::string construct_message(Message* message)
{

    GatewayProto::Standard_Message *standard_message = new GatewayProto::Standard_Message();
    message->set_config(standard_message);
    
    std::string msg;
    standard_message->SerializeToString(&msg);
    free(standard_message);
    return msg;
}

int parse_logon_content(const GatewayProto::Logon& logon, int id, int sockfd)
{
    // TODO:将recv_msg转化protobuf的对象
    int val = rand() % 2;
    Config config;

    if (val == 0)
    {
        spdlog::info("[Server test sockfd: " + std::to_string(sockfd) + " ] reply logon success message.");
        LogonReplyMessage logon_reply_message(config);
        std::string send_msg = construct_message(&logon_reply_message);
        tcp_server.Send(send_msg, id);

    }
    else if (val == 1)
    {
        spdlog::info("[Server test sockfd: " + std::to_string(sockfd) + " ] reply logon failed message.");
        LogoutReplyMessage logout_message(config);
        std::string send_msg = construct_message(&logout_message);
        tcp_server.Send(send_msg, id);
    }
    return 0;
}

int parse_heartbeat_content(const GatewayProto::Heart_Beat& hb, int id, int sockfd)
{
    // 如果是返回的心跳包信息就跳过
    spdlog::info("[Server test sockfd: " + std::to_string(sockfd) + " ] reply heartbeat message.");
    // Config config;
    // HeartBeatReportMessage heartbeat_message(config);
    // std::string send_msg = construct_message(&heartbeat_message);
    // tcp_server.Send(send_msg, id);
    return 0;
}

int parse_neworder_content(const GatewayProto::New_Order& no, int id, int sockfd)
{
    Config config;
    // 分三种情况 拒绝 接受全部成交 接受部分成交
    int val = rand() % 2;
    if (val == 0)
    {
        // 拒绝订单
        spdlog::info("[NewOrder Reply sockfd: " + std::to_string(sockfd) + " ] Rejected.");
        ExecutiveReportConfig er_config(config);
        er_config.ExecType = "Rejected";
        er_config.OrdStatus = "Rejected";
        er_config.OrderQty = no.orderqty();
        ExecutiveReportMessage executive_report(er_config);
        std::string send_msg = construct_message(&executive_report);
        tcp_server.Send(send_msg, id);
    }
    else if (val == 1)
    {
        // 接受全部成交
        spdlog::info("[NewOrder Reply sockfd: " + std::to_string(sockfd) + " ] New Order accepted.");
        ExecutiveReportConfig er_config(config);
        er_config.ExecType = "New";
        er_config.OrdStatus = "New";
        er_config.OrderQty = no.orderqty();
        er_config.LeavesQty = no.orderqty();
        ExecutiveReportMessage executive_report(er_config);
        std::string send_msg = construct_message(&executive_report);
        tcp_server.Send(send_msg, id);

        // usleep(10000);
        spdlog::info("[NewOrder Reply sockfd: " + std::to_string(sockfd) + " ] All Filled."); //  last send: "  + std::to_string(send_msg.length()) + send_msg);
        TransactionReportConfig tr_config(config);
        tr_config.ExecType = "Trade";
        tr_config.OrdStatus = "Filled";
        tr_config.CumQty = no.orderqty();
		tr_config.LeavesQty = 0;
		tr_config.LastQty = no.orderqty();
		TransactionReportMessage transaction_report(tr_config);
		send_msg = construct_message(&transaction_report);
		tcp_server.Send(send_msg, id);
    }
	else if (val == 2)
	{
		// 部分分批成交
		// 需要设计一个类保存客户端的状态信息，要不然没法实现部分成交后撤单的场景
		// 需要设计一下不同股票的类 还有一个 用户当前情况的类（保存用户目前的订单类型，订单数目，订单成交状态）
		// 整体的逻辑是 每日登录时，把用户的目前订单拿出来存到用户对象中，
	}
	return 0;
}

int parse_cancelorder_content(const GatewayProto::Cancel_Order& co, int id, int sockfd)
{
    return 0;
}

int parse_retransmit_content(const GatewayProto::Retransmission_Report& rr, int id, int sockfd)
{
    return 0;
}

int send_heartbeat_message(void* arg)
{
    spdlog::info("[server test] Try send heartbeat message.");
    // struct SocketMessage *socket_message = (struct SocketMessage*) arg;

    // Config config;
    // HeartBeatReportMessage heartbeat_message(config);
    // std::string send_msg = construct_message(&heartbeat_message);
    // tcp_server.Send(send_msg, socket_message->id);
    return 0;
}

void* send_client(void* arg) 
{
    // 遍历Message数组，
    struct SocketMessage *socket_message = (struct SocketMessage*) arg;

    spdlog::info("[Server test sockfd: " + std::to_string(socket_message->sockfd) + "] GetMessage From id:  " + to_string(socket_message->id));

    if(!tcp_server.is_online() || socket_message->id == -1) // tcp_server.get_last_closed_socket() == socket_message->id || 
    {
        spdlog::info("[Server test sockfd: " + std::to_string(socket_message->sockfd) + "] send_client thread exit \
                    , for " + "is_online" + std::to_string(tcp_server.is_online()) + 
                    " ,id ==" + std::to_string(socket_message->id));
        pthread_exit(NULL);
        return 0;
    }
    GatewayProto::Standard_Message recv_sm;
    
    recv_sm.ParseFromString(socket_message->message);

    // spdlog::error(recv_sm.new_order().orderqty());

    // 获取Message的头部 判断是哪个字段
    GatewayProto::Standard_Header recv_sh;
    recv_sh = recv_sm.standardheader();

    // int res = 0;
    if (recv_sh.msgtype() == MSGTYPE::LOGON)
    {
        parse_logon_content(recv_sm.logon(), socket_message->id, socket_message->sockfd);
    }
    else if (recv_sh.msgtype() == MSGTYPE::HEART_BEAT)
    {
        // 接受心跳包进行处理，接受的到了心跳包就
        parse_heartbeat_content(recv_sm.heart_beat(), socket_message->id, socket_message->sockfd);
    }
    else if (recv_sh.msgtype() == MSGTYPE::NEW_ORDER)
    {
        parse_neworder_content(recv_sm.new_order(), socket_message->id, socket_message->sockfd);
    }
    else if (recv_sh.msgtype() == MSGTYPE::CANCEL_ORDER)
    {

    }
    else if (recv_sh.msgtype() == MSGTYPE::RETRANSIMIT)
    {

    }

    pthread_exit(NULL);
    return 0;
}

void* timeout_proc(void* arg)
{
    while (1)
    {
        auto s_timeoutfd = tcp_server.getTimeoutfd();
        
        if (s_timeoutfd.size() == 0)
        {
            pthread_mutex_lock(&TCPServer::mt_timeout);
            pthread_cond_wait(&TCPServer::cond, &TCPServer::mt_timeout);
            s_timeoutfd = tcp_server.getTimeoutfd();
            for (auto iter = s_timeoutfd.begin(); iter !=s_timeoutfd.end(); iter++)
            {
                struct SocketMessage *socket_message = *iter;
                // q_timeoutfd.pop();
                send_heartbeat_message(socket_message);
            }
            pthread_mutex_unlock(&TCPServer::mt_timeout);
        }
        else
        {
            for (auto iter = s_timeoutfd.begin(); iter !=s_timeoutfd.end(); iter++)
            {
                struct SocketMessage *socket_message = *iter;
                // q_timeoutfd.pop();
                send_heartbeat_message(socket_message);
            }
        }
        
    }
    pthread_exit(NULL);
    return 0;
}


void* received(void* arg)
{
    pthread_detach(pthread_self());
    // TODO: 心跳包中处理异常中断的连接
    // pthread_t timeout_tid;
    // if (pthread_create(&timeout_tid, NULL, timeout_proc, NULL) != 0)
    // {
    //     spdlog::error("[Server test] creating timeout_thread failed.");
    // }

    
    vector<SocketMessage*> v_socket_message;
    while(1)
    {
        v_socket_message = tcp_server.getMessage();
        
        for(int i = 0; i < (int)v_socket_message.size(); i++) 
        {
            if(v_socket_message[i]->message != "")
            {
                if(!v_socket_message[i]->is_running) 
                {
                    v_socket_message[i]->is_running = true;
                    spdlog::info("[Server test sockfd: " + std::to_string(v_socket_message[i]->sockfd) + "] start.");
                    // TODO:处理消息线程
                    if( pthread_create(&send_thread[num_message], NULL, send_client, (void *) v_socket_message[i]) != 0) 
                    {
                        spdlog::error("[Server test] server creating worker thread failed.");
                    }
                    num_message++;
                    
                }
                else
                {
                    // spdlog::error("[debug] sockfd: " + std::to_string(i) + " is running.");
                }
                
                // tcp_server.clean(i); // 上面线程还没处理好这里就clean掉了    
                // 为了避免socket_message的大小不断增大，应该主动设置下大小，把message空白的清除掉。
            }
            else
            {
                spdlog::error("[Server test] v_socket_message->message == NULL sockfd: " + std::to_string(v_socket_message[i]->sockfd) + " is running.");
            }
        }
        usleep(2000000);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage:./server_test port" << endl;
    }
    pthread_t receive_thread;
    map<int, int> opts = { {SO_REUSEPORT, 1}, {SO_REUSEADDR, 1} };
    
    if(tcp_server.setup(atoi(argv[1]), "127.0.0.1", opts) == 0) 
    {
        if(pthread_create(&receive_thread, NULL, received, (void *)0) == 0)
        {
            while(1) 
            {    
                tcp_server.accepted();
            }
        }
    }
    return 0;
}
