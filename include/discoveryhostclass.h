#ifndef DISCOVERYHOSTCLASS_H

#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/time.h>
#include <vector>
#include <net/if.h>
#include <memory.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <sys/ioctl.h>

typedef struct Message {
    uint64_t host_id;
    std::vector<char *> ips;
    in_port_t image_port;
    in_port_t control_port;
} Message;

typedef struct {
    time_t scantime;
    struct sockaddr_in address;
} Address;

typedef struct {
    Message msg; 
    std::vector<Address> addresses;
} Dev;


class Host{
private:
    int sock;
    in_port_t port;
    time_t sender_timer;
    time_t interval=1;
    time_t recv_timeout=1;
    Message message;
    struct sockaddr_in broadcast_addr;
    std::vector<Dev> devs;

    void discovery_device_list(struct sockaddr_in &device_addr,Message &msg);
    void init_broadcast();

public:
    Host(in_port_t port);
    bool host_broadcast();
    void print_device_list();
    ~Host();
};
#endif