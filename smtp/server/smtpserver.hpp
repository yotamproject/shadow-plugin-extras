#ifndef STMPSERVER_HPP
#define STMPSERVER_HPP

#include <unistd.h> /* close */
#include <string.h> /* memset */
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <netdb.h>
#include <time.h>
#include <shd-library.h>

#include "myevent.hpp"

#include <map>
#include <string>
#include <queue>
#include <set>

class smtpserver_t
{
public:
    smtpserver_t();
    ~smtpserver_t();

    void start(int argc, char *argv[]);
    void activate(const bool blocking);
    void on_readable();

    const uint32_t instNum_; // monotonic id of this smtpserver obj
private:

    static uint32_t nextInstNum;

    myevent_base* evbase_;
    myevent_socket_t* listenev_;
    int listenfd_;
    std::string docroot_;
};

void smtpserver_start(smtpserver_t* b, int argc, char** argv);
void smtpserver_free(smtpserver_t* b);
void smtpserver_activate(smtpserver_t* b);

#endif /* STMPSERVER_HPP */
