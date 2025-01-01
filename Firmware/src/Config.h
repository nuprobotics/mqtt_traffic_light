#ifndef CONFIG_H
#define CONFIG_H
#define LED_PIN 4
#define WIFI_SSID "NUP-WiFi"
#define WIFI_PASSWD "Neapolis2010!"
#define MQTT_ADDRESS "labserver.local"
#define MQTT_PORT 1883
#define MQTT_INIT_TOPIC "/trafficlights/init"
#define MQTT_CONTROL_TOPIC "/trafficlights/control"
#define ID_COLLECTION_TIME_MS 2000
//TODO: Add mqtt credentials and their use in the controller

#endif //CONFIG_H
