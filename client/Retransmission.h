#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class RetransmissionMessage : public Message
{
public:
    RetransmissionMessage(Config);
    ~RetransmissionMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Retransmission_Report* retransmission_report;
    RetransmissionConfig retransmission_config;
};
