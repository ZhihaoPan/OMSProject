#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class LogonMessage : public Message
{
public:
    LogonMessage(Config);
    ~LogonMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Logon* logon;
    LogonConfig logon_config;
};
