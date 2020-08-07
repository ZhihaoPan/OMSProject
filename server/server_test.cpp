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

int parse_logon_content(const GatewayProto::Logon& logon, int id)
{
    // TODO:将recv_msg转化protobuf的对象
    int val = rand() % 2;
    Config config;

    if (val == 0)
    {
        LogonReplyMessage logon_reply_message(config);
        std::string send_msg = construct_message(&logon_reply_message);
        tcp_server.Send(send_msg, id);

    }
    else if (val == 1)
    {    
        LogoutReplyMessage logout_message(config);
        std::string send_msg = construct_message(&logout_message);
        tcp_server.Send(send_msg, id);
    }
    return 0;
}

int parse_heartbeat_content(const GatewayProto::Heart_Beat& hb, int id)
{
    Config config;
    HeartBeatReportMessage heartbeat_message(config);
    std::string send_msg = construct_message(&heartbeat_message);
    tcp_server.Send(send_msg, id);
    return 0;
}

int parse_neworder_content(const GatewayProto::New_Order& no, int id)
{
    Config config;
    // 分三种情况 拒绝 接受全部成交 接受部分成交
    int val = rand() % 3;
    if (val == 0)
    {
        // 拒绝订单
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
        ExecutiveReportConfig er_config(config);
        er_config.ExecType = "New";
        er_config.OrdStatus = "New";
        er_config.OrderQty = no.orderqty();
        er_config.LeavesQty = no.orderqty();
        ExecutiveReportMessage executive_report(er_config);
        std::string send_msg = construct_message(&executive_report);
        tcp_server.Send(send_msg, id);

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

int parse_cancelorder_content(const GatewayProto::Cancel_Order& co, int id)
{
    
}

int parse_retransmit_content(const GatewayProto::Retransmission_Report& rr, int id)
{

}

void* send_client(void* arg) 
{
    // 遍历Message数组，
    struct SocketMessage *socket_message = (struct SocketMessage*) arg;

    if(!tcp_server.is_online() || tcp_server.get_last_closed_socket() == socket_message->id || socket_message->id == -1) 
    {
        return 0;
    }
    GatewayProto::Standard_Message recv_sm;
    // spdlog::error(std::to_string(socket_message->message.size()));
    recv_sm.ParseFromString(socket_message->message);

    // 获取Message的头部 判断是哪个字段
    GatewayProto::Standard_Header recv_sh;
    recv_sh = recv_sm.standardheader();

    int res = 0;
    // 这里的判断可以用函数指针来简化；初始化map<int, void*> 
    if (recv_sh.msgtype() == MSGTYPE::LOGON)
    {
        parse_logon_content(recv_sm.logon(), socket_message->id);
    }
    else if (recv_sh.msgtype() == MSGTYPE::HEART_BEAT)
    {

    }
    else if (recv_sh.msgtype() == MSGTYPE::NEW_ORDER)
    {

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

void* received(void* arg)
{
    // TODO: 心跳包中处理异常中断的连接

    pthread_detach(pthread_self());
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
                    if( pthread_create(&send_thread[num_message], NULL, send_client, (void *) v_socket_message[i]) != 0) 
                    {
                        spdlog::error("[Server] server creating worker thread failed.");
                    }
                    num_message++;
                }
                
                // tcp_server.clean(i); // shadiao错误，上面线程还没处理好这里就clean掉了    
                // 为了避免socket_message的大小不断增大，应该主动设置下大小，把message空白的清除掉。
            }
        }
        usleep(1000);
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
                cout << "waiting for connection" << endl;
                tcp_server.accepted();
            }
        }
    }
    return 0;
}
