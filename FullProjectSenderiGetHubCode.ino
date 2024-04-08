#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>

// Declaration of variables for acceleration and motion calculations
float xavg = 0, yavg = 0, zavg = 0;
float xcur = 0, ycur = 0, zcur = 0;
float xnxt = 0, ynxt = 0, znxt = 0;
int steps = 0;
float totalDistance = 0; // Total distance covered in meters
float energyExpenditure = 0; // Energy expenditure in calories
float averageSpeed = 0; // Average speed in km/h
unsigned long startTime = millis(); // Stores the start time
unsigned long lastCadenceCalculationTime = 0; // Last cadence calculation timestamp
int stepsSinceLastCadenceCalculation = 0;
float peakAcceleration = 0; // Highest acceleration value detected
unsigned long lastStepDetectedTime = millis(); // Timestamp when the last step was detected
unsigned long lastRestPeriodDuration = 0; // Duration of the last rest period in seconds
unsigned long restPeriodStartTime = 0; // Timestamp when the rest period started
bool isInRestPeriod = false; // Flag to indicate if in a rest period
int lastStepCount = steps; // Step count at the last loop iteration
int cadence = 0; // Steps per minute

// Peer MAC address for ESP-NOW communication
uint8_t broadcastAddress[] = {0xEC, 0x62, 0x60, 0x5B, 0x12, 0xDC}; // Receiver's MAC address
unsigned long restPeriodDuration = 0;

// Struct to package sensor data for transmission
typedef struct {
 int steps;
 float totalDistance;
 float energyExpenditure;
 float averageSpeed;
 int cadence;
 float peakAcceleration;
 unsigned long restPeriodDuration;
} SensorData;

void setup() {
 Serial.begin(115200); // Initialize serial communication
 Wire.begin(); // Initialize I2C
 adxl345_init(); // Initialize ADXL345 accelerometer
 read_av_acc(); // Initialize acceleration averages
 WiFi.mode(WIFI_STA); // Set Wi-Fi to station mode
 delay(100);
 // Initialize ESP-NOW
 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW");
   ESP.restart();
   return;
 }
 // Register ESP-NOW peer
 esp_now_peer_info_t peerInfo;
 memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));
 memcpy(peerInfo.peer_addr, broadcastAddress, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
   Serial.println("Failed to add peer");
   return;
 }
}

void loop() {
 // Reset current and next acceleration values
 xcur = ycur = zcur = 0;
 xnxt = ynxt = znxt = 0;
 calculateCurrentAcceleration();
 calculateNextAcceleration();
 // Calculate average speed considering the elapsed time
 float elapsedTime = (millis() - startTime) / 3600000.0; // Time in hours
 elapsedTime = max(elapsedTime, 0.0001f); // Avoid division by zero
 averageSpeed = (totalDistance / 1000) / elapsedTime; // Speed in km/h

 // Calculate current and next acceleration magnitudes
 float acc = sqrt((xcur - xavg) * (xcur - xavg) + (ycur - yavg) * (ycur - yavg) + (zcur - zavg) * (zcur - zavg));
 float acc2 = sqrt((xnxt - xavg) * (xnxt - xavg) + (ynxt - yavg) * (ynxt - yavg) + (znxt - zavg) * (znxt - zavg));

 // Update peak acceleration if higher acceleration detected
 peakAcceleration = max(peakAcceleration, max(acc, acc2));

 // Step detection logic
 unsigned long currentTime = millis();
 if (acc2 - acc > 0.005) { // Threshold for detecting a step
   steps++;
   totalDistance = steps * 0.7; // Assuming each step covers 0.7 meters
   energyExpenditure = calculateEnergyExpenditure(steps); // Calculate energy expenditure
   stepsSinceLastCadenceCalculation++;
   lastStepDetectedTime = currentTime;
   // Exit rest period if a step is detected
   if (isInRestPeriod) {
     isInRestPeriod = false;
     restPeriodDuration = (currentTime - restPeriodStartTime) / 1000;
     restPeriodStartTime = 0;
   }
 }

 // Check for rest period if no steps detected for 5 seconds
 if (steps == lastStepCount && (currentTime - lastStepDetectedTime > 5000)) {
   if (!isInRestPeriod) {
     isInRestPeriod = true;
     restPeriodStartTime = currentTime;
     Serial.println("Rest period started.");
   }
 } else if (steps != lastStepCount && isInRestPeriod) {
   isInRestPeriod = false;
   restPeriodDuration = (currentTime - restPeriodStartTime) / 1000;
   Serial.print("Rest period ended. Duration: ");
   Serial.print(restPeriodDuration);
   Serial.println(" seconds.");
   restPeriodStartTime = 0;
 }
 lastStepCount = steps; // Update for the next iteration

 // Calculate cadence every minute
 if (currentTime - lastCadenceCalculationTime >= 60000) {
   cadence = stepsSinceLastCadenceCalculation * 60 / ((currentTime - lastCadenceCalculationTime) / 1000);
   stepsSinceLastCadenceCalculation = 0;
   lastCadenceCalculationTime = currentTime;
 }

 // Send sensor data including rest period duration
 sendSensorData();

 // Print steps and other metrics for debugging
 Serial.print(steps);
 Serial.print(",");
 Serial.print(totalDistance, 3);
 Serial.print(",");
 Serial.print(energyExpenditure, 2);
 Serial.print(",");
 Serial.print(averageSpeed, 2);
 Serial.print(",");
 Serial.print(cadence);
 Serial.print(",");
 Serial.print(peakAcceleration, 3);
 Serial.print(",");
 Serial.println(restPeriodDuration);
 delay(800); // Loop delay
}

