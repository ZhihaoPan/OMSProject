#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "../config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class CancelReplyMessage : public Message
{
public:
    CancelReplyMessage(Config);
    CancelReplyMessage(const CancelOrderFailConfig&);
    ~CancelReplyMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Cancel_Order_Fail* cancel_order_fail;
    CancelOrderFailConfig cof_config;
};
