#include "CancelOrderReply.h"
#include <spdlog/spdlog.h>

CancelReplyMessage::CancelReplyMessage(Config config)
{
    cof_config = CancelOrderFailConfig(config);
    standard_header = NULL;
    cancel_order_fail = NULL;
}

CancelReplyMessage::CancelReplyMessage(const CancelOrderFailConfig& config)
{
    cof_config = config;
    standard_header = NULL;
    cancel_order_fail = NULL;
}

CancelReplyMessage::~CancelReplyMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (cancel_order_fail != NULL)
    {
        delete cancel_order_fail;
        cancel_order_fail = NULL;
    }
}

void CancelReplyMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(cof_config.MsgType);
    standard_header->set_bodylength(cof_config.BodyLength);

    cancel_order_fail = new GatewayProto::Cancel_Order_Fail();
    cancel_order_fail->set_partitionno(cof_config.PartitionNo);
    cancel_order_fail->set_reportindex(cof_config.ReportIndex);
    cancel_order_fail->set_applid(cof_config.SubmittingPBUID);
    cancel_order_fail->set_reportingpbuid(cof_config.ReportingPBUID);
    cancel_order_fail->set_submittingpbuid(cof_config.SubmittingPBUID);
    cancel_order_fail->set_securityid(cof_config.SecurityID);
    cancel_order_fail->set_securityidsource(cof_config.SecurityIDSource);
    cancel_order_fail->set_ownertype(cof_config.OwnerType);
    cancel_order_fail->set_clearingfirm(cof_config.ClearingFirm);
    cancel_order_fail->set_transacttime(cof_config.TransactTime);
    cancel_order_fail->set_userinfo(cof_config.UserInfo);
    cancel_order_fail->set_clordid(cof_config.ClOrdID);
    cancel_order_fail->set_execid(cof_config.ExecID);
    cancel_order_fail->set_origclordid(cof_config.OrigClOrdID);
    cancel_order_fail->set_side(cof_config.Side);
    cancel_order_fail->set_ordstatus(cof_config.OrdStatus);
    cancel_order_fail->set_cxlrejreason(cof_config.CxlRejReason);
    cancel_order_fail->set_rejecttext(cof_config.RejectText);
    cancel_order_fail->set_orderid(cof_config.OrderID);

    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_cancel_order_fail(cancel_order_fail);
}

void CancelReplyMessage::show_config()
{
    return ;
}
void CancelReplyMessage::get_config()
{
    return ;
}