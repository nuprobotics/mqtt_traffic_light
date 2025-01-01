#include "MqttController.h"

MqttController::MqttController(TrafficLight& trafficLight)
    : m_light(trafficLight),
      m_mqttClient(m_wifiClient),
      m_trafficLightId(0),
      m_isActive(false),
      m_initStartTime(0)
{
    // Empty
}

void MqttController::begin() {
    connectWiFi();

    m_mqttClient.setServer(MQTT_ADDRESS, MQTT_PORT);
    // Attach the callback function for incoming MQTT messages
    m_mqttClient.setCallback([this](const char* topic, const byte* payload, const unsigned int length){
        this->onMqttMessage(topic, payload, length);
    });

    // Start the ID negotiation
    m_isActive = false;
    m_initStartTime = millis();
    m_knownIds.clear();
    m_trafficLightId = 0;  // Means we don't have an ID yet

    connectMQTT();
    // Publish a message saying we're NEW and listening for existing IDs
    publishInitRequest();
}

void MqttController::loop() {
    // Maintain MQTT connection
    if (!m_mqttClient.connected()) {
        connectMQTT();
    }
    m_mqttClient.loop();
    // If we aren't active yet, check if we need to finalize ID assignment
    if (!m_isActive) {
        const unsigned long elapsed = millis() - m_initStartTime;
        if (elapsed >= ID_COLLECTION_TIME_MS) {
            checkAndAssignId();
        }
    }
}
// Example: using ESP32's chip-specific ID / MAC
String MqttController::generateMqttClientId() {
    // For ESP32 Arduino, you can get chip ID or MAC
    // In a typical ESP32 (not ESP32-C3), we might do something like:
    //   uint64_t chipid = ESP.getEfuseMac();
    // For ESP32-C3, you can read the MAC similarly:

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);  // read station mac
    char buf[30];
    snprintf(buf, sizeof(buf),
             "TrafficLight_%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return {buf};
}

