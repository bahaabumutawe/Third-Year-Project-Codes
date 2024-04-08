#include <esp_now.h> // Include ESP-NOW library
#include <WiFi.h> // Include WiFi library for ESP32

// Define the receiver's MAC address
uint8_t receiverAddress[] = {0xEC, 0x62, 0x60, 0x5B, 0x12, 0xDC};

// Define a structure to hold the outgoing message
typedef struct struct_message {
 char text[32]; // Can hold a string up to 31 characters + null terminator
} struct_message;

// Create an instance of the message structure
struct_message message;

// Callback function to be called after data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 if (status == ESP_NOW_SEND_SUCCESS) {
   Serial.println("Data Sent Successfully");
 } else {
   Serial.println("Error sending Data");
 }
}

void setup() {
 Serial.begin(115200); // Initialize serial communication at 115200 baud rate
 WiFi.mode(WIFI_STA); // Set WiFi mode to station (STA)
 
 // Initialize ESP-NOW. If it fails, print an error message and return from setup
 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW");
   return;
 }
 
 // Register callback function for ESP-NOW send status
 esp_now_register_send_cb(OnDataSent);
 
 // Setup peer information
 esp_now_peer_info_t peerInfo;
 memset(&peerInfo, 0, sizeof(peerInfo)); // Initialize peerInfo memory to zeros
 memcpy(peerInfo.peer_addr, receiverAddress, 6); // Copy receiver MAC address
 peerInfo.channel = 0; // Use the default WiFi channel
 peerInfo.encrypt = false; // Do not use encryption
 
 // Add peer. If it fails, print an error message and return from setup
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
   Serial.println("Failed to add peer");
   return;
 }
}

void loop() {
 // Set the message to be sent
 strcpy(message.text, "Hello World");
 
 // Send the message to the receiver
 esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&message, sizeof(message));
 
 // Check the send result
 if (result == ESP_OK) {
   Serial.println("Sent with success");
 } else {
   Serial.println("Error sending the data");
 }
 
 delay(2000); // Wait for 2 seconds before sending the next message
}
