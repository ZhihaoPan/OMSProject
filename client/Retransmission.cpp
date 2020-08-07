#include "Retransmission.h"
#include <spdlog/spdlog.h>

RetransmissionMessage::RetransmissionMessage(Config config)
{
    retransmission_config = RetransmissionConfig(config);
    standard_header = NULL;
    retransmission_report = NULL;
}

RetransmissionMessage::~RetransmissionMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (retransmission_report != NULL)
    {
        delete retransmission_report;
        retransmission_report = NULL;
    }
}

void RetransmissionMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(retransmission_config.MsgType);
    standard_header->set_bodylength(retransmission_config.BodyLength);

    retransmission_report = new GatewayProto::Retransmission_Report();
    retransmission_report->set_nopartitions(retransmission_config.NoPartitions);
    for (int i = 0; i < retransmission_config.NoPartitions; i++)
    {
        retransmission_report->add_partitionno(*retransmission_config.PartitionNo);
        retransmission_report->add_reportindex(*retransmission_config.ReportIndex);
        retransmission_config.PartitionNo++;
        retransmission_config.ReportIndex++;
    }
    
    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_retransmission_report(retransmission_report);
}

void RetransmissionMessage::show_config()
{
    return;
}

void RetransmissionMessage::get_config()
{
    return ;
}