#include "TransactionReport.h"
#include <spdlog/spdlog.h>

TransactionReportMessage::TransactionReportMessage(Config config)
{
    tr_config = TransactionReportConfig(config);
    standard_header = NULL;
    transaction_report = NULL;
}

TransactionReportMessage::TransactionReportMessage(const TransactionReportConfig& config)
{
    tr_config = config;
    standard_header = NULL;
    transaction_report = NULL;
}


TransactionReportMessage::~TransactionReportMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (transaction_report != NULL)
    {
        delete transaction_report;
        transaction_report = NULL;
    }
}

void TransactionReportMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(tr_config.MsgType);
    standard_header->set_bodylength(tr_config.BodyLength);

    transaction_report = new GatewayProto::Transaction_Report();
    transaction_report->set_partitionno(tr_config.PartitionNo);
    transaction_report->set_reportindex(tr_config.ReportIndex);
    transaction_report->set_applid(tr_config.SubmittingPBUID);
    transaction_report->set_reportingpbuid(tr_config.ReportingPBUID);
    transaction_report->set_submittingpbuid(tr_config.SubmittingPBUID);
    transaction_report->set_securityid(tr_config.SecurityID);
    transaction_report->set_securityidsource(tr_config.SecurityIDSource);
    transaction_report->set_ownertype(tr_config.OwnerType);
    transaction_report->set_clearingfirm(tr_config.ClearingFirm);
    transaction_report->set_transacttime(tr_config.TransactTime);
    transaction_report->set_userinfo(tr_config.UserInfo);
    transaction_report->set_orderid(tr_config.OrderID);
    transaction_report->set_clordid(tr_config.ClOrdID);

    transaction_report->set_execid(tr_config.ExecID);
    transaction_report->set_exectype(tr_config.ExecType);
    transaction_report->set_ordstatus(tr_config.OrdStatus);
    transaction_report->set_lastpx(tr_config.LastPx);
    transaction_report->set_lastqty(tr_config.LastQty);
    transaction_report->set_leavesqty(tr_config.LeavesQty);
    transaction_report->set_cumqty(tr_config.CumQty);
    transaction_report->set_side(tr_config.Side);
    transaction_report->set_accountid(tr_config.AccountID);
    transaction_report->set_branchid(tr_config.BranchID);
    
    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_transaction_report(transaction_report);
}

void TransactionReportMessage::show_config()
{
    return ;
}
void TransactionReportMessage::get_config()
{
    return ;
}