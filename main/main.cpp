#include <stdio.h>
#include "MyRemoteDevice.h"

extern "C" {
    void app_main(void);
}
void app_main(void){
    MyRemoteDevice mrd;
    mrd.sayHello();
    mrd.connectToWifi();
    mrd.createWebsocketDeviceServer();
    for(int x=0;x<6;x++){
        
    }
    mrd.setPinDirection((gpio_num_t)1,GPIO_MODE_OUTPUT);
}
// JsonDocument doc;
// doc["sensor"] = "temperature";
// doc["value"] = 25.6;
// char jsonBuffer[256];
// serializeJson(doc, jsonBuffer);
// printf("JSON: %s\n", jsonBuffer);
void app_main1(void)
{

}