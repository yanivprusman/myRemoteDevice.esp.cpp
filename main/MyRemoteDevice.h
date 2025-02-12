#include <stdio.h>
#include "project_config.h"
#include "env.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
class MyRemoteDevice{
    public:
    static bool is_nvs_initialized;
    static bool is_netif_initialized;
    static bool is_event_loop_initialized;
    static nvs_handle_t nvsHandle;
    MyRemoteDevice();
    void sayHello();
    void espNvsFlashInit();
    esp_err_t espNetifInit();
    esp_err_t espEventLoopCreateDefault();
    esp_err_t connectToWifi();
    void createWebsocketDeviceServer();
};