void MqttController::connectWiFi() {
    Serial.print("Connecting to Wi-Fi: ");
    Serial.print(WIFI_SSID);

    char ssid[] = WIFI_SSID;
    char passwd[] = WIFI_PASSWD;
    WiFi.begin(ssid, passwd);

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(".");
    Serial.println("Wi-Fi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void MqttController::connectMQTT() {
    while (!m_mqttClient.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.print(MQTT_ADDRESS);
        Serial.print("...");
        String clientId = generateMqttClientId();
        Serial.print("ClientID: ");
        Serial.println(clientId);


        #ifndef MQTT_LOGIN
        // Connect without auth
        if (m_mqttClient.connect(clientId.c_str())) {
            Serial.println("connected!");
            // Subscribe to the init topic unconditionally (we need to respond to new lights)
            m_mqttClient.subscribe(MQTT_INIT_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(m_mqttClient.state());
            Serial.println("; trying again in 2 seconds");
            delay(2000);
        }
        #else
        // Connect with auth from Config.h
        if (m_mqttClient.connect(clientId.c_str(), MQTT_LOGIN, MQTT_PASSWORD)) {
            Serial.println("connected!");
            // Subscribe to the init topic unconditionally (we need to respond to new lights)
            m_mqttClient.subscribe(MQTT_INIT_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(m_mqttClient.state());
            Serial.println("; trying again in 2 seconds");
            delay(2000);
        }
        #endif
    }
}

void MqttController::onMqttMessage(const char* topic, const byte* payload, const unsigned int length) {
    const String topicStr(topic);
    String message;
    for (unsigned int i=0; i<length; i++) {
        message += static_cast<char>(payload[i]);
    }

    // If this is the init topic
    if (topicStr == MQTT_INIT_TOPIC) {
        processInitMessage(message);
    }
    // If it's our control topic
    else if (m_isActive) {
        // Our control topic is something like: "trafficlights/control/<id>"
        const String controlTopic = String(MQTT_CONTROL_TOPIC) + String(m_trafficLightId);
        if (topicStr == controlTopic) {
            processControlMessage(message);
        }
    }
}

// We publish a "NEW" message so active lights will respond with their ID
void MqttController::publishInitRequest() {

    // For instance: "NEW"
    // You could embed your MAC address or partial ID for reference
    m_mqttClient.publish(MQTT_INIT_TOPIC, "NEW");
    // Also subscribe to the init topic (to see active lights responding)
    m_mqttClient.subscribe(MQTT_INIT_TOPIC);
    Serial.println("[MQTT] Published init request: NEW");
    Serial.println("Waiting for init responses...");
}

// We respond to another light's "NEW" request with "ACTIVE_ID=<myId>"
void MqttController::publishInitResponse() {
    if (!m_mqttClient.connected() || !m_isActive) return;
    char msg[32];
    snprintf(msg, sizeof(msg), "ACTIVE_ID=%d", m_trafficLightId);
    m_mqttClient.publish(MQTT_INIT_TOPIC, msg);
    Serial.print("[MQTT] Published init response: ACTIVE_ID=");
    Serial.println(m_trafficLightId);
}

void MqttController::processInitMessage(const String& message) {
    // If we see "NEW" from someone else, and we are active, we should respond with our ID
    if (message == "NEW") {
        if (m_isActive) {
            // respond with "ACTIVE_ID=<myId>"
            Serial.println("[MQTT] Received init request, responding...");
            publishInitResponse();
        }
    }
    // If we see something like "ACTIVE_ID=3" from an existing light while we are collecting IDs
    else if (message.startsWith("ACTIVE_ID=")) {
        const String idPart = message.substring(9); // get the substring after 'ACTIVE_ID='
        const int existingId = idPart.toInt();
        Serial.print("[MQTT] Received init response, id announced: ");
        Serial.println(existingId);
        m_knownIds.push_back(existingId);
        Serial.print("[MQTT] Got active ID, added to known ids: ");
        Serial.println(existingId);
    }
}

void MqttController::checkAndAssignId() {
    // We have finished collecting IDs, so pick the largest + 1
    // If no known IDs, we become ID=1
    int maxId = -1;
    for (int id : m_knownIds) {
        Serial.println(id);
        if (id > maxId) maxId = id;
    }
    m_trafficLightId = maxId + 1;
    m_isActive = true;

    // Now subscribe to our control topic
    subscribeToControlTopic();

    Serial.print("[MQTT] Assigned new ID: ");
    Serial.println(m_trafficLightId);
    publishInitResponse();
}

void MqttController::subscribeToControlTopic() {
    if (!m_mqttClient.connected()) return;
    // e.g. "trafficlights/control/3"
    const String controlTopic = String(MQTT_CONTROL_TOPIC) + String(m_trafficLightId);
    m_mqttClient.subscribe(controlTopic.c_str());
    Serial.print("[MQTT] Subscribed to control topic: ");
    Serial.println(controlTopic);
}

void MqttController::processControlMessage(const String& message) {
    // Example messages:
    //  "SET MAIN RED"
    //  "SET MAIN GREEN LEFT GREEN RIGHT OFF"
    //  "SET CONFIG MAIN_PLUS_BOTH"
    // We’ll parse them similarly to earlier examples
    // Then respond with "OK", "Bad request", etc.

    if (message.startsWith("SET CONFIG")) {
        // e.g. "SET CONFIG MAIN_PLUS_BOTH"
        String configPart = message.substring(10);
        configPart.trim();

        if (configPart == "MAIN_3_STATE") {
            m_light.changeConfig(TrafficLightConfiguration::MAIN_3_STATE);
            publishControlResponse("Successfully changed config to MAIN_3_STATE");
        } else if (configPart == "MAIN_PLUS_RIGHT") {
            m_light.changeConfig(TrafficLightConfiguration::MAIN_PLUS_RIGHT);
            publishControlResponse("Successfully changed config to MAIN_PLUS_RIGHT");
        } else if (configPart == "MAIN_PLUS_LEFT") {
            m_light.changeConfig(TrafficLightConfiguration::MAIN_PLUS_LEFT);
            publishControlResponse("Successfully changed config to MAIN_PLUS_LEFT");
        } else if (configPart == "MAIN_PLUS_BOTH") {
            m_light.changeConfig(TrafficLightConfiguration::MAIN_PLUS_BOTH);
            publishControlResponse("Successfully changed config to MAIN_PLUS_BOTH");
        } else {
            publishControlResponse("Bad request: unknown config");
        }
        m_light.printState();
    }
    else if (message.startsWith("SET MAIN")) {
        // e.g. "SET MAIN RED LEFT GREEN RIGHT OFF"
        // parse tokens
        char buffer[128];
        message.toCharArray(buffer, sizeof(buffer));

        auto mainSt = MainLightState::RED;
        auto leftSt = ArrowLightState::OFF;
        auto rightSt = ArrowLightState::OFF;

        const char* token = strtok(buffer, " ");
        while (token != nullptr) {
            String t(token);

            if (t == "MAIN") {
                token = strtok(nullptr, " ");
                if (token) {
                    String val(token);
                    if (val == "RED")    mainSt = MainLightState::RED;
                    if (val == "YELLOW") mainSt = MainLightState::YELLOW;
                    if (val == "GREEN")  mainSt = MainLightState::GREEN;
                }
            }
            else if (t == "LEFT") {
                token = strtok(nullptr, " ");
                if (token) {
                    String val(token);
                    if (val == "OFF")   leftSt  = ArrowLightState::OFF;
                    if (val == "GREEN") leftSt  = ArrowLightState::GREEN;
                }
            }
            else if (t == "RIGHT") {
                token = strtok(nullptr, " ");
                if (token) {
                    String val(token);
                    if (val == "OFF")   rightSt = ArrowLightState::OFF;
                    if (val == "GREEN") rightSt = ArrowLightState::GREEN;
                }
            }
            token = strtok(nullptr, " ");
        }

        m_light.setState(mainSt, rightSt, leftSt);
        m_light.printState();
        publishControlResponse("Successfully changed the state");
    }
    else {
        publishControlResponse("Bad request: unknown command");
    }
}

void MqttController::publishControlResponse(const String& response) {
    if (!m_mqttClient.connected()) return;
    // Publish the response back to the same control topic
    const String controlTopic = String(MQTT_CONTROL_TOPIC) + String(m_trafficLightId);
    m_mqttClient.publish(controlTopic.c_str(), response.c_str());
}
