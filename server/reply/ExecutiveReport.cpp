#include "ExecutiveReport.h"
#include <spdlog/spdlog.h>

ExecutiveReportMessage::ExecutiveReportMessage(Config config)
{
    er_config = ExecutiveReportConfig(config);
    standard_header = NULL;
    executive_report = NULL;
}

ExecutiveReportMessage::ExecutiveReportMessage(const ExecutiveReportConfig& config)
{
    er_config = config;
    standard_header = NULL;
    executive_report = NULL;
}

ExecutiveReportMessage::~ExecutiveReportMessage()
{
    if (standard_header != NULL)
    {
        delete standard_header;
        standard_header = NULL;
    }
    if (executive_report != NULL)
    {
        delete executive_report;
        executive_report = NULL;
    }
}

void ExecutiveReportMessage::set_config(GatewayProto::Standard_Message* standard_message)
{
    standard_header = new GatewayProto::Standard_Header();
    standard_header->set_msgtype(er_config.MsgType);
    standard_header->set_bodylength(er_config.BodyLength);

    executive_report = new GatewayProto::Executive_Report();
    executive_report->set_partitionno(er_config.PartitionNo);
    executive_report->set_reportindex(er_config.ReportIndex);
    executive_report->set_applid(er_config.SubmittingPBUID);
    executive_report->set_reportingpbuid(er_config.ReportingPBUID);
    executive_report->set_submittingpbuid(er_config.SubmittingPBUID);
    executive_report->set_securityid(er_config.SecurityID);
    executive_report->set_securityidsource(er_config.SecurityIDSource);
    executive_report->set_ownertype(er_config.OwnerType);
    executive_report->set_clearingfirm(er_config.ClearingFirm);
    executive_report->set_transacttime(er_config.TransactTime);
    executive_report->set_userinfo(er_config.UserInfo);
    executive_report->set_orderid(er_config.OrderID);
    executive_report->set_clordid(er_config.ClOrdID);
    executive_report->set_origclordid(er_config.OrigClOrdID);
    executive_report->set_execid(er_config.ExecID);
    executive_report->set_exectype(er_config.ExecType);
    executive_report->set_ordstatus(er_config.OrdStatus);
    executive_report->set_ordrejreason(er_config.OrdRejReason);
    executive_report->set_leavesqty(er_config.LeavesQty);
    executive_report->set_cumqty(er_config.CumQty);
    executive_report->set_side(er_config.Side);
    executive_report->set_ordtype(er_config.OrdType);
    executive_report->set_orderqty(er_config.OrderQty);
    executive_report->set_price(er_config.Price);
    executive_report->set_accountid(er_config.AccountID);
    executive_report->set_branchid(er_config.BranchID);
    executive_report->set_orderrestrictions(er_config.OrderRestrictions);

    standard_message->set_allocated_standardheader(standard_header);
    standard_message->set_allocated_executive_report(executive_report);
    spdlog::error("[debug] " + std::to_string(standard_message->executive_report().orderqty()) + standard_message->executive_report().ordstatus());
}

void ExecutiveReportMessage::show_config()
{
    return ;
}
void ExecutiveReportMessage::get_config()
{
    return ;
}