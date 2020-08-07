#include "CancelOrder.h"
#include <spdlog/spdlog.h>

CancelOrderMessage::CancelOrderMessage(Config config)
{
    cancel_order_config = CancelOrderConfig(config);
    standard_header = NULL;
    cancel_order = NULL;
}

CancelOrderMessage::~CancelOrderMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (cancel_order != NULL)
    {
        delete cancel_order;
        cancel_order = NULL;
    }
}

void CancelOrderMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(cancel_order_config.MsgType);
    standard_header->set_bodylength(cancel_order_config.BodyLength);

    cancel_order = new GatewayProto::Cancel_Order();
    cancel_order->set_applid(cancel_order_config.ApplID);
    cancel_order->set_submittingpbuid(cancel_order_config.SubmittingPBUID);
    cancel_order->set_securityid(cancel_order_config.SecurityID);
    cancel_order->set_securityidsource(cancel_order_config.SecurityIDSource);
    cancel_order->set_ownertype(cancel_order_config.OwnerType);
    cancel_order->set_clearingfirm(cancel_order_config.ClearingFirm);
    cancel_order->set_transacttime(cancel_order_config.TransactTime);
    cancel_order->set_userinfo(cancel_order_config.UserInfo);
    cancel_order->set_clordid(cancel_order_config.ClOrdID);
    cancel_order->set_origclordid(cancel_order_config.OrigClOrdID);
    cancel_order->set_side(cancel_order_config.Side);
    cancel_order->set_orderid(cancel_order_config.OrderID);
    cancel_order->set_orderqty(cancel_order_config.OrderQty);

    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_cancel_order(cancel_order);
}

void CancelOrderMessage::show_config()
{
    return;
}
void CancelOrderMessage::get_config()
{
    return;
}