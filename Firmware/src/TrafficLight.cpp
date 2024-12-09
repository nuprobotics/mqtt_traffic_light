#include "TrafficLight.h"
#include "Config.h"
#include <iostream>

// Constructor sets initial state to RED and all arrows OFF
TrafficLight::TrafficLight(TrafficLightConfiguration configuration)
    : mainState(MainLightState::RED),
      leftArrowState(ArrowLightState::OFF),
      rightArrowState(ArrowLightState::OFF),
      config(configuration),
      pixels(5, LED_PIN, NEO_GRB + NEO_KHZ800)
{
    pixels.begin();
}

std::string TrafficLight::mainStateToString(MainLightState st) {
    switch (st) {
        case MainLightState::RED:    return "RED";
        case MainLightState::YELLOW: return "YELLOW";
        case MainLightState::GREEN:  return "GREEN";
    }
    return "UNKNOWN";
}

std::string TrafficLight::arrowStateToString(ArrowLightState st) {
    switch (st) {
        case ArrowLightState::OFF:   return "OFF";
        case ArrowLightState::GREEN: return "GREEN";
    }
    return "UNKNOWN";
}

std::string TrafficLight::configToString(TrafficLightConfiguration c) {
    switch(c) {
        case TrafficLightConfiguration::MAIN_3_STATE:     return "MAIN_3_STATE";
        case TrafficLightConfiguration::MAIN_PLUS_RIGHT:  return "MAIN_PLUS_RIGHT";
        case TrafficLightConfiguration::MAIN_PLUS_LEFT:   return "MAIN_PLUS_LEFT";
        case TrafficLightConfiguration::MAIN_PLUS_BOTH:   return "MAIN_PLUS_BOTH";
    }
    return "UNKNOWN_CONFIG";
}

void TrafficLight::printState() {
    std::cout << "Config: " << configToString(config)
              << " | Main: " << mainStateToString(mainState)
              << " | Left Arrow: " << arrowStateToString(leftArrowState)
              << " | Right Arrow: " << arrowStateToString(rightArrowState)
              << std::endl;
}

void TrafficLight::changeConfig(TrafficLightConfiguration configuration){
    mainState = MainLightState::RED;
    leftArrowState = ArrowLightState::OFF;
    rightArrowState = ArrowLightState::OFF;
    config = configuration;
}

void TrafficLight::setState(MainLightState newMainState, ArrowLightState newRightArrowState, ArrowLightState newLeftArrowState) {
    mainState = newMainState;
    leftArrowState = newLeftArrowState;
    rightArrowState = newRightArrowState;
    switch(config){
        case TrafficLightConfiguration::MAIN_3_STATE: applyStateMain();
            break;
        case TrafficLightConfiguration::MAIN_PLUS_RIGHT: applyStateRight();
            break;
        case TrafficLightConfiguration::MAIN_PLUS_LEFT: applyStateLeft();
            break;
        case TrafficLightConfiguration::MAIN_PLUS_BOTH: applyStateBoth();
            break;
        default:
            break;
    }
}
void TrafficLight::applyStateMain() {
    pixels.clear();
    switch(mainState) {
        case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(50, 0, 0));
            break;
        case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(50, 50, 0));
            break;
        case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 50, 0));
            break;
        default:
            break;
    }
    pixels.show();
}

void TrafficLight::applyStateRight() {
    pixels.clear();
    switch(mainState) {
        case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(50, 0, 0));
        break;
        case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(50, 50, 0));
        break;
        case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 50, 0));
        break;
        default:
            break;
    }
    switch(rightArrowState) {
        case ArrowLightState::OFF:
            break;
        case ArrowLightState::GREEN:
            pixels.setPixelColor(3, Adafruit_NeoPixel::Color(0, 50, 0));
            break;
        default:
            break;
    }
    pixels.show();
}

void TrafficLight::applyStateLeft() {
    pixels.clear();
    switch(mainState) {
        case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(50, 0, 0));
        break;
        case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(50, 50, 0));
        break;
        case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 50, 0));
        break;
        default:
            break;
    }
    switch(leftArrowState) {
        case ArrowLightState::OFF:
            break;
        case ArrowLightState::GREEN:
            pixels.setPixelColor(3, Adafruit_NeoPixel::Color(0, 50, 0));
        break;
        default:
            break;
    }
    pixels.show();
}

void TrafficLight::applyStateBoth() {
    pixels.clear();
    switch(mainState) {
        case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(75, 0, 0));
        break;
        case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(50, 50, 0));
        break;
        case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 50, 0));
        break;
        default:
            break;
    }
    switch(rightArrowState) {
        case ArrowLightState::OFF:
            break;
        case ArrowLightState::GREEN:
            pixels.setPixelColor(3, Adafruit_NeoPixel::Color(0, 50, 0));
        break;
        default:
            break;
    }
    switch(leftArrowState) {
        case ArrowLightState::OFF:
            break;
        case ArrowLightState::GREEN:
            pixels.setPixelColor(4, Adafruit_NeoPixel::Color(0, 50, 0));
        break;
        default:
            break;
    }
    pixels.show();
}