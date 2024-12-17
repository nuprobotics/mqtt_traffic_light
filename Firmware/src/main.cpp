#include <Arduino.h>
#include "TrafficLight.h"
#include "Config.h"

auto traffic_light = TrafficLight(TrafficLightConfiguration::MAIN_PLUS_RIGHT);

void setup() {
    pinMode(LED_PIN, OUTPUT);
}
void loop() {
    traffic_light.setState(MainLightState::RED, ArrowLightState::OFF);
}