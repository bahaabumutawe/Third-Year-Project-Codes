**Full Sender Project Code:**

The provided code fragment is an intricate example of an embedded system application designed to track physical 
activity metrics using an ESP32 microcontroller and an ADXL345 accelerometer. The application's primary 
function is to calculate and monitor various physical activity indicators such as step count, total distance covered, 
energy expenditure, average speed, peak acceleration, cadence, and rest period durations. The code commences 
with the inclusion of essential libraries for I2C communication (`Wire.h`), ESP-NOW protocol (`esp_now.h`), 
and Wi-Fi functionality (`WiFi.h`), underscoring the application's reliance on both sensor data acquisition and 
wireless communication.

Variables for storing current, next, and average acceleration values (`xcur`, `ycur`, `zcur`, `xnxt`, `ynxt`, `znxt`, 
`xavg`, `yavg`, `zavg`) are declared at the outset, indicating a methodical approach to processing acceleration data 
for movement analysis. The `steps` variable counts the user's steps, while `totalDistance`, `energyExpenditure`, 
`averageSpeed`, and `peakAcceleration` are calculated based on the step count and acceleration data, offering a 
comprehensive overview of the user's physical activity.

The inclusion of time-related variables such as `startTime`, `lastCadenceCalculationTime`, 
`lastStepDetectedTime`, and `restPeriodStartTime` facilitates the temporal analysis of physical activity, allowing 
for the calculation of average speed, cadence, and rest period durations. The code employs a struct `SensorData` 
to encapsulate these metrics, illustrating a structured approach to data management and transmission via ESP-NOW.
In the `setup` function, serial communication is initiated, and the ADXL345 accelerometer is configured through 
I2C, highlighting the integration of peripheral devices for data collection. The ESP-NOW wireless communication 
protocol is initialized, and a peer device is registered using a predefined MAC address, enabling the transmission 
of sensor data to a remote receiver.

The `loop` function contains the core logic for calculating physical activity metrics. It begins by resetting 
acceleration values and calculating current and next acceleration magnitudes. These values are used to update 
peak acceleration and detect steps based on a predefined acceleration threshold, demonstrating an algorithmic 
approach to step detection. The total distance, energy expenditure, and average speed are recalculated with each 
detected step, providing real-time feedback on the user's activity.

Additionally, the code intelligently manages rest periods by tracking the time since the last step was detected and 
updating rest period durations accordingly. This feature highlights the application's capacity to distinguish 
between active and rest phases of physical activity.

Cadence, or steps per minute, is calculated at fixed intervals, offering insight into the intensity of the user's activity. 
The `sendSensorData` function then transmits the collected data to a remote receiver, emphasizing the system's 
capability for wireless data communication.

Finally, the code includes functions for initializing the ADXL345 accelerometer (`adxl345_init`), calculating 
acceleration (`calculateCurrentAcceleration`, `calculateNextAcceleration`), reading acceleration data from the 
ADXL345 (`readAxis`), and initializing average acceleration values (`read_av_acc`). These functions illustrate 
the application's reliance on precise sensor data acquisition and processing to accurately track physical activity 
metrics.

In summary, the provided code represents a sophisticated embedded system application that integrates sensor data 
acquisition, processing, and wireless communication to track and analyze physical activity metrics. Through 
methodical data management, algorithmic processing, and real-time feedback mechanisms, the application offers 
valuable insights into the user's physical activity, underscoring the potential of embedded systems in health and 
fitness monitoring.

**MAC Address Code**

The program begins with the inclusion of the "WiFi.h" library, which is essential for accessing Wi-Fi related 
functions. In the setup() function, serial communication is initiated at a baud rate of 115200 bits per second, 
enabling the device to communicate with the computer over USB. Following this, the Wi-Fi mode is set to STA 
(Station mode) using the WiFi.mode(WIFI_MODE_STA) function. In Station mode, the device can connect to a 
Wi-Fi network as a client, allowing it to access the internet or communicate within the network.

