#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "../config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class HeartBeatReportMessage : public Message
{
public:
    HeartBeatReportMessage(Config);
    ~HeartBeatReportMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Heart_Beat* heart_beat;
    HeartBeatConfig hb_config;
};
