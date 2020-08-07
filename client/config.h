#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

enum MSGTYPE
{
    LOGON = 1,
    LOGOUT = 2,
    HEART_BEAT = 3,
    RETRANSIMIT = 5,
    NEW_ORDER = 100101, 
    EXECUTIVE_REPORT = 200102,         // 订单被接受或者拒绝 ，撤单成功也是这个报告
    NEW_ORDER_REPLY = 200115,          // 新订单成交还是部分成交
    CANCEL_ORDER = 190007,      
    CANCEL_ORDER_FAIL_REPORT = 290008, // 撤单被拒绝
};

struct Extend_Fields
{
    float StopPx;
    float MinQty;
    int MaxPriceLevels;
    std::string TimeInForce;
    std::string CashMargin;  
};

struct Config
{
    std::string address;
    int port;
    int receive_size = 4096;
};

class LogonConfig
{
public:
    LogonConfig(){}
    LogonConfig(Config config):config(config){}
    Config config;
    // Logon字段
    int MsgType = MSGTYPE::LOGON;
    int BodyLength = 5;
    std::string SenderCompID = "sendeID1";
    std::string TargetCompID = "targetID1";
    int HeartBtInt = 5;
    std::string Password = "password";
    std::string DefaultApplVerID = "1.10";
    
};

class LogoutConfig
{
public:
    LogoutConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::LOGOUT;
    int BodyLength = 2;
    // Logout字段
    int SessionStatus;
    std::string Text;
};

class HeartBeatConfig
{
public:
    HeartBeatConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::HEART_BEAT;
    int BodyLength = 0;
};

class NewOrderConfig
{
public:
    NewOrderConfig(){}
    NewOrderConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::NEW_ORDER;
    int BodyLength = 16;
    // 新订单字段
    std::string ApplID;
    std::string SubmittingPBUID;
    std::string SecurityID;
    std::string SecurityIDSource;
    int OwnerType;
    std::string ClearingFirm;
    std::string TransactTime;
    std::string UserInfo;
    std::string ClOrdID;
    std::string AccountID;
    std::string BranchID;
    std::string OrderRestrictions;
    std::string Side;
    std::string OrdType;
    int OrderQty;
    float Price;
    Extend_Fields ExtendFields;
};

class CancelOrderConfig
{
public:
    CancelOrderConfig(){}
    CancelOrderConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::CANCEL_ORDER;
    int BodyLength = 13;
    // 撤销订单
    std::string ApplID;
    std::string SubmittingPBUID ;
    std::string SecurityID ;
    std::string SecurityIDSource ;
    int OwnerType ;
    std::string ClearingFirm ;
    std::string TransactTime ;
    std::string UserInfo ;
    std::string ClOrdID ;
    std::string OrigClOrdID ;
    std::string Side ;
    std::string OrderID ;
    int OrderQty ;
};

class RetransmissionConfig
{
public:
    RetransmissionConfig(){}
    RetransmissionConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::RETRANSIMIT;
    int BodyLength = 3;
    // 请求重传报告
    int NoPartitions;
    int* PartitionNo;
    int* ReportIndex;

};



class ExecutiveReportConfig
{
public:
    ExecutiveReportConfig(){}
    ExecutiveReportConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::EXECUTIVE_REPORT;
    int BodyLength = 17;

    int PartitionNo;
    int ReportIndex;
    std::string ApplID;
    std::string ReportingPBUID;
    std::string SubmittingPBUID;
    std::string SecurityID;
    std::string SecurityIDSource;
    int OwnerType;
    std::string ClearingFirm;
    std::string TransactTime;
    std::string UserInfo;
    std::string OrderID;
    std::string ClOrdID;
    std::string OrigClOrdID;
    std::string ExecID;
    std::string ExecType;
    std::string OrdStatus;
    std::string OrdRejReason;
    int LeavesQty;
    int CumQty;
    std::string Side;
    std::string OrdType;
    int OrderQty;
    float Price;
    std::string AccountID;
    std::string BranchID;
    std::string OrderRestrictions;
    Extend_Fields ExtendFields;
};

class TransactionReportConfig
{
public:
    TransactionReportConfig(){}
    TransactionReportConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::NEW_ORDER_REPLY;
    int BodyLength = 23;

    int PartitionNo;
    int ReportIndex;
    std::string ApplID;
    std::string ReportingPBUID;
    std::string SubmittingPBUID;
    std::string SecurityID;
    std::string SecurityIDSource;
    int OwnerType;
    std::string ClearingFirm;
    std::string TransactTime;
    std::string UserInfo;
    std::string OrderID;
    std::string ClOrdID;
    std::string ExecID;
    std::string ExecType;
    std::string OrdStatus;
    float LastPx;
    int LastQty;
    int LeavesQty;
    int CumQty;
    std::string Side;
    std::string AccountID;
    std::string BranchID;
    Extend_Fields ExtendFields;
};

class CancelOrderFailConfig
{
public:
    CancelOrderFailConfig(){}
    CancelOrderFailConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::CANCEL_ORDER_FAIL_REPORT;
    int BodyLength = 19;

    int PartitionNo;
    int ReportIndex;
    std::string ApplID;
    std::string ReportingPBUID;
    std::string SubmittingPBUID;
    std::string SecurityID;
    std::string SecurityIDSource;
    int OwnerType;
    std::string ClearingFirm;
    std::string TransactTime;
    std::string UserInfo;
    std::string ClOrdID;
    std::string ExecID;
    std::string OrigClOrdID;
    std::string Side;
    std::string OrdStatus;
    int CxlRejReason;
    std::string RejectText;
    std::string OrderID;
};

#endif