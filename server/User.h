#pragma once
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <mutex>
#include <spdlog/spdlog.h>
#include "config.h"
#include "Message.h"
#include "../protobuf/Gateway.pb.h"

class UserInfo
{
public:
    UserInfo(std::string userid):m_userid(userid){}

    int buy_stock(std::string , int, int);

    int sell_stock(std::string , int, int);

private:
    std::set<std::string> m_stockid_set;
    std::map<std::string, int> m_stocks;
    std::string m_userid;
    std::mutex m_mutex;

};