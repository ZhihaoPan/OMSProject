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
    float StopPx = 0.0;
    float MinQty = 0.0;
    int MaxPriceLevels = 0;
    std::string TimeInForce = "test";
    std::string CashMargin = "test";  
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
    LogoutConfig(){}
    LogoutConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::LOGOUT;
    int BodyLength = 2;
    // Logout字段
    int SessionStatus = 1;
    std::string Text = "test";
};

class HeartBeatConfig
{
public:
    HeartBeatConfig(){}
    HeartBeatConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::HEART_BEAT;
    int BodyLength = 2;
};

class ExecutiveReportConfig
{
public:
    ExecutiveReportConfig(){}
    ExecutiveReportConfig(Config config):config(config){}
    Config config;
    int MsgType = MSGTYPE::EXECUTIVE_REPORT;
    int BodyLength = 17;

    int PartitionNo = 999;
    int ReportIndex = 1;
    std::string ApplID = "test";
    std::string ReportingPBUID = "test";
    std::string SubmittingPBUID = "test";
    std::string SecurityID = "test";
    std::string SecurityIDSource = "test";
    int OwnerType = 1;
    std::string ClearingFirm = "test";
    std::string TransactTime = "test";
    std::string UserInfo = "test";
    std::string OrderID = "test";
    std::string ClOrdID = "test";
    std::string OrigClOrdID = "test";
    std::string ExecID = "test";
    std::string ExecType = "test";
    std::string OrdStatus = "test";
    std::string OrdRejReason = "test";
    int LeavesQty = 1;
    int CumQty = 1;
    std::string Side = "test";
    std::string OrdType = "test";
    int OrderQty = 1;
    float Price = 0.0;
    std::string AccountID = "test";
    std::string BranchID = "test";
    std::string OrderRestrictions = "test";
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

    int PartitionNo = 1;
    int ReportIndex = 1;
    std::string ApplID = "test";
    std::string ReportingPBUID = "test";
    std::string SubmittingPBUID = "test";
    std::string SecurityID = "";
    std::string SecurityIDSource = "test";
    int OwnerType = 1;
    std::string ClearingFirm = "test";
    std::string TransactTime = "test";
    std::string UserInfo = "test";
    std::string OrderID = "test";
    std::string ClOrdID = "test";
    std::string ExecID = "test";
    std::string ExecType = "test";
    std::string OrdStatus = "test";
    float LastPx = 0.0;
    int LastQty = 0;
    int LeavesQty = 0;
    int CumQty = 0;
    std::string Side = "test";
    std::string AccountID = "test";
    std::string BranchID = "test";
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

    int PartitionNo = 1;
    int ReportIndex = 1;
    std::string ApplID = "test";
    std::string ReportingPBUID = "test";
    std::string SubmittingPBUID = "test";
    std::string SecurityID = "test";
    std::string SecurityIDSource = "test";
    int OwnerType = 1;
    std::string ClearingFirm = "test";
    std::string TransactTime = "test";
    std::string UserInfo = "test";
    std::string ClOrdID = "test";
    std::string ExecID = "test";
    std::string OrigClOrdID = "test";
    std::string Side = "test";
    std::string OrdStatus = "test";
    int CxlRejReason = 1;
    std::string RejectText = "test";
    std::string OrderID = "test";
};

#endif