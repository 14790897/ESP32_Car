#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h> // For ESP32
#include <ESPAsyncWebServer.h>
#include "secrets.h" // 包含你的配置 (WiFi凭证, API密钥等)

// --- Pin Definitions ---
// DRV8833 #1 - Controls Motor A and B
#define IN1_PIN 2 // Motor A Direction 1
#define IN2_PIN 3 // Motor A Direction 2
#define IN3_PIN 10 // Motor B Direction 1
#define IN4_PIN 6 // Motor B Direction 2

// DRV8833 #2 - Controls Motor C and D
#define IN5_PIN 5 // Motor C Direction 1
#define IN6_PIN 4 // Motor C Direction 2
#define IN7_PIN 8 // Motor D Direction 1
#define IN8_PIN 9 // Motor D Direction 2

// --- PWM Properties ---
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;                         // 0-255 duty cycle range
const int MAX_DUTY_CYCLE = (1 << PWM_RESOLUTION) - 1; // 255 for 8-bit

// --- Global Variables ---
AsyncWebServer server(80); // Web server object on port 80
int motorASpeed = 150;     // Default speed for Motor A (0-255)
int motorBSpeed = 150;     // Default speed for Motor B (0-255)
int motorCSpeed = 150;     // Default speed for Motor C (0-255)
int motorDSpeed = 150;     // Default speed for Motor D (0-255)

// --- HTML Web Page (PROGMEM) ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Motor Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; }
   .control-button { padding: 15px 25px; font-size: 18px; margin: 5px; cursor: pointer; min-width: 100px; }
    #speedValue { font-weight: bold; margin-left: 10px;}
   .slider-container { margin-top: 20px; display: inline-block; }
   .button-row { margin-bottom: 10px; }
  </style>
</head>
<body>
  <h2>ESP32 DRV8833 Web Control</h2>
  <div class="button-row">
    <button class="control-button" onclick="sendCommand('/forward')">Forward</button>
  </div>
  <div class="button-row">
    <button class="control-button" onclick="sendCommand('/left')">Left</button>
    <button class="control-button" onclick="sendCommand('/stop')">Stop</button>
    <button class="control-button" onclick="sendCommand('/right')">Right</button>
  </div>
  <div class="button-row">
    <button class="control-button" onclick="sendCommand('/backward')">Backward</button>
  </div>
  <div class="slider-container">
    <label for="speedSlider">Speed:</label>
    <input type="range" id="speedSlider" min="0" max="255" value="150" oninput="sendSpeed(this.value)" style="vertical-align: middle;">
    <span id="speedValue">150</span>
  </div>

<script>
  function sendCommand(commandPath) {
    console.log("Sending command: " + commandPath);
    fetch(commandPath)
     .then(response => {
        if (!response.ok) { console.error('Command failed:', response.status); }
        return response.text();
      })
     .then(text => console.log('Server response:', text))
     .catch(error => console.error('Fetch error:', error));
  }

  function sendSpeed(speedValue) {
    document.getElementById('speedValue').innerText = speedValue;
    const commandPath = '/speed?value=' + speedValue;
    console.log("Sending speed: " + commandPath);
    fetch(commandPath)
      .then(response => {
         if (!response.ok) { console.error('Speed command failed:', response.status); }
         return response.text();
       })
     .then(text => console.log('Server response:', text))
     .catch(error => console.error('Fetch error:', error));
  }

  // Update display value when slider is moved (using oninput for better responsiveness)
  document.getElementById('speedSlider').addEventListener('input', function() {
    document.getElementById('speedValue').innerText = this.value;
  });

  // Set initial display value on load
  window.onload = () => {
    const slider = document.getElementById('speedSlider');
    document.getElementById('speedValue').innerText = slider.value;
    motorASpeed = parseInt(slider.value); // Sync initial JS speed with C++ variable
    motorBSpeed = parseInt(slider.value);
  };
</script>
</body>
</html>
)rawliteral";

