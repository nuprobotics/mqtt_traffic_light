#include "TrafficLight.h"

// Constructor sets initial state to RED and all arrows OFF
TrafficLight::TrafficLight(const TrafficLightConfiguration configuration)
    : mainState(MainLightState::RED),
      leftArrowState(ArrowLightState::OFF),
      rightArrowState(ArrowLightState::OFF),
      config(configuration),
      pixels(5, LED_PIN, NEO_GRB + NEO_KHZ800)
{
    pixels.begin();
    //pixels.show();
}

String TrafficLight::mainStateToString(const MainLightState st)
{
    switch (st)
    {
    case MainLightState::RED: return "RED";
    case MainLightState::YELLOW: return "YELLOW";
    case MainLightState::GREEN: return "GREEN";
    }
    return "UNKNOWN";
}

String TrafficLight::arrowStateToString(const ArrowLightState st)
{
    switch (st)
    {
    case ArrowLightState::OFF: return "OFF";
    case ArrowLightState::GREEN: return "GREEN";
    }
    return "UNKNOWN";
}

String TrafficLight::configToString(const TrafficLightConfiguration c)
{
    switch (c)
    {
    case TrafficLightConfiguration::MAIN_3_STATE: return "MAIN_3_STATE";
    case TrafficLightConfiguration::MAIN_PLUS_RIGHT: return "MAIN_PLUS_RIGHT";
    case TrafficLightConfiguration::MAIN_PLUS_LEFT: return "MAIN_PLUS_LEFT";
    case TrafficLightConfiguration::MAIN_PLUS_BOTH: return "MAIN_PLUS_BOTH";
    }
    return "UNKNOWN_CONFIG";
}

void TrafficLight::printState() const
{
    Serial.print("| Config: ");
    Serial.println(configToString(config));
    Serial.print("| Main: ");
    Serial.println(mainStateToString(mainState));
    Serial.print("| Left Arrow: ");
    Serial.println(arrowStateToString(leftArrowState));
    Serial.print("| Right Arrow: ");
    Serial.println(arrowStateToString(rightArrowState));
}

void TrafficLight::changeConfig(const TrafficLightConfiguration configuration)
{
    mainState = MainLightState::RED;
    leftArrowState = ArrowLightState::OFF;
    rightArrowState = ArrowLightState::OFF;
    config = configuration;
}

void TrafficLight::setState(const MainLightState newMainState, const ArrowLightState newRightArrowState,
                            const ArrowLightState newLeftArrowState)
{
    mainState = newMainState;
    leftArrowState = newLeftArrowState;
    rightArrowState = newRightArrowState;
    switch (config)
    {
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

void TrafficLight::applyStateMain()
{
    pixels.clear();
    switch (mainState)
    {
    case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(120, 0, 0));
        break;
    case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(75, 75, 0));
        break;
    case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    pixels.show();
}

void TrafficLight::applyStateRight()
{
    pixels.clear();
    switch (mainState)
    {
    case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(120, 0, 0));
        break;
    case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(75, 75, 0));
        break;
    case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    switch (rightArrowState)
    {
    case ArrowLightState::OFF:
        break;
    case ArrowLightState::GREEN:
        pixels.setPixelColor(3, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    pixels.show();
}

void TrafficLight::applyStateLeft()
{
    pixels.clear();
    switch (mainState)
    {
    case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(120, 0, 0));
        break;
    case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(75, 75, 0));
        break;
    case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    switch (leftArrowState)
    {
    case ArrowLightState::OFF:
        break;
    case ArrowLightState::GREEN:
        pixels.setPixelColor(3, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    pixels.show();
}

void TrafficLight::applyStateBoth()
{
    pixels.clear();
    switch (mainState)
    {
    case MainLightState::RED: pixels.setPixelColor(2, Adafruit_NeoPixel::Color(120, 0, 0));
        break;
    case MainLightState::YELLOW: pixels.setPixelColor(1, Adafruit_NeoPixel::Color(75, 75, 0));
        break;
    case MainLightState::GREEN: pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    switch (rightArrowState)
    {
    case ArrowLightState::OFF:
        break;
    case ArrowLightState::GREEN:
        pixels.setPixelColor(3, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    switch (leftArrowState)
    {
    case ArrowLightState::OFF:
        break;
    case ArrowLightState::GREEN:
        pixels.setPixelColor(4, Adafruit_NeoPixel::Color(0, 75, 0));
        break;
    default:
        break;
    }
    pixels.show();
}
