#include <esp_now.h> // Include ESP-NOW library for ESP32
#include <WiFi.h> // Include WiFi library for ESP32

// Define a structure to hold the incoming message
typedef struct struct_message {
 char text[32]; // Can hold a string up to 31 characters + null terminator
} struct_message;

// Create an instance of the message structure to store the received message
struct_message message;

// Callback function called when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
 memcpy(&message, incomingData, sizeof(message)); // Copy the incoming data into the message structure
 Serial.print("Received message: ");
 Serial.println(message.text); // Print the received message to the serial monitor
}

void setup() {
 Serial.begin(115200); // Initialize serial communication at 115200 baud rate
 WiFi.mode(WIFI_STA); // Set WiFi mode to station (STA)
 
 // Initialize ESP-NOW. If it fails, print an error message and return from setup
 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW");
   return;
 }
 
 // Register callback function for data reception
 esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
 // The loop function is intentionally left empty
 // All ESP-NOW communication is handled in the OnDataRecv callback
}
