#include "LoginReply.h"
#include <spdlog/spdlog.h>

LogonReplyMessage::LogonReplyMessage(Config config)
{
    standard_header = NULL;
    logon = NULL;
    logon_config = LogonConfig(config);
}

LogonReplyMessage::~LogonReplyMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (logon != NULL)
    {
        delete logon;
        logon = NULL;
    }
}

void LogonReplyMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(logon_config.MsgType);
    standard_header->set_bodylength(logon_config.BodyLength);
    
    logon = new GatewayProto::Logon();
    logon->set_sendercompid(logon_config.SenderCompID);
    logon->set_targetcompid(logon_config.TargetCompID);
    logon->set_heartbtint(logon_config.HeartBtInt);
    logon->set_password(logon_config.Password);
    logon->set_defaultapplverid(logon_config.DefaultApplVerID);
    
    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_logon(logon);
}

void LogonReplyMessage::get_config()
{
    return;
}

void LogonReplyMessage::show_config()
{
    return;
}