// --- Motor Control Functions ---
// Motor A control functions
void motorA_Forward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(0, speed);  // Use channel 0 for IN1_PIN (LOW)
  ledcWrite(1, 0);      // Use channel 1 for IN2_PIN (HIGH)
}

void motorA_Backward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(0, 0);      // Use channel 0 for IN1_PIN (HIGH)
  ledcWrite(1, speed);  // Use channel 1 for IN2_PIN (LOW)
}

void motorA_Stop()
{
  // Stop by setting both direction pins to LOW
  ledcWrite(0, 0);  // Use channel 0 for IN1_PIN
  ledcWrite(1, 0);  // Use channel 1 for IN2_PIN
}

// Motor B control functions
void motorB_Forward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(2, speed);  // Use channel 2 for IN3_PIN (LOW)
  ledcWrite(3, 0);      // Use channel 3 for IN4_PIN (HIGH)
}

void motorB_Backward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(2, 0);      // Use channel 2 for IN3_PIN (HIGH)
  ledcWrite(3, speed);  // Use channel 3 for IN4_PIN (LOW)
}

void motorB_Stop()
{
  // Stop by setting both direction pins to LOW
  ledcWrite(2, 0);  // Use channel 2 for IN3_PIN
  ledcWrite(3, 0);  // Use channel 3 for IN4_PIN
}

// Motor C control functions
void motorC_Forward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(4, speed);  // Use channel 4 for IN5_PIN (LOW)
  ledcWrite(5, 0);      // Use channel 5 for IN6_PIN (HIGH)
}

void motorC_Backward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(4, 0);      // Use channel 4 for IN5_PIN (HIGH)
  ledcWrite(5, speed);  // Use channel 5 for IN6_PIN (LOW)
}

void motorC_Stop()
{
  // Stop by setting both direction pins to LOW
  ledcWrite(4, 0);  // Use channel 4 for IN5_PIN
  ledcWrite(5, 0);  // Use channel 5 for IN6_PIN
}

// Motor D control functions
void motorD_Forward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(6, speed);  // Use channel 6 for IN7_PIN (LOW)
  ledcWrite(7, 0);      // Use channel 7 for IN8_PIN (HIGH)
}

void motorD_Backward(int speed)
{
  // PWM on direction pins for speed control
  ledcWrite(6, 0);      // Use channel 6 for IN7_PIN (HIGH)
  ledcWrite(7, speed);  // Use channel 7 for IN8_PIN (LOW)
}

void motorD_Stop()
{
  // Stop by setting both direction pins to LOW
  ledcWrite(6, 0);  // Use channel 6 for IN7_PIN
  ledcWrite(7, 0);  // Use channel 7 for IN8_PIN
}

void motorForward()
{
  motorA_Forward(motorASpeed);
  motorB_Forward(motorBSpeed);
  motorC_Forward(motorCSpeed);
  motorD_Forward(motorDSpeed);
  Serial.println("Moving Forward");
}

void motorBackward()
{
  motorA_Backward(motorASpeed);
  motorB_Backward(motorBSpeed);
  motorC_Backward(motorCSpeed);
  motorD_Backward(motorDSpeed);
  Serial.println("Moving Backward");
}

void motorLeft()
{
  // Left side motors backward, right side motors forward
  motorA_Backward(motorASpeed); // Left front
  motorC_Backward(motorCSpeed); // Left rear
  motorB_Forward(motorBSpeed);  // Right front
  motorD_Forward(motorDSpeed);  // Right rear
  Serial.println("Turning Left");
}

void motorRight()
{
  // Left side motors forward, right side motors backward
  motorA_Forward(motorASpeed);  // Left front
  motorC_Forward(motorCSpeed);  // Left rear
  motorB_Backward(motorBSpeed); // Right front
  motorD_Backward(motorDSpeed); // Right rear
  Serial.println("Turning Right");
}

