#include "WiFi.h" // Include the WiFi library for ESP32

void setup() {
    Serial.begin(115200); // Initialize serial communication at 115200 baud rate
    WiFi.mode(WIFI_MODE_STA); // Set the WiFi mode to Station (STA)
    Serial.print("MAC address: "); // Print a message to the serial monitor
    Serial.println(WiFi.macAddress()); // Print the WiFi MAC address to the serial monitor
}

void loop() {
    // The loop function is intentionally left empty since the task is completed in the setup
}
