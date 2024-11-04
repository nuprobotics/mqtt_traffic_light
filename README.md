[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/cU98Cs0g)
# Robotics NUP
## MQTT Traffic Light

### Overview
A Traffic light, managed via mqtt protocol based on ESP32C3 controller and WS2821B LED strip.

###  Problem Statement
We need a managed traffic light for our polygon in smart city lab.

### Objectives
- The system has to be stable.
- The traffic light configuration must be changable with minimal human intervention.


### Features
- Modular structure (interchangable 5,4 and 3 segmented traffic light heads)
- Wifi as a network interface
- Powered by Type-C connector 

### Hardware
ESP32C3
WS2812B 60LED/M  

### Timeline
- Phase 1 (8.11 - 22.11): MQTT brocker running on a server in the lab, esp32 flashes an LED when triggered through MQTT
- Phase 2 (22.11 - 6.12): The phisical traffic light is designed andassembled
- Phase 3 (6.12 - 20.12): The traffic light is stably running, managed through an MQTT broker

### Team
- Vladimir Shtarev
- Innokentii Meleshchenko

### Risks and Mitigation Strategies

At least two!

| Risk                                                                  | Impact          | Probability     | Mitigation Strategy                       |
| --------------------------------------| --------------- | --------------- | ------------------------------|
| No native MQTT support for ESP32C3    | High                    | Medium                      | Port MQTT for ESP32                   |
| No native WS2812B support for ESP32C3 | High                    | Low                         | Port WS2812B driver for ESP32 |
### Success Criteria
- The Traffic light works as an mqtt device
- The traffic light's configuration can be changed in less than 5 minutes
- The traffic light works reliably (no fails during 1 hour continuous test)

### Appendix
The hardware is provided by Vladimir Shtarev

