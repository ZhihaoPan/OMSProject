#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "config.h"

const int MAX_EVENTS = 10000;
const int MAX_FDS = 65536;

class Epoll
{
public:
    static epoll_event *event;
    static int epoll_fd;
public:
    static int epoll_init()

};



#endif 