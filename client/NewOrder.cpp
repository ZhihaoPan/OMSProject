#include "NewOrder.h"
#include <spdlog/spdlog.h>

NewOrderMessage::NewOrderMessage(Config config)
{
    new_order_config = NewOrderConfig(config);
    standard_header = NULL;
    new_order = NULL;
}

NewOrderMessage::~NewOrderMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (new_order != NULL)
    {
        delete new_order;
        new_order = NULL;
    }
}

void NewOrderMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(new_order_config.MsgType);
    standard_header->set_bodylength(new_order_config.BodyLength);

    new_order = new GatewayProto::New_Order();
    new_order->set_applid(new_order_config.ApplID);
    new_order->set_submittingpbuid(new_order_config.SubmittingPBUID);
    new_order->set_securityid(new_order_config.SecurityID);
    new_order->set_securityidsource(new_order_config.SecurityIDSource);
    new_order->set_ownertype(new_order_config.OwnerType);
    new_order->set_clearingfirm(new_order_config.ClearingFirm);
    new_order->set_transacttime(new_order_config.TransactTime);
    new_order->set_userinfo(new_order_config.UserInfo);
    new_order->set_clordid(new_order_config.ClOrdID);
    new_order->set_accountid(new_order_config.AccountID);
    new_order->set_branchid(new_order_config.BranchID);
    new_order->set_orderrestrictions(new_order_config.OrderRestrictions);
    new_order->set_side(new_order_config.Side);
    new_order->set_ordtype(new_order_config.OrdType);
    new_order->set_orderqty(new_order_config.OrderQty);
    new_order->set_price(new_order_config.Price);

    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_new_order(new_order);
}

void NewOrderMessage::show_config()
{
    return ;
}
void NewOrderMessage::get_config()
{
    return ;
}