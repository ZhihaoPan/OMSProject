#pragma once
#include "../protobuf/Gateway.pb.h"

class Message
{
public:
    virtual ~Message(){};

    virtual void get_config() = 0;

    virtual void set_config(GatewayProto::Standard_Message*) = 0;

    virtual void show_config() = 0;

    // virtual void parse_content();
};
