#include "../src/TCPClient.h"
#include "config.h"
#include "Login.h"
#include "NewOrder.h"
#include "CancelOrder.h"
#include "Retransmission.h"
#include "../protobuf/Gateway.pb.h"
#include <iostream>
#include <spdlog/spdlog.h>

void send_message(TCPClient& tcp_client, std::string message, int retry_time = 2)
{
    while (retry_time > 0)
    {
        bool res = tcp_client.Send(message);
        if (!res)
        {
            retry_time--;
            spdlog::error("[Client send_message] send message failed.");
            std::string err_msg = "[Client send_message] retry time: " + std::to_string(retry_time) + ".";
            spdlog::error(err_msg.c_str());
        }
        else
        {
            spdlog::info("[Client send_message] client send message success.");
            return;
        }
        usleep(600);
    }
}

void receive_message(TCPClient& tcp_client, std::string& receive_message)
{
    receive_message = tcp_client.receive();
    if (receive_message == "")
    {
        spdlog::info("[Client reveive_message] client receive empty message.");
    }
    return ;
}

int parse_reply(const std::string reply)
{
    /*返回0表示收到正确的回包，同时没有后续消息
      返回1表示收到正确的回包，还需要继续等待后续消息
      返回-1表示收到拒绝报文。*/
    GatewayProto::Standard_Message standard_message;
    GatewayProto::Standard_Header standard_header;
    standard_message.ParseFromString(reply);

    standard_header = standard_message.standardheader();
    
    if (standard_header.msgtype() == MSGTYPE::LOGON)
    {
        GatewayProto::Logon logon = standard_message.logon();
        // 服务端需要考虑用户多次登录的情况怎么处理
        return 0;
    }
    else if (standard_header.msgtype() == MSGTYPE::LOGOUT)
    {
        spdlog::info("[parse_reply] Get Logout");
        GatewayProto::Logout logout = standard_message.logout();
        return -1;
    }
    else if (standard_header.msgtype() == MSGTYPE::NEW_ORDER_REPLY)
    {
        GatewayProto::Transaction_Report transaction_report = standard_message.transaction_report();
        if (transaction_report.ordstatus() == "Partially Filled")
        {
            spdlog::info("[parse_reply] get Paritially Filled.");
            return 1;
        }
        else if (transaction_report.ordstatus() == "Filled")
        {
            spdlog::info("[parse_reply] get All Filled.");
            return 0;
        }
    }
    else if (standard_header.msgtype() == MSGTYPE::EXECUTIVE_REPORT)
    {
        GatewayProto::Executive_Report executive_report = standard_message.executive_report();
        spdlog::info("[parse_reply] receive EXECUTIVE_REPORT.");
        if (executive_report.ordstatus() == "New")
        {
            spdlog::info("[parse_reply] EXECUTIVE_REPORT: Accept.");
            return 1;
        }
        else if (executive_report.ordstatus() == "Rejected")
        {
            spdlog::info("[parse_reply] EXECUTIVE_REPORT: Rejected.");
            return -1;
        }
    }
    else if (standard_header.msgtype() == MSGTYPE::CANCEL_ORDER_FAIL_REPORT)
    {
        GatewayProto::Cancel_Order_Fail cancel_order_fail = standard_message.cancel_order_fail();
        if (cancel_order_fail.ordstatus() == "Rejected")
        {
            spdlog::info("[parse_reply] Cancel Order Rejected.");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    Config config;
    config.address = "127.0.0.1";
    config.port = atoi(argv[1]);
    
    TCPClient tcp(config.address, config.port);
    bool setup_success = tcp.setup();

    std::string reply;
    {
        // 测试登录
        LogonMessage logon_message(config);

        GatewayProto::Standard_Message *standard_message = new GatewayProto::Standard_Message();
        logon_message.set_config(standard_message);
        std::string msg;
        standard_message->SerializeToString(&msg);

        // spdlog::info("size: " + std::to_string(msg.size()) +" -- header: " + std::to_string(standard_message->standardheader().msgtype()));
        int res = 1;
        int retry_times = 3;
        while (res != 0 && retry_times > 0)
        {
            // usleep(2000000);
            retry_times--;
            spdlog::info("[Client] Sending message for logining.");
            res = tcp.Send(msg);
            spdlog::info(msg.c_str());
            if (res == -1)
            {
                spdlog::error("[Client] tcp Send failed res == -1.");
                // dusleep(3000);
                continue;
            }
            reply = tcp.receive();
            if (reply == "")
            {
                res = 1;
                spdlog::error("[Client] tcp receive failed reply == NULL.");
                // usleep(3000);
                continue;
            }
            spdlog::info("[Client] get reply message:" + reply);
            res = parse_reply(reply);
            if (res == 1)
            {
                spdlog::error("[Client] receive logout when trying logon.");
                // usleep(3000);
                return 0;
            }
        }
        if (retry_times == 0)
        {
            spdlog::error("[Client] try logon 3 times failed.");
            return 0;
        }

        if (res == 0)
        {
            spdlog::info("[Client] Connect to TGW successfully.");
        }
    }

    // 测试发出新订单
    spdlog::info("[Client] 测试发出新订单.");
    {
        NewOrderMessage new_order_message(config);
        GatewayProto::Standard_Message *standard_message = new GatewayProto::Standard_Message();
        new_order_message.set_config(standard_message);

        std::string msg;
        int res;
        standard_message->SerializeToString(&msg);

        standard_message->ParseFromString(msg);
        res = tcp.Send(msg);
        if (res == -1)
        {
            spdlog::error("[Client] tcp Send NewOrderMessage failed.");
        }

            // 解析执行报告
        reply = tcp.receive(40960);
        if (reply == "")
        {
            spdlog::error("[Client] tcp receive NewOrderReply failed.");
        }

        res = parse_reply(reply);
        if (res == -1 || res == 0)
        {
            spdlog::error("[Client] receive NewOrderReply, fail to execute order.");
        }
        else
        {
            // 解析成交报告
            reply = tcp.receive(40960);
            spdlog::info("[Client] receive transaction message: " + reply);
            res =parse_reply(reply);
        }
    }
    spdlog::info("[Client] 测试发出新订单结束.");
    // {
    //     CancelOrderMessage cancel_order_message(config);
    //     GatewayProto::Standard_Message *standard_message = NULL;
    //     cancel_order_message.set_config(standard_message);

    //     std::string msg;
    //     int res;
    //     standard_message->SerializeToString(&msg);
    //     res = tcp.Send(msg);

    //     if (res == -1)
    //     {
    //         spdlog::error("[Client] tcp Send CancelOrderMessage failed.");
    //     }

    //         // 解析执行报告
    //     reply = tcp.receive();
    //     if (reply == "")
    //     {
    //         spdlog::error("[Client] tcp receive CancelOrderMessage failed.");
    //     }

    //     res = parse_reply(reply);
    //     if (res == -1)
    //     {
    //         spdlog::error("[Client] receive CancelOrderMessage, fail to execute order.");
    //     }
    //     else if (res == 0)
    //     {
    //         spdlog::error("[Client] receive CancelOrderMessage, success.");
    //     }
    //     else
    //     {
    //         // 申请已成交的部分订单
    //         reply = tcp.receive();
    //     }
        
    // }
}