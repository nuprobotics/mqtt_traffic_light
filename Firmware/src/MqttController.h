#ifndef MQTT_CONTROLLER_H
#define MQTT_CONTROLLER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "TrafficLight.h"
#include <vector>
#include "Config.h"

class MqttController {
public:
    // Constructor accepts a reference to TrafficLight
    // so we can call setState(), changeConfig(), etc.
    explicit MqttController(TrafficLight& trafficLight);

    // Call in setup() to start everything (connect WiFi, MQTT, etc.)
    void begin();

    // Call in loop() to maintain connection + process messages
    void loop();

private:
    // References
    TrafficLight& m_light;
    WiFiClient    m_wifiClient;
    PubSubClient  m_mqttClient;

    // Our assigned traffic light ID (or 0 if not assigned yet)
    int m_trafficLightId;

    // Whether we’re active or still collecting IDs
    bool m_isActive;

    // List of IDs from active traffic lights
    std::vector<int> m_knownIds;

    // Time when we started collecting IDs
    unsigned long m_initStartTime;

    // Internal methods
    static void connectWiFi();
    static String generateMqttClientId();

    void connectMQTT();
    void onMqttMessage(const char* topic, const byte* payload, unsigned int length);

    void publishInitRequest();
    void publishInitResponse();

    void processInitMessage(const String& message);
    void processControlMessage(const String& message);

    void checkAndAssignId();
    void subscribeToControlTopic();

    // Helper to publish a message on our control topic
    void publishControlResponse(const String& response);
};

#endif // MQTT_CONTROLLER_H
