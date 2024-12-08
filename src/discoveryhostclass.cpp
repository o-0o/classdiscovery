#include "discoveryhostclass.h"


void capture_broadcast_address(int inet_sock,std::vector<struct in_addr> &addrs);

Host::~Host(){
    if (sock != -1) {
        close(sock);
    }
}

void Host::init_broadcast(){
    this->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->sock < 0) {
        perror("this->sock error");
    }

    struct timeval tv;
    tv.tv_sec=this->recv_timeout;
    tv.tv_usec=0;
    int opt=1;
    setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    memset((void*)&this->broadcast_addr, 0, sizeof(this->broadcast_addr));
    this->broadcast_addr.sin_family = AF_INET;
    this->broadcast_addr.sin_port = htons(this->port);
}
Host::Host(in_port_t port){
    this->port=port;
    this->interval=1;
    this->recv_timeout=1;
}
bool Host::host_broadcast(){
    this->init_broadcast();
    struct sockaddr_in device_addr;
    size_t addr_len = sizeof(device_addr);
    Message msg;

    std::vector<struct in_addr> addrs;
    capture_broadcast_address(this->sock,addrs);

    time_t run_time=time(NULL);

    for(int i=0;i<addrs.size();i++){
        this->broadcast_addr.sin_addr.s_addr = addrs[0].s_addr;
        ssize_t size = sendto(this->sock, &msg, sizeof(msg), 0, (struct sockaddr*)&this->broadcast_addr, addr_len);

        while(1){
            if(time(NULL)-run_time>1)
                break;
            ssize_t count = recvfrom(this->sock, &msg, sizeof(msg), 0, (struct sockaddr*)&device_addr, (socklen_t*)&addr_len);
            if(count>=0){
                msg.host_id=ntohl(msg.host_id);
                msg.control_port=ntohs(msg.control_port);
                msg.image_port=ntohs(msg.image_port);
                discovery_device_list(device_addr,msg);
            }
        }
    }
    close(this->sock);
    if(this->devs.size()>0)
        return true;
    else
        return false;
}
void Host::print_device_list(){
    for(int i=0;i<this->devs.size();i++){
        if(this->devs[i].addresses.size()==0)
            continue;
        std::cout<<this->devs[i].msg.host_id<<std::endl;
        for(int j=0;j<this->devs[i].addresses.size();j++){
            // addrs.push_back(this->devs[i].addresses[j].address);
            std::cout<<"     ip:"<<inet_ntoa(this->devs[i].addresses[j].address.sin_addr)<<" image_port:"<<this->devs[i].msg.image_port<<" control_port"<<this->devs[i].msg.control_port<<std::endl;
        }
    }
}


void Host::discovery_device_list(struct sockaddr_in &device_addr,Message &msg){
    int device_id_find=0;
    int device_id_index=0;
    int address_id_index=0;
    for(int i=0;i<this->devs.size();i++){
        if(this->devs[i].msg.host_id==msg.host_id){
            device_id_index=i;
            device_id_find=1;
            break;
        }
    }
    if(!device_id_find){
        Dev dev;
        dev.msg.host_id=msg.host_id;
        dev.msg.image_port=msg.image_port;
        dev.msg.control_port=msg.control_port;
        this->devs.push_back(dev);


        Address addr;
        addr.address=device_addr;
        addr.scantime=time(NULL);
        this->devs[this->devs.size()-1].addresses.push_back(addr);
        this->devs[this->devs.size()-1].msg.ips.push_back(inet_ntoa(device_addr.sin_addr));
    }else{
        int address_id_find=0;
        for(int j=0;j<this->devs[device_id_index].addresses.size();j++){
            if(this->devs[device_id_index].addresses[j].address.sin_addr.s_addr==device_addr.sin_addr.s_addr){
                address_id_index=j;
                address_id_find=1;
                break;
            }
        }
        if(!address_id_find){
            Address addr;
            addr.address=device_addr;
            addr.scantime=time(NULL);
            this->devs[device_id_index].addresses.push_back(addr);
            this->devs[device_id_index].msg.ips.push_back(inet_ntoa(device_addr.sin_addr));
        }else{
            this->devs[device_id_index].addresses[address_id_index].scantime=time(NULL);
        }
    }
}
void capture_broadcast_address(int inet_sock,std::vector<struct in_addr> &addrs) {
    struct ifconf ifc;
    struct ifreq *ifr;
    char buf[1024];
    int num_ifs;

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(inet_sock, SIOCGIFCONF, &ifc) == -1) {
        perror("ioctl(SIOCGIFCONF)");
        exit(1);
    }
    ifr = ifc.ifc_req;
    num_ifs = ifc.ifc_len / sizeof(struct ifreq);
    for (int i = 0; i < num_ifs; ++i) {
        if(ifr[i].ifr_name[0]=='l'){
            continue;
        }else{
            if (ioctl(inet_sock, SIOCGIFBRDADDR, &ifr[i]) == 0) {
                addrs.push_back(((struct sockaddr_in *)&ifr[i].ifr_addr)->sin_addr);
                //print brocastaddress
                // std::cout<< inet_ntoa(((struct sockaddr_in *)&ifr[i].ifr_addr)->sin_addr)<<std::endl;
            }
        }

    }
}
