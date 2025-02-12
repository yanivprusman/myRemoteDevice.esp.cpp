#include <stdio.h>
#include "MyRemoteDevice.h"

extern "C" {
    void app_main(void);
}

void app_main(void)
{
    MyRemoteDevice mrd;
    mrd.sayHello();
    mrd.connectToWifi();
}