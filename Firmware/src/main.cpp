#include "TrafficLight.h"
#include "MqttController.h"

TrafficLight trafficLight(TrafficLightConfiguration::MAIN_PLUS_BOTH);
MqttController mqttController(trafficLight);

void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(LED_PIN, OUTPUT);
    mqttController.begin();
    Serial.println("Finished setup, starting main loop...");
}

void loop() {
    mqttController.loop();
}
