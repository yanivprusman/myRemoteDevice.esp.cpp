#include "MyRemoteDevice.h"
bool MyRemoteDevice::isNvsInitialized;
bool MyRemoteDevice::isNetifInitialized;
bool MyRemoteDevice::isEventLoopInitialized;
esp_websocket_client_handle_t MyRemoteDevice::client;
esp_netif_t* MyRemoteDevice::netif;
EventGroupHandle_t MyRemoteDevice::sWifiEventGroup;
const int MyRemoteDevice::WIFICONNECTEDBIT;
const int MyRemoteDevice::WIFIFAILBIT;
MyRemoteDevice::MyRemoteDevice(){
    this->isNvsInitialized=false;
    this->isNetifInitialized=false;
    this->isEventLoopInitialized=false;
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
    if (!this->isNetifInitialized) {
        err = esp_netif_init();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize netif: %s\n", esp_err_to_name(err));
            return err;
        }
        this->isNetifInitialized = true;
    }
    return ESP_OK;
}
esp_err_t MyRemoteDevice::espEventLoopCreateDefault(){
    char TAG[] = "MyRemoteDeviceEventLoopCreateDefault";
    esp_err_t err;
    if (!this->isEventLoopInitialized) {
        err = esp_event_loop_create_default();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize event loop default: %s\n", esp_err_to_name(err));
            return err;
        }
        this->isEventLoopInitialized = true;
    }
    return ESP_OK;
}
esp_err_t MyRemoteDevice::connectToWifi() {
    const char TAG[] = "MyRemoteDeviceConnectToWifi";
    esp_err_t err;
    this->netif = NULL;
    const int MAX_RETRY_ATTEMPTS = 5;
    const int RETRY_DELAY_MS = 5000;
    int retry_count = 0;
    bool connected = false;
    this->espNvsFlashInit();
    if ((err = this->espNetifInit()) != ESP_OK) return err;
    if ((err = this->espEventLoopCreateDefault()) != ESP_OK) return err;
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_WIFI_STA();
    this->netif = esp_netif_new(&cfg);
    if (this->netif == NULL) {
        ESP_LOGE(TAG, "Failed to create network interface");
        return ESP_FAIL;
    }
    if ((err = esp_netif_attach_wifi_station(this->netif)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to attach WiFi station: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    if ((err = esp_wifi_set_default_wifi_sta_handlers()) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi handlers: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    if ((err = esp_wifi_init(&init_config)) != ESP_OK) {
        ESP_LOGE(TAG, "WiFi initialization failed: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    if ((err = esp_event_handler_instance_register(
        WIFI_EVENT, 
        ESP_EVENT_ANY_ID,
        &this->wifiEventHandler,
        NULL,
        NULL)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register WiFi event handler: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    if ((err = esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &ipEventHandler,
        NULL,
        NULL)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register IP event handler: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    if ((err = esp_wifi_set_mode(WIFI_MODE_STA)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi mode: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    wifi_config_t wifi_config = {};
    memset(&wifi_config, 0, sizeof(wifi_config_t));
    strncpy((char*)wifi_config.sta.ssid, SSID, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char*)wifi_config.sta.password, WIFIPASSWORD, sizeof(wifi_config.sta.password) - 1);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;
    wifi_config.sta.failure_retry_cnt = MAX_RETRY_ATTEMPTS;
    if ((err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi configuration: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    if ((err = esp_wifi_start()) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(err));
        return(this->connectToWifiCleanup(err));
    }
    ESP_LOGI(TAG, "WiFi started, attempting to connect...");
    sWifiEventGroup = xEventGroupCreate();
    if (sWifiEventGroup == NULL) {
        ESP_LOGE(TAG, "Failed to create event group");
        return ESP_FAIL;
    }
    while (retry_count < MAX_RETRY_ATTEMPTS && !connected) {
        err = esp_wifi_connect();
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "Connection attempt %d failed: %s", retry_count + 1, esp_err_to_name(err));
            retry_count++;
            if (retry_count < MAX_RETRY_ATTEMPTS) {
                vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS));
                continue;
            }
            return(this->connectToWifiCleanup(err));
        }
        EventBits_t bits = xEventGroupWaitBits(
            sWifiEventGroup,
            WIFICONNECTEDBIT | WIFIFAILBIT,
            pdFALSE,
            pdFALSE,
            pdMS_TO_TICKS(10000)
        );
        if (bits & WIFICONNECTEDBIT) {
            connected = true;
            ESP_LOGI(TAG, "Connected to SSID: %s", SSID);
        } else if (bits & WIFIFAILBIT) {
            ESP_LOGW(TAG, "Failed to connect to SSID: %s", SSID);
            retry_count++;
            if (retry_count < MAX_RETRY_ATTEMPTS) {
                vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS));
                continue;
            }
            err = ESP_FAIL;
            return(this->connectToWifiCleanup(err));
        } else {
            ESP_LOGE(TAG, "Connection timeout");
            retry_count++;
            if (retry_count < MAX_RETRY_ATTEMPTS) {
                vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS));
                continue;
            }
            err = ESP_ERR_TIMEOUT;
            return(this->connectToWifiCleanup(err));
        }
    }
    if (!connected) {
        ESP_LOGE(TAG, "Failed to connect after %d attempts", MAX_RETRY_ATTEMPTS);
        err = ESP_FAIL;
        return(this->connectToWifiCleanup(err));
    }
    return ESP_OK;
}
esp_err_t MyRemoteDevice::connectToWifiCleanup(esp_err_t err){
    if (this->netif != NULL) {
        esp_netif_destroy(this->netif);
    }
    return err;
}
void MyRemoteDevice::wifiEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    const char*TAG= "MyRemoteDeviceWifHandler";
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "WiFi station started");
            break;
        case WIFI_EVENT_STA_DISCONNECTED: {
            wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
            ESP_LOGW(TAG, "Disconnected from WiFi, reason: %d", event->reason);
            xEventGroupSetBits(sWifiEventGroup, WIFIFAILBIT);
            break;
        }
        default:
            break;
    }
}