void motorStop()
{
  motorA_Stop();
  motorB_Stop();
  motorC_Stop();
  motorD_Stop();
  Serial.println("Motor Stopped");
}

void setMotorSpeed(int speed)
{
  // Clamp speed value
  if (speed < 0)
    speed = 0;
  if (speed > MAX_DUTY_CYCLE)
    speed = MAX_DUTY_CYCLE;
  motorASpeed = speed;
  motorBSpeed = speed;
  motorCSpeed = speed;
  motorDSpeed = speed;
  Serial.print("Global speed set to: ");
  Serial.println(speed);
  // Re-apply current movement command to update speed immediately if needed
  // Example: if (/* check current moving direction */) { motorForward(); }
}

// --- Setup Function ---
void setup()
{
  Serial.begin(115200);
  Serial.println("\nESP32 DRV8833 Web Control");

  // Initialize Motor Control Pins
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(IN5_PIN, OUTPUT);
  pinMode(IN6_PIN, OUTPUT);
  pinMode(IN7_PIN, OUTPUT);
  pinMode(IN8_PIN, OUTPUT);

  // Initialize PWM channels for all direction pins
  // Motor A
  ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);  // Channel 0 for IN1_PIN
  ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);  // Channel 1 for IN2_PIN
  ledcAttachPin(IN1_PIN, 0);
  ledcAttachPin(IN2_PIN, 1);

  // Motor B
  ledcSetup(2, PWM_FREQ, PWM_RESOLUTION);  // Channel 2 for IN3_PIN
  ledcSetup(3, PWM_FREQ, PWM_RESOLUTION);  // Channel 3 for IN4_PIN
  ledcAttachPin(IN3_PIN, 2);
  ledcAttachPin(IN4_PIN, 3);

  // Motor C
  ledcSetup(4, PWM_FREQ, PWM_RESOLUTION);  // Channel 4 for IN5_PIN
  ledcSetup(5, PWM_FREQ, PWM_RESOLUTION);  // Channel 5 for IN6_PIN
  ledcAttachPin(IN5_PIN, 4);
  ledcAttachPin(IN6_PIN, 5);

  // Motor D
  ledcSetup(6, PWM_FREQ, PWM_RESOLUTION);  // Channel 6 for IN7_PIN
  ledcSetup(7, PWM_FREQ, PWM_RESOLUTION);  // Channel 7 for IN8_PIN
  ledcAttachPin(IN7_PIN, 6);
  ledcAttachPin(IN8_PIN, 7);

  // Stop motors initially
  motorStop();
  setMotorSpeed(150); // Set initial speed variable from slider default

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Replace with your actual SSID and password
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Print ESP32 Local IP Address
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  // --- Define Web Server Routes ---

  // Root route: Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", index_html); // Send the HTML page
  });

  // Movement control routes
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest * request) {
    motorForward();
    request->send(200, "text/plain", "OK - Forward"); });

  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest * request) {
    motorBackward();
    request->send(200, "text/plain", "OK - Backward"); });

  server.on("/left", HTTP_GET, [](AsyncWebServerRequest * request) {
    motorLeft();
    request->send(200, "text/plain", "OK - Left"); });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest * request) {
    motorRight();
    request->send(200, "text/plain", "OK - Right"); });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest * request) {
    motorStop();
    request->send(200, "text/plain", "OK - Stop"); });

  // Speed control route
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest * request) {
    String message;
    if (request->hasParam("value")) {
      String speed_str = request->getParam("value")->value();
      int speed_val = speed_str.toInt();
      setMotorSpeed(speed_val);
      message = "OK - Speed set to " + speed_str;
      request->send(200, "text/plain", message);
    } else {
      message = "ERROR - Missing 'value' parameter";
      request->send(400, "text/plain", message); // Bad Request
    } });

  // Not Found handler
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

// --- Loop Function ---
void loop()
{
  // Async server handles requests in the background.
  // Keep loop() clean or use for non-blocking tasks only.
  // Avoid delay() or long-running operations here.
}