void sendSensorData() {
 SensorData data;
 // Populate the structure with current sensor data
 data.steps = steps;
 data.totalDistance = totalDistance;
 data.energyExpenditure = energyExpenditure;
 data.averageSpeed = averageSpeed;
 data.cadence = cadence;
 data.peakAcceleration = peakAcceleration;
 data.restPeriodDuration = restPeriodDuration;
 // Send the data via ESP-NOW
 esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(SensorData));
}

void adxl345_init() {
 // Initialize ADXL345 with basic settings
 Wire.beginTransmission(0x53);
 Wire.write(0x2D);
 Wire.write(0x08);
 Wire.endTransmission();
 delay(5);
 Wire.beginTransmission(0x53);
 Wire.write(0x31);
 Wire.write(0x08); // Set to full resolution
 Wire.endTransmission();
 delay(5);
}

void calculateCurrentAcceleration() {
 // Accumulate readings and calculate average current acceleration
 for (int i = 0; i < 50; i++) {
   xcur += readAxis(0x32);
   ycur += readAxis(0x34);
   zcur += readAxis(0x36);
 }
 xcur /= 50;
 ycur /= 50;
 zcur /= 50;
}

void calculateNextAcceleration() {
 // Accumulate readings and calculate average next acceleration
 for (int i = 0; i < 50; i++) {
   xnxt += readAxis(0x32);
   ynxt += readAxis(0x34);
   znxt += readAxis(0x36);
 }
 xnxt /= 50;
 ynxt /= 50;
 znxt /= 50;
}

float readAxis(byte axisAddress) {
 // Read acceleration data from ADXL345 sensor
 Wire.beginTransmission(0x53);
 Wire.write(axisAddress);
 Wire.endTransmission(false);
 Wire.requestFrom(0x53, 2, true);
 byte l = Wire.read();
 byte h = Wire.read();
 int16_t value = (int16_t)(l | (h << 8));
 return value * 0.004; // Convert to g's
}

void read_av_acc() {
 // Initialize averages by reading acceleration multiple times
 for (int i = 0; i < 50; i++) {
   xavg += readAxis(0x32);
   yavg += readAxis(0x34);
   zavg += readAxis(0x36);
 }
 xavg /= 50;
 yavg /= 50;
 zavg /= 50;
}

float calculateEnergyExpenditure(int stepsTaken){
 // Simple calculation of energy expenditure based on steps taken
 return stepsTaken * 0.08;
}
