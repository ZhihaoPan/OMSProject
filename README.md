实习Proj

实习要求：
1)题目请独立完成，可以上网查资料，但是禁止请教他人或者直接抄网上大段源码
2)实习题目可以自主决定采用java/c++/python语言实现(题目限定范围内)。
3)默认请使用语言本身的基础库实现，允许使用基础开源库(不允许直接使用课题对应的解决方案开源项目)，包括但不限于：
  boost/poco/ACE/protobuf/ZeroMQ
  libxml2/tinyxml/log4cxx
  redis/mongodb/memcached/mysql/sqllite
4)要求设计思路清晰、编码规范
5)对于题目要求不清楚的可以咨询，但是不可以就实现方案请教。


题目1(仅限C++)：
    设计一个简单的类似于shell的语法解释器，要求：
    1）变量仅需要支持整数，且能够进行整数的四则混合运算及括号
    2）能够支持多语句解析，语句以分号结尾（换行不作为语句分割）
    3）支持if流程控制语句( if else endif，注意没有代码块{}，使用 endif 结束)
    4）支持 > < == 逻辑运算
    5）print 语句是终端输出语句，用于输出变量(或运算结果)
    5）提供shell命令行接口能够读入终端输入，语句解析完毕后计算结果
    6）示例(以下语句能够输出 33)：
    
    a = 1*(3+
    4);
    b =5 + 3*a;
    if a < 0 
      print a;
    else
      print a+b;
    endif


题目2(限C++/Java)：
    利用Thrift开发一个服务器监控工具，要求有agent/server端，提供如下功能:
    1)agent端部署在每一台被监视的主机上，能够采集主机资源使用情况(CPU/磁盘空间/内存等资源)和应用进程状态
    2)server端集中收集每个agent端采集的数据，并能够保存历史数据
	3)能够通过Web界面展示主机资源的使用情况统计(能够按天/小时/分钟为单位统计并绘图)
    4)server端对被监控主机能够分组管理，备注机器别名，能够支持按机器IP、别名等多条件搜索
    5)Server段能够通过界面操作向指定的组发送某个指令（指令内容可配置），执行对应的脚本
    6)能够设置一些简单的预警策略，当资源使用条件达到预警阀值之后能够在界面提醒或发送邮件预警
	  
  
题目3(java/c++/python): 
   实现一个模拟的深交所Binary行情网关
要求：
  1）参照深交所Binary行情网关MDGW的应用层协议
  2）需要实现登录、重传、心跳等机制
  3）应用层只需要实现现货集中竞价交易快照行情、现货集中竞价交易逐笔行情，其中逐笔行情需要能够支持重传
  4）支持多个推送与多个订阅服务同时运行
  5）应用层通讯协议要基于protobuf灵活定制，修改或新增一个消息不应该重复手工编码
  6）订阅服务需要能够支持断点重传（即故障之后重新接入能够从上次接收消息的断开处继续收消息）
  
题目4(java/c++/python): 
   实现一个模拟的深交所Binary交易网关
要求：
  1）参照深交所Binary交易网关TGW的应用层协议
  2）需要实现登录、重传、心跳等机制
  3）应用层只需要实现现货集中竞价交易业务的委托、确认、成交、撤单
  4）支持多种委托方式及成交模式（全部成交、部分成交、拒单等）
  5）应用层通讯协议要基于protobuf灵活定制，修改或新增一个消息不应该重复手工编码