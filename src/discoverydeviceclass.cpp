#include "discoverydeviceclass.h"

void Device::device_receiver(){
    Message msg;
    struct sockaddr_in recv_addr;
    socklen_t recv_addr_len=sizeof(struct sockaddr_in);
    msg.device_id=htonl(this->message.device_id);
    msg.image_port=htons(this->message.image_port);
    msg.control_port=htons(this->message.control_port);
    const char* str[] = {"|", "/", "-", "\\"};
    int count=0;
    bool show=true;
    while(this->running){
        Message ms;

        if(recvfrom(this->sock,&ms,sizeof(ms),0,(struct sockaddr *)&recv_addr,&recv_addr_len)<0){
            std::cout<<"\r"<<str[count]<<"runing.."<<std::flush;
            count=(count+1)%4; 
            show=true;
        }else{
            if(show){
                std::cout<<std::endl<<"id:"<<this->message.device_id<<std::endl;
                std::cout<<"     ip:"<<inet_ntoa(recv_addr.sin_addr)<<" image_port:"<<this->message.image_port<<" control_port:"<<this->message.control_port<<std::endl;
                show=false;
            }
            if(sendto(this->sock,&msg,sizeof(msg),0,(struct sockaddr *)&recv_addr,recv_addr_len)<0){
                std::cout<<"send failed"<<std::endl;
            }
        }
    }
}
void Device::init_udp(){
    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->sock < 0) {
        perror("this->sock error");
    }

    struct timeval tv;
    tv.tv_sec=this->recv_timeout;
    tv.tv_usec=0;
    int opt=1;
    int ret;
    ret= setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ret= setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    ret= setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    memset((void*)&this->device_addr, 0, sizeof(this->device_addr));
    this->device_addr.sin_family = AF_INET;
    this->device_addr.sin_port = htons(this->port);
    this->device_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    bind(this->sock,(struct sockaddr *)&this->device_addr,sizeof(this->device_addr));

    this->running=true;
    device_receiver();
}
Device::Device(in_port_t port,Message &msg){
    this->port=port;
    this->running=false;
    this->interval=1;
    this->recv_timeout=1;
    this->message.device_id=msg.device_id;
    this->message.image_port=msg.image_port;
    this->message.control_port=msg.control_port;

    init_udp();
}
Device::~Device(){
    if (sock != -1) {
        close(sock);
    }
}