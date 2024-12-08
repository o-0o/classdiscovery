#ifndef  SENDER_H
#define  SENDER_H

#include <sys/socket.h>
#include <memory.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <iostream>
#include <sys/ioctl.h>
#include <net/if.h>

#include <vector>

#define PORT 12345
#define ALIVE_TIME 5

#define NEW_HOST 1
#define NEW_ADDRESS 2
#define HOST_NOT_ALIVE 3
#define ADDRESS_NOT_ALIVE 4

typedef struct {
    uint64_t device_id;
    std::vector<char *> ips;
    in_port_t image_port;
    in_port_t control_port;
} Message;

typedef struct {
    time_t scantime;
    struct sockaddr_in address;
} Address;

typedef struct dev {
    uint64_t id;
    std::vector<Address> addresses;
} Dev;


class Device{
private:
    int sock;
    in_port_t port;
    time_t sender_timer;
    bool running;
    time_t interval=1;
    time_t recv_timeout=1;
    Message message;
    struct sockaddr_in device_addr;
    std::vector<Dev> devs;
    pthread_t receiver_thread;
    void init_udp();
    void device_receiver();
public:
    Device(in_port_t port,Message &msg);
    ~Device();

};


#endif