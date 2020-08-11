#include <User.h>

UserInfo::UserInfo(std::string user_id)
{
    // 载入上一个交易日的用户数据
}

int UserInfo::buy_stock(std::string stock_id, int price, int num)
{
    if (m_stockid_set.find(stock_id) == m_stockid_set.end())
    {
        // 不存在该股票信息
        spdlog::info("不存在该股票信息.");
        return -1;
    }

    std::lock_guard<std::mutex> lck(m_mutex);
    spdlog::info(m_userid + " 买入股票数量：" + std::to_string(num));
    if (m_stocks.find(stock_id) != m_stocks.end())
    {
        m_stocks[stock_id] += num;
    }
    else
    {
        m_stocks[stock_id] = num;
    }
    return 0;
}

int UserInfo::sell_stock(std::string stock_id, int price, int num)
{
    if (m_stockid_set.find(stock_id) == m_stockid_set.end())
    {
        // 不存在该股票信息
        spdlog::info("不存在该股票信息.");
        return -1;
    }

    std::lock_guard<std::mutex> lck(m_mutex);

    if (m_stocks.find(stock_id) != m_stocks.end() && m_stocks[stock_id] < num)
    {
        spdlog::info(m_userid + " 卖出股票数量：" + std::to_string(num));
        m_stocks[stock_id] -= num;
    }
    else
    {
        spdlog::info("卖出股票超出当前数字");
        return 1;
    }
    return 0;
}
