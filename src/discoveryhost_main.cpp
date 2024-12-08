#include "discoveryhostclass.h"

int main(){
    Host host(12345);
    bool ret=host.host_broadcast();
    if(ret)
        host.print_device_list();
    else
        std::cout<<"don't search device"<<std::endl;

    return 0;
}