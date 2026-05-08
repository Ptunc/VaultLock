VaultLock: Physical Vibration-Based Workstation Security System

Project Purpose and Cybersecurity Context

Cybersecurity is not just about firewalls, encryption algorithms, or network monitoring tools. The weakest link in information security is **physical access**. An attacker gaining physical access to an unlocked computer means all digital defense lines can be breached in seconds (See: *Evil Maid Attacks*, *Insider Threats*).

**VaultLock** is a hardware-based **"Tripwire"** system that activates when you leave your computer unattended for a short time in public or shared spaces like offices, cafes, or libraries. If the device is moved, attempted to be put into a bag, or if the desk is shaken, the hardware accelerometer detects the vibration and instantly locks the operating system.

System Features

*   **Out-of-Band Triggering:** Unlike BadUSB attacks (e.g., Rubber Ducky), the system does not introduce itself directly as an HID (Keyboard/Mouse). Thus, it is not blocked by operating system security policies or antivirus software.
*   **Raw Data Processing (Library-Free):** Instead of relying on bulky third-party libraries, data from the MPU6050 sensor is read directly at the I2C register level (0x3B) via `Wire.h`. This prevents clone hardware incompatibilities and reduces reaction time to milliseconds.
*   **Vectoral Acceleration Calculation:** To ensure orientation-independent shake detection, the magnitude of the 3-axis (X, Y, Z) acceleration vector is calculated dynamically.
*   **Visual Feedback:** Hardware status LEDs allow easy monitoring of whether the system is armed, running, or triggered.

Hardware Requirements

*   1x Arduino Uno 
*   1x MPU6050 6-Axis Accelerometer and Gyroscope Sensor
*   2x LEDs (Green and Red)
*   2x 220Ω - 330Ω Resistors
*   Jumper Wires and a Breadboard

Circuit Diagram and Connections

| MPU6050 Pin | Arduino Uno Pin | Function |
| :--- | :--- | :--- |
| VCC | 5V | Power Supply |
| GND | GND | Ground |
| SDA | A4 | I2C Data Line |
| SCL | A5 | I2C Clock Line |

**Status Indicators:**
*   **Green LED (System Ready):** Arduino `Pin 8` -> Resistor -> GND
*   **Red LED (Danger/Lock):** Arduino `Pin 9` -> Resistor -> GND

Setup and Usage

### 1. Arduino Configuration (Firmware)
1. Open the `main.ino` file using the Arduino IDE.
2. There is no need to install any external libraries (like Adafruit, etc.); the code uses the standard `Wire.h` library.
3. Upload the code to your Arduino Uno.
4. After a successful upload, the green LED will turn on, indicating that the system is active and monitoring.

### 2. Python Host Script Configuration
The system requires a lightweight background script listening to the serial port.
1. Make sure Python 3 is installed on your computer.
2. Install the required serial communication library via terminal/cmd:

Bash
pip install pyserial
   
Open koruma.py and update the arduino_port = 'COM3' variable to match the COM port your Arduino is connected to.

Run the script:

Bash
python koruma.py


Working Logic and Algorithm

1.  **Hardware Layer:** The Arduino wakes the MPU6050 from Sleep mode and continuously requests acceleration data via the I2C protocol.
2.  **Mathematical Model:** Raw data from the X, Y, and Z axes are converted to standard gravitational acceleration (g). Then, the total resultant acceleration is calculated using the following formula:
    $Total\ Acceleration = \sqrt{x^2 + y^2 + z^2}$
3.  **Triggering:** On a stationary desk, the total acceleration is approximately 9.8 m/s². If this value exceeds the defined security threshold (e.g., 13.0), a `KILITLE` (LOCK) flag is transmitted over the serial port.
4.  **Operating System Layer:** The moment the background Python script catches this flag, it calls the Windows API to execute the `rundll32.exe user32.dll,LockWorkStation` command, locking the workstation instantaneously.

Security Warnings and Future Improvements
*   **Port Collision:** Make sure no other application (e.g., the Arduino IDE Serial Monitor) is using the same COM port while the Python script is running. Otherwise, you will receive an "Access Denied" error.
*   **Future Roadmap:** Upcoming versions plan to integrate a Telegram Bot API. This will allow the system to send real-time cyber threat intelligence (timestamps and trigger logs) directly to the user's mobile device when a physical breach attempt is detected.

---
*Developer Note: This project was designed as a Proof of Concept (PoC) to demonstrate hardware-based physical access control methodologies.*
