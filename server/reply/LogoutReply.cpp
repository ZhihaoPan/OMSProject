#include "LogoutReply.h"
#include <spdlog/spdlog.h>

LogoutReplyMessage::LogoutReplyMessage(Config config)
{
    standard_header = NULL;
    logout = NULL;
    logout_config = LogoutConfig(config);
}

LogoutReplyMessage::~LogoutReplyMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (logout != NULL)
    {
        delete logout;
        logout = NULL;
    }
}

void LogoutReplyMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(logout_config.MsgType);
    standard_header->set_bodylength(logout_config.BodyLength);
    
    logout = new GatewayProto::Logout();
    logout->set_sessionstatus(logout_config.SessionStatus);
    logout->set_text(logout_config.Text);
    
    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_logout(logout);
}

void LogoutReplyMessage::get_config()
{
    return;
}

void LogoutReplyMessage::show_config()
{
    return;
}