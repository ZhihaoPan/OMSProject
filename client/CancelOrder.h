#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class CancelOrderMessage : public Message
{
public:
    CancelOrderMessage(Config);
    ~CancelOrderMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Cancel_Order* cancel_order;
    CancelOrderConfig cancel_order_config;
};
