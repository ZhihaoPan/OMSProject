#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "../config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class ExecutiveReportMessage : public Message
{
public:
    ExecutiveReportMessage(Config);
    ExecutiveReportMessage(const ExecutiveReportConfig&);
    ~ExecutiveReportMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Executive_Report* executive_report;
    ExecutiveReportConfig er_config;
};
