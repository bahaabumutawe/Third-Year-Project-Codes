#include <esp_now.h> // Include ESP-NOW library
#include <WiFi.h> // Include WiFi library for network configuration

// Define a structure for the sensor data to be received
typedef struct {
 int steps; // Number of steps taken
 float totalDistance; // Total distance covered
 float energyExpenditure; // Energy expenditure in calories
 float averageSpeed; // Average speed
 int cadence; // Steps per minute
 float peakAcceleration; // Maximum acceleration recorded
 unsigned long restPeriodDuration; // Duration of the last rest period
} SensorData;

// Callback function to be executed when data is received
void OnDataReceive(const uint8_t* mac, const uint8_t* incomingData, int len) {
 Serial.print("Data received from: ");
 // Print the MAC address of the sender
 for (int i = 0; i < 6; i++) {
   Serial.print(mac[i], HEX);
   if (i < 5) Serial.print(":");
 }
 Serial.println();

 // Verify the length of the received data
 if (len == sizeof(SensorData)) {
   SensorData receivedData; // Temporary storage for the received data
   // Copy the received data into our structure
   memcpy(&receivedData, incomingData, sizeof(SensorData));

   // Print the received data values
   Serial.print("Steps: "); Serial.print(receivedData.steps);
   Serial.print(", Total Distance: "); Serial.print(receivedData.totalDistance, 3);
   Serial.print(" m, Energy Expenditure: "); Serial.print(receivedData.energyExpenditure, 2);
   Serial.print(" cal, Average Speed: "); Serial.print(receivedData.averageSpeed, 2);
   Serial.print(" km/h, Cadence: "); Serial.print(receivedData.cadence);
   Serial.print(" steps/min, Peak Acceleration: "); Serial.print(receivedData.peakAcceleration, 3);
   Serial.print(" g, Rest Period Duration: "); Serial.println(receivedData.restPeriodDuration);
   Serial.println(" seconds.");
 } else {
   // Handle unexpected data size
   Serial.print("Unexpected data size received. Expected: ");
   Serial.print(sizeof(SensorData));
   Serial.print(", Received: ");
   Serial.println(len);
 }
}

void setup() {
 Serial.begin(115200); // Initialize serial communication
 Serial.println("ESP-NOW Receiver");

 // Initialize WiFi in Station (STA) mode
 WiFi.mode(WIFI_STA);
 delay(100); // Short delay to ensure WiFi mode is set before proceeding
 WiFi.disconnect(); // Ensure the ESP32 is not connected to a WiFi network

 // Initialize ESP-NOW
 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW, retrying...");
   ESP.restart(); // Restart the ESP32 if initialization fails
   return;
 }

 // Register the callback function for receiving data
 esp_now_register_recv_cb(OnDataReceive);
}

void loop() {
  // The loop function is intentionally left empty
  // All actions are handled in the OnDataReceive callback
}
