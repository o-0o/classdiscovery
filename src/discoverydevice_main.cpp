#include "discoverydeviceclass.h"
int main(int argc, char const *argv[])
{
    int ret = 0;

    Message msg={.device_id=123,
                 .image_port=8081,
                 .control_port=8080};

    Device device(12345,msg);
    
    // register_callback(&device,notification);
    // discovery_start_broadcast(&device);


    return 0;
}