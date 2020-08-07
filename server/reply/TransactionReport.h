#pragma once
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "../config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"


class TransactionReportMessage : public Message
{
public:
    TransactionReportMessage(Config);
    TransactionReportMessage(const TransactionReportConfig&);
    ~TransactionReportMessage();

    virtual void get_config();

    virtual void set_config(GatewayProto::Standard_Message*);

    virtual void show_config();
private:
    GatewayProto::Standard_Header* standard_header;
    GatewayProto::Transaction_Report* transaction_report;
    TransactionReportConfig tr_config;
};
