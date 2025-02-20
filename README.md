Morse Code Translator using ESP32
📌 Overview
This project is a gesture-based Morse Code Translator built using ESP32, which detects hand movements via an ultrasonic sensor, converts them into Morse code, and translates it into text. The decoded text is displayed on an LCD screen and can also be accessed in real-time through a web interface over Wi-Fi.

🚀 Key Features
🔹 Gesture-Based Morse Code Input – Detects hand movements and converts them into Morse code.
🔹 Real-Time Translation – Displays translated text on an LCD screen.
🔹 Web Interface – Enables real-time translation access via a Wi-Fi-hosted dashboard.
🔹 Buzzer Feedback – Provides audible tones for dots and dashes.

🛠️ Components Used
✅ ESP32 (Microcontroller)
✅ LCD Display (16x2)
✅ Ultrasonic Sensor (HC-SR04)
✅ Buzzer
✅ LDR (Light Dependent Resistor)
✅ LEDs
✅ Jumper Wires & Breadboard

📜 Code
The full source code is available in the repository: View Code

🔧 Installation & Setup

1️⃣ Hardware Connections

ESP32 Pin	Component
GPIO 12	Ultrasonic Sensor (Trig)
GPIO 13	Ultrasonic Sensor (Echo)
GPIO 14	Buzzer
GPIO 21, 4, 5, 18, 19, 22	LCD (RS, EN, D4, D5, D6, D7)
2️⃣ Software Setup
✅ Install Arduino IDE with ESP32 board support.
✅ Install required libraries:

WiFi.h
ESPAsyncWebServer.h
LiquidCrystal.h
✅ Clone the repository and open the .ino file in Arduino IDE.
✅ Upload the code to your ESP32.
3️⃣ Running the Project
✅ Power up the ESP32.
✅ Connect to the Wi-Fi network defined in the code.
✅ Open a browser and enter the ESP32’s IP Address to access the web interface.
✅ Start using hand gestures to input Morse code!

🎯 How to Use
✋ Place your hand within the detection range of the ultrasonic sensor to start input.
⏳ Hold for less than 1 second for a dot and more than 1 second for a dash.
🚀 Remove your hand to finalize input and receive the decoded character.
📟 The translated text will appear on both the LCD screen and the web interface.

🤝 Contributors
This project was developed by a dedicated team of 10 members, including me.

#IoT #ESP32 #MorseCode #Arduino #EmbeddedSystems #TechForGood
