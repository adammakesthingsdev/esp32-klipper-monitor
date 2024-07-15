#include <stdio.h>
#include "staconnection.h"


static const char *TAG = "MAIN";

static int s_retry_num = 0;

static EventGroupHandle_t s_wifi_event_group;

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        ESP_LOGI(TAG,"Attempting to connect...");
        //esp_wifi_connect();
    } 
    else if (event_base==WIFI_EVENT&& event_id==WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG,"CONNECTED WOOOO");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
        ESP_LOGI(TAG,"Something failed. Reason: %d, SSID: %s, RSSI: %d",event->reason,event->ssid,event->rssi);
        if (s_retry_num < MAX_RETRY) 
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } 
        
        else 
        {
            ESP_LOGE(TAG,"Could not connect, setting fail bit!");
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            ESP_LOGE(TAG,"Fail bit set!");
        }

        ESP_LOGI(TAG,"connect to the AP fail");
    } 

    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG,":DDDD");
    }
}

int wifi_init_sta()
{
    s_wifi_event_group = xEventGroupCreate(); //creates event group and returns handle 

    esp_netif_init(); //checks if tcp/ip stack is init correctly
    esp_event_loop_create_default(); //checks if event loop created 
    esp_netif_create_default_wifi_sta(); //creates esp_netif object and returns pointer

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //creates default wifi config based on config file
    esp_wifi_init(&cfg); //starts wifi task and checks for errors

    esp_event_handler_instance_t instance_any_id; //event handler for any event (?)
    esp_event_handler_instance_t instance_got_ip; //event handler for ip

    esp_event_handler_instance_register(WIFI_EVENT,
                                                ESP_EVENT_ANY_ID,
                                                &event_handler,
                                                NULL,
                                                &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT,
                                                IP_EVENT_STA_GOT_IP,
                                                &event_handler,
                                                NULL,
                                                &instance_got_ip);

    wifi_config_t wifi_config = { //constructor for wifi configuration
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_connect();

    ESP_LOGI(TAG, "wifi_init_sta finished."); //:DDDD


    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);
    ESP_LOGI(TAG,"finished waiting");
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 WIFI_SSID, WIFI_PASS);
        return 1;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 WIFI_SSID, WIFI_PASS);
        return 0;
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        return -1;
    }

    ESP_LOGW(TAG,"Operation completed. Moving back into main program...");
    return -1;

}
