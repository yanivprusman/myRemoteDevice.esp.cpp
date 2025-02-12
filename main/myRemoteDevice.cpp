#include "MyRemoteDevice.h"
bool MyRemoteDevice::is_nvs_initialized;
bool MyRemoteDevice::is_netif_initialized;
bool MyRemoteDevice::is_event_loop_initialized;
MyRemoteDevice::MyRemoteDevice(){
    this->is_nvs_initialized=false;
    this->is_netif_initialized=false;
    this->is_event_loop_initialized=false;
}
void MyRemoteDevice::sayHello(){
    printf("Hi %s\n",PROJECT_NAME);
}
void MyRemoteDevice::espNvsFlashInit(){
    char TAG[] = "MyRemoteDeviceEspNvsFlashInit";
    esp_err_t err;
    err = nvs_flash_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS: %s\n", esp_err_to_name(err));
    };
    return;
}
esp_err_t MyRemoteDevice::espNetifInit(){
    char TAG[] = "MyRemoteDeviceNetif";
    esp_err_t err;
    if (!this->is_netif_initialized) {
        err = esp_netif_init();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize netif: %s\n", esp_err_to_name(err));
            return err;
        }
        this->is_netif_initialized = true;
    }
    return ESP_OK;
}
esp_err_t MyRemoteDevice::espEventLoopCreateDefault(){
    char TAG[] = "MyRemoteDeviceEventLoopCreateDefault";
    esp_err_t err;
    if (!this->is_event_loop_initialized) {
        err = esp_event_loop_create_default();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize event loop default: %s\n", esp_err_to_name(err));
            return err;
        }
        this->is_event_loop_initialized = true;
    }
    return ESP_OK;
}
esp_err_t MyRemoteDevice::connectToWifi(){
    char TAG[] = "MyRemoteDeviceConnectToWifi";
    esp_err_t err;
    esp_netif_t* netif = NULL;
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_WIFI_STA();
    wifi_init_config_t init_config;
    wifi_config_t wifi_config;
    this->espNvsFlashInit();
    if ((err = this->espNetifInit()) != ESP_OK) return err;
    if ((err = this->espEventLoopCreateDefault()) != ESP_OK) return err;
    netif = esp_netif_new(&cfg);
    if (netif == NULL) {
        ESP_LOGE(TAG, "Failed to esp_netif_new\n");
        return ESP_FAIL;
    };
    err = esp_netif_attach_wifi_station(netif);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to esp_netif_attach_wifi_station: %s\n", esp_err_to_name(err));
        return err;
    };
    err = esp_wifi_set_default_wifi_sta_handlers();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to esp_wifi_set_default_wifi_sta_handlers: %s\n", esp_err_to_name(err));
        return err;
    }
    init_config = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&init_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize WiFi: %s", esp_err_to_name(err));
        return err;
    }
    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi mode: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "WiFi initialization completed successfully");
    wifi_config.sta = {
        .ssid = SSID,
        .password = WIFIPASSWORD
    };
    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi configuration: %s", esp_err_to_name(err));
        return err;
    }

    // Start WiFi
    err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(err));
        return err;
    }

    // Connect to WiFi
    err = esp_wifi_connect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to WiFi: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "WiFi initialization and connection attempt completed");
    return ESP_OK;
}
void MyRemoteDevice::createWebsocketDeviceServer(){

}