After configuring the Wi-Fi mode, the device's MAC (Media Access Control) address is retrieved and printed to 
the Serial Monitor using Serial.print() and Serial.println(). The MAC address is a unique identifier assigned to 
network interfaces for communications on the physical network segment. Printing the MAC address can be helpful 
for debugging purposes or for identifying the device on a network.

The loop() function is left empty, as the primary purpose of this sketch is to perform the initial Wi-Fi configuration 
and print the MAC address once at the beginning. This minimalist approach ensures that the sketch is 
straightforward and focuses solely on demonstrating how to set up Wi-Fi and retrieve the device's MAC address.

**Basic Sender Code Example**

This program highlights how to use ESP-NOW for sending a simple "Hello World" message from one ESP32 
device to another specified by its MAC address. It begins by including necessary libraries, setting up a receiver's 
MAC address, and defining a message structure. Upon initialization in the `setup()` function, the ESP32 
configures itself for ESP-NOW communication, registers a callback function to monitor the status of sent 
messages, and adds the receiver as a peer. 

In the `loop()` function, the program fills the message structure with "Hello World", attempts to send it to the 
receiver, and then waits for 2 seconds before repeating the process. Success or failure of the message send 
operation is communicated via serial output. This example illustrates the basics of ESP-NOW communication, 
including initialization, peer setup, and message sending, making it suitable for simple peer-to-peer data transfer 
applications.

**Basic Receiver Code Example**

This ESP32 receiver program uses ESP-NOW for wireless communication, receiving messages from another 
ESP32 device. It defines a structure to hold incoming messages and sets up a callback function, `OnDataRecv`, 
to handle received data. When data arrives, this function copies it into the predefined structure and prints the 
message to the Serial Monitor.

The setup involves initializing serial communication, setting the device to station mode, and initializing ESP-NOW. If ESP-NOW initialization fails, an error message is displayed. The program then registers the 
`OnDataRecv` function as the callback for incoming data.

The main loop remains empty as the data reception and processing are managed by the `OnDataRecv` callback 
function, demonstrating a straightforward implementation of ESP-NOW for receiving messages without needing 
a continuous loop.

**Full Receiver Project Code**

This program sets up an ESP32 device as a receiver for ESP-NOW, a protocol for low-power, peer-to-peer 
communication between ESP devices. It defines a `SensorData` structure to match the expected incoming data 
format, including steps, total distance, energy expenditure, average speed, cadence, peak acceleration, and rest 
period duration.

Upon receiving data, the `OnDataReceive` callback function is invoked, printing the sender's MAC address and 
the received sensor data to the serial monitor, provided the data size matches the `SensorData` structure. If the 
size differs, an error message indicates the discrepancy.

The setup process involves initializing serial communication, configuring the ESP32 in Wi-Fi Station mode to 
prevent it from connecting to a Wi-Fi network, and initializing ESP-NOW. If ESP-NOW initialization fails, the 
device restarts to try again. The callback function for receiving data is registered, completing the setup.

The main loop is left empty since data reception and processing are handled asynchronously by the callback 
function, demonstrating an efficient, event-driven approach to receiving structured sensor data over ESP-NOW.

**MATLAB Project Code**

This MATLAB script sets up a real-time data visualization from an Arduino via a serial connection. Initially, it 
clears any existing serial connections to avoid conflicts. It then establishes a new serial connection on a specified 
COM port (e.g., `COM7`) with a baud rate of 115200. The script configures this connection to call a 
`readSerialData` function whenever data arrives, using a line feed as the terminator.

Global variables are declared for storing sensor data (steps, total distance, energy expenditure, average speed, 
cadence, peak acceleration, and rest period duration) and for handling the graphical representation of each metric 
in bar graphs.

A figure with subplots for each metric is initialized through the `modernFigures` function, creating a 
comprehensive dashboard for data visualization. The `readSerialData` function, triggered by incoming serial data, 
parses the received string into numeric values, updates the data storage variables, and refreshes the bar graphs to 
display the latest values.

Utility functions `modernFigures` and `updateBarGraph` manage the graphical interface, setting up the initial 
layout of the dashboard and updating the graphs with new data, respectively. This setup provides an efficient and 
dynamic way to monitor and visualize sensor data from an Arduino in real-time.
