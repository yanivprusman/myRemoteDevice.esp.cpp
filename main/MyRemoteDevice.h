#include <stdio.h>
#include "project_config.h"
#include "env.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_websocket_client.h"
#include "cJSON.h"
#include "driver/gpio.h"
#include <ArduinoJson.h>
#include "../flatBuffers/mrd/mrd_generated.h"
#define MRDE "MRDE"
class MyRemoteDevice{
    private:
    static bool isNvsInitialized;
    static bool isNetifInitialized;
    static bool isEventLoopInitialized;
    static nvs_handle_t nvsHandle;
    static esp_netif_t* netif;
    static EventGroupHandle_t sWifiEventGroup;
    static const int WIFICONNECTEDBIT = BIT0;
    static const int WIFIFAILBIT = BIT1;
    public:
    static esp_websocket_client_handle_t client;
    MyRemoteDevice();
    private:
    void espNvsFlashInit();
    esp_err_t espNetifInit();
    esp_err_t espEventLoopCreateDefault();
    esp_err_t connectToWifiCleanup(esp_err_t err);
    static void wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);
    static void ipEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);
    public:
    void sayHello();
    esp_err_t connectToWifi();
    void createWebsocketDeviceServer();
    static void websocketEventHandler1(void *handler_args, esp_event_base_t base, int32_t eventId, void *eventData);
    static void websocketEventHandler(void *handler_args, esp_event_base_t base, int32_t eventId, void *eventData);
    static void websocketEventHandlerHandleData(void *handler_args, esp_event_base_t base, void *eventData);
    void setPinDirection(gpio_num_t pin,  gpio_mode_t direction);
    void setPinLevel(gpio_num_t pin,  uint32_t level);
    uint32_t getPinLevel(gpio_num_t pin);
};

