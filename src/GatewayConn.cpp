#include <GatewayConn.h>

int GatewayConn::m_epollfd = -1;
int GatewayConn::m_client_count = 0;

int GatewayConn::init()
{
    m_sockfd = -1;
    m_ip = "";
    m_message = "";
    m_report_index = 0;
    m_read_index = 0;
    m_write_index = 0;

    memset(read_buff, '\0', READPACKSIZE);
    memset(write_buff, '\0', WRITEPACKSIZE);
    return 0;
}

int GatewayConn::init(int sockfd, const struct sockaddr_in& addr)
{
    m_sockfd = sockfd;
    m_address = addr;
    m_ip = inet_ntoa(m_address.sin_addr);
    // setsockopt
    int reuse = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    // 设置超时时间
    struct timeval timeout;
    timeout.tv_sec = 6;
    timeout.tv_usec = 0;
    setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout , sizeof(timeout));
    
    init();
    // TODO: 静态变量是不是线程安全的
    m_client_count++;
    return 0;
}

int GatewayConn::close_conn()
{
    if (m_sockfd != -1)
    {
        removefd(m_sockfd, m_epollfd);
        // TODO: 考虑线程安全
        m_client_count--;
        m_sockfd = -1;
    }
}

int GatewayConn::Task()
{
    int bytes_read;
    while (true)
    {
        bytes_read = recv(m_sockfd, read_buff + m_read_index, READPACKSIZE, 0);
        if (bytes_read > 0)
        {
            m_message += std::string(read_buff);
            spdlog::info("[GATEWAYCONN] [sockfd: " + std::to_string(m_sockfd) + "] recv msg: " + m_message);
            m_read_index = m_read_index + bytes_read;
        }
        else if (bytes_read == 0)
        {
            return 1;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
        }
    }
    return 0;
}