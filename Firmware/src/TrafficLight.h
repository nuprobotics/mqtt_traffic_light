#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "Config.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

enum class MainLightState {
    RED,
    YELLOW,
    GREEN
};

enum class ArrowLightState {
    OFF,
    GREEN
};

enum class TrafficLightConfiguration {
    MAIN_3_STATE,     // Regular Red/Yellow/Green
    MAIN_PLUS_RIGHT,  // Includes right turn arrow
    MAIN_PLUS_LEFT,   // Includes left turn arrow
    MAIN_PLUS_BOTH    // Includes both left and right turn arrows
};

class TrafficLight {
public:

    explicit TrafficLight(TrafficLightConfiguration configuration = TrafficLightConfiguration::MAIN_3_STATE);
    void changeConfig(TrafficLightConfiguration configuration);
    void setState(MainLightState newMainState = MainLightState::RED, ArrowLightState newRightArrowState = ArrowLightState::OFF, ArrowLightState newLeftArrowState = ArrowLightState::OFF);
    void printState() const;

private:
    MainLightState mainState;
    ArrowLightState leftArrowState;
    ArrowLightState rightArrowState;
    TrafficLightConfiguration config;
    Adafruit_NeoPixel pixels;

    static String mainStateToString(MainLightState st);
    static String arrowStateToString(ArrowLightState st);
    static String configToString(TrafficLightConfiguration c);

    void applyStateMain();
    void applyStateRight();
    void applyStateLeft();
    void applyStateBoth();
};

#endif // TRAFFIC_LIGHT_H
