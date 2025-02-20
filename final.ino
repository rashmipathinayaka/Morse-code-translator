#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal.h>  // Include the LCD library

const char* ssid = "Kavee";
const char* password = "11111111";

AsyncWebServer server(80);

// Define pins for the ultrasonic sensor
const int trigPin = 12;
const int echoPin = 13;
const int buzzerPin = 14;

// Define LCD pins: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(21, 4, 5, 18, 19, 22);  // Connect the LCD to ESP32

const float upperThreshold = 10;
const float lowerThreshold = 5;

// Timing thresholds (in milliseconds)
const unsigned long dotThreshold = 1000; // < 1000ms = dot, >= 1000ms = dash
const unsigned long letterGap = 2000;    // 2 seconds of inactivity = end of letter
const unsigned long wordGap = 5000;      // 4 seconds of inactivity = end of word

// Variables for gesture timing
bool handDetected = false;
unsigned long handStartTime = 0;
unsigned long lastGestureTime = 0;
bool buzzerActive = false;
String morseSequence = "";
String translatedMessage = "";
String lastCharacter = " "; 

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Morse Code Translator</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #121212;
      color: #ffffff;
      text-align: center;
      margin: 0;
      padding: 20px;
    }
    h1 {
      font-size: 2.5em;
      margin-bottom: 10px;
      color: #ffcc00;
    }
    p {
      font-size: 1.5em;
      background: #333;
      padding: 15px;
      border-radius: 10px;
      display: inline-block;
      box-shadow: 0px 0px 10px rgba(255, 255, 255, 0.2);
    }
  </style>
  <script>
    function fetchMessages() {
      fetch("/message")
        .then(response => response.text())
        .then(data => { document.getElementById("output").innerText = data; });
    }
    setInterval(fetchMessages, 1000);
  </script>
</head>
<body>
  <h1>Morse Code Translator</h1>
  <p id="output">Waiting for messages...</p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.println("\nConnected! IP Address: " + WiFi.localIP().toString());

  setupServer();

  server.begin();
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);  // Ensure buzzer is off initially
  
  Serial.println("Morse Code Translator Started");

  // Initialize LCD
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.print("Morse Code");  // Print a welcome message to the LCD
  lcd.setCursor(0, 1);
  lcd.print("Translator");
  delay(2000);  // Wait for 2 seconds
  lcd.clear();
}

void loop() {
  float distance = measureDistance();

  if (!handDetected && (distance >= lowerThreshold && distance <= upperThreshold)) {
    handDetected = true;
    handStartTime = millis();
  }

  // If hand is detected and time exceeds the dash threshold, sound the buzzer
  if (handDetected && (millis() - handStartTime >= dotThreshold) && !buzzerActive) {
    digitalWrite(buzzerPin, HIGH);
    buzzerActive = true;
  }

  if (handDetected && (distance > upperThreshold || distance < lowerThreshold)) {
    // Hand leaves the detection range
    unsigned long gestureDuration = millis() - handStartTime;
    handDetected = false;
    lastGestureTime = millis();
    
    // Turn off buzzer
    digitalWrite(buzzerPin, LOW);
    buzzerActive = false;
    
    // Decide whether it's a dot or a dash based on duration
    if (gestureDuration < dotThreshold && gestureDuration > 200) {
      morseSequence += ".";
      Serial.print("Detected DOT (");
    } else if (gestureDuration > dotThreshold && gestureDuration > 200) {
      morseSequence += "-";
      Serial.print("Detected DASH (");
    }
    
    Serial.print(gestureDuration);
    Serial.println(" ms)");
    Serial.print("Current Morse sequence: ");
    Serial.println(morseSequence);
  }

  if (!handDetected && morseSequence.length() > 0 && (millis() - lastGestureTime) > letterGap) {
    char letter = decodeMorse(morseSequence);
    Serial.print("Decoded Letter: ");
    Serial.println(letter);

    translatedMessage += letter;
    lastCharacter = letter;
    
    morseSequence = "";
  }

  if (!handDetected && (millis() - lastGestureTime) > wordGap && lastCharacter != " ") {
    translatedMessage += " ";
    lastCharacter = " ";
  }

  // Update the LCD with the translated message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Morse: ");
  lcd.setCursor(0, 1);
  lcd.print(translatedMessage);

  delay(50);
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.0343 / 2.0;
  return distance;
}

char decodeMorse(String morse) {
  if (morse == ".-") return 'A';
  if (morse == "-...") return 'B';
  if (morse == "-.-.") return 'C';
  if (morse == "-..") return 'D';
  if (morse == ".") return 'E';
  if (morse == "..-.") return 'F';
  if (morse == "--.") return 'G';
  if (morse == "....") return 'H';
  if (morse == "..") return 'I';
  if (morse == ".---") return 'J';
  if (morse == "-.-") return 'K';
  if (morse == ".-..") return 'L';
  if (morse == "--") return 'M';
  if (morse == "-.") return 'N';
  if (morse == "---") return 'O';
  if (morse == ".--.") return 'P';
  if (morse == "--.-") return 'Q';
  if (morse == ".-.") return 'R';
  if (morse == "...") return 'S';
  if (morse == "-") return 'T';
  if (morse == "..-") return 'U';
  if (morse == "...-") return 'V';
  if (morse == ".--") return 'W';
  if (morse == "-..-") return 'X';
  if (morse == "-.--") return 'Y';
  if (morse == "--..") return 'Z';
  return '?';
}

void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", htmlPage);
  });

  server.on("/message", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", translatedMessage);
  });

  server.begin();
}

void addMorseCharacter(char letter) {
  translatedMessage += letter;
}

void addSpaceForWord() {
  translatedMessage += " ";
}
