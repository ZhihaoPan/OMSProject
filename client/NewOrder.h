#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class NewOrderMessage : public Message
{
public:
    NewOrderMessage(Config);
    ~NewOrderMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::New_Order* new_order;
    NewOrderConfig new_order_config;
};
