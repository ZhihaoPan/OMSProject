#include "HeartBeatReport.h"
#include <spdlog/spdlog.h>

HeartBeatReportMessage::HeartBeatReportMessage(Config config)
{
    hb_config = HeartBeatConfig(config);
    standard_header = NULL;
    heart_beat = NULL;
}

HeartBeatReportMessage::~HeartBeatReportMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (heart_beat != NULL)
    {
        delete heart_beat;
        heart_beat = NULL;
    }
}

void HeartBeatReportMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(hb_config.MsgType);
    standard_header->set_bodylength(hb_config.BodyLength);

    heart_beat = new GatewayProto::Heart_Beat();
    

    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_heart_beat(heart_beat);
}

void HeartBeatReportMessage::show_config()
{
    return ;
}
void HeartBeatReportMessage::get_config()
{
    return ;
}