void MyRemoteDevice::ipEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    const char*TAG= "MyRemoteDeviceIpEventHandler";
    if (event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP address: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(sWifiEventGroup, WIFICONNECTEDBIT);
    }
}

void MyRemoteDevice::createWebsocketDeviceServer(){
    char wsUri[] = "wss://myRemoteDevice.ya-niv.com:8443/?type=device&deviceIdentifier=" DEVICE_IDENTIFIER "&passWord=" DEVICE_PASS_WORD;
    // char homePage[] = "hi from device 1 what next? <br>will do button";
    const char *rootCa = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
"-----END CERTIFICATE-----\n";
    esp_websocket_client_config_t websocketCfg = {
        .uri = wsUri,
        .disable_auto_reconnect = false,
        .buffer_size = 4096,
        .cert_pem = rootCa,
        .transport = WEBSOCKET_TRANSPORT_OVER_SSL,
        .skip_cert_common_name_check = true,
    };

    this->client = esp_websocket_client_init(&websocketCfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocketEventHandler, (void*)this->client);

    esp_websocket_client_start(this->client);



}
void MyRemoteDevice::websocketEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    
    char TAG[] = "MyRemoteDeviceWebsocketEventHandler";
    char homePage[] = "hi from device 1 what next? <br>will do button";
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    esp_websocket_client_handle_t client = (esp_websocket_client_handle_t)handler_args;
    switch (event_id) {
        case WEBSOCKET_EVENT_CONNECTED:
            ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
            break;
        case WEBSOCKET_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "WEBSOCKET_EVENT_DISCONNECTED");
            break;
        case WEBSOCKET_EVENT_DATA: {
            char *payload = (char *)data->data_ptr;
            int len = data->data_len;
            char jsonStr[len + 1];
            memcpy(jsonStr, payload, len);
            jsonStr[len] = '\0';
            cJSON *json = cJSON_Parse(jsonStr);
            if (json) {
                cJSON *resourceId = cJSON_GetObjectItem(json, "resourceId");
                cJSON *msg = cJSON_GetObjectItem(json, "msg");
                if (cJSON_IsNumber(resourceId) && cJSON_IsString(msg)) {
                    cJSON *msgJson = cJSON_Parse(msg->valuestring);
                    if (msgJson) {
                        cJSON *action = cJSON_GetObjectItem(msgJson, "action");
                        if (cJSON_IsString(action) && strcmp(action->valuestring, "getPage") == 0) {
                            printf("%s\n",homePage);
                            cJSON *response = cJSON_CreateObject();
                            cJSON *responseMsg = cJSON_CreateObject();
                            cJSON_AddNumberToObject(responseMsg, "resourceId", resourceId->valueint);
                            cJSON_AddStringToObject(responseMsg, "action", "respondingToGetPage");
                            cJSON_AddStringToObject(responseMsg, "page", homePage);
                            cJSON_AddItemToObject(response, "msg", responseMsg);
                            char *responseStr = cJSON_PrintUnformatted(response);
                            esp_websocket_client_send_text(client, responseStr, strlen(responseStr), portMAX_DELAY);
                            cJSON_Delete(response);
                            free(responseStr);
                        }
                        cJSON_Delete(msgJson);
                    }
                }
                cJSON_Delete(json);
            }
            break;
        }
           
        case WEBSOCKET_EVENT_ERROR:
            ESP_LOGI(TAG, "WEBSOCKET_EVENT_ERROR");
            break;
    }
}




