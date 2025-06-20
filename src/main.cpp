#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h> // For ESP32
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ESPmDNS.h>
#include "secrets.h" // 包含你的配置 (WiFi凭证, API密钥等)

// --- Pin Definitions ---
// DRV8833 #1 - Controls Motor A and B

#define IN1_PIN 2 // Motor A Direction 1
#define IN2_PIN 3 // Motor A Direction 2
#define IN3_PIN 10 // Motor B Direction 1
#define IN4_PIN 6  // Motor B Direction 2
// DRV8833 #2 - Controls Motor C and D
#define IN5_PIN 1  // Motor C Direction 1
#define IN6_PIN 12 // Motor C Direction 2
#define IN7_PIN 18 // Motor D Direction 1
#define IN8_PIN 19 // Motor D Direction 2

// --- PWM Properties ---
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;                         // 0-255 duty cycle range
const int MAX_DUTY_CYCLE = (1 << PWM_RESOLUTION) - 1; // 255 for 8-bit

// --- Global Variables ---
AsyncWebServer server(80); // Web server object on port 80
const char *mdnsHostname = "esp32car"; // mDNS hostname (will be esp32car.local)
int motorASpeed = 150;     // Default speed for Motor A (0-255)
int motorBSpeed = 150;     // Default speed for Motor B (0-255)
int motorCSpeed = 150;     // Default speed for Motor C (0-255)
int motorDSpeed = 150;     // Default speed for Motor D (0-255)

// --- LittleFS Helper Functions ---
String getContentType(String filename)
{
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".svg"))
    return "image/svg+xml";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

// --- Motor Control Functions ---
// Motor A control functions
void motorA_Forward(int speed)
{
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  // Set speed by analogWrite only on one pin if needed for speed control
  // For basic operation, we'll use digitalWrite only
}

void motorA_Backward(int speed)
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
}

void motorA_Stop()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
}

// Motor B control functions
void motorB_Forward(int speed)
{
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void motorB_Backward(int speed)
{
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}

void motorB_Stop()
{
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

// Motor C control functions
void motorC_Forward(int speed)
{
  digitalWrite(IN5_PIN, HIGH);
  digitalWrite(IN6_PIN, LOW);
}

void motorC_Backward(int speed)
{
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, HIGH);
}

void motorC_Stop()
{
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, LOW);
}

// Motor D control functions
void motorD_Forward(int speed)
{
  digitalWrite(IN7_PIN, HIGH);
  digitalWrite(IN8_PIN, LOW);
}

void motorD_Backward(int speed)
{
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, HIGH);
}

void motorD_Stop()
{
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, LOW);
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
  // Left side motors forward, right side motors backward
  motorA_Forward(motorASpeed);  // Left front
  motorC_Forward(motorCSpeed);  // Left rear
  motorB_Backward(motorBSpeed); // Right front
  motorD_Backward(motorDSpeed); // Right rear
  Serial.println("Turning Left");
}

void motorRight()
{
  // Left side motors backward, right side motors forward
  motorA_Backward(motorASpeed); // Left front
  motorC_Backward(motorCSpeed); // Left rear
  motorB_Forward(motorBSpeed);  // Right front
  motorD_Forward(motorDSpeed);  // Right rear
  Serial.println("Turning Right");
}

void motorStop()
{
  motorA_Stop();
  motorB_Stop();
  motorC_Stop();
  motorD_Stop();
  Serial.println("All Motors Stopped");
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

// --- Motor Diagnostic Functions ---
void testMotor(char motor, int speed = 150)
{
  Serial.print("Testing Motor ");
  Serial.print(motor);
  Serial.print(" at speed ");
  Serial.println(speed);

  switch (motor)
  {
  case 'A':
    Serial.println("Motor A Forward...");
    motorA_Forward(speed);
    delay(2000);
    Serial.println("Motor A Stop");
    motorA_Stop();
    delay(1000);
    Serial.println("Motor A Backward...");
    motorA_Backward(speed);
    delay(2000);
    Serial.println("Motor A Stop");
    motorA_Stop();
    break;

  case 'B':
    Serial.println("Motor B Forward...");
    motorB_Forward(speed);
    delay(2000);
    Serial.println("Motor B Stop");
    motorB_Stop();
    delay(1000);
    Serial.println("Motor B Backward...");
    motorB_Backward(speed);
    delay(2000);
    Serial.println("Motor B Stop");
    motorB_Stop();
    break;

  case 'C':
    Serial.println("Motor C Forward...");
    motorC_Forward(speed);
    delay(2000);
    Serial.println("Motor C Stop");
    motorC_Stop();
    delay(1000);
    Serial.println("Motor C Backward...");
    motorC_Backward(speed);
    delay(2000);
    Serial.println("Motor C Stop");
    motorC_Stop();
    break;

  case 'D':
    Serial.println("Motor D Forward...");
    motorD_Forward(speed);
    delay(2000);
    Serial.println("Motor D Stop");
    motorD_Stop();
    delay(1000);
    Serial.println("Motor D Backward...");
    motorD_Backward(speed);
    delay(2000);
    Serial.println("Motor D Stop");
    motorD_Stop();
    break;
  }
  Serial.println("Motor test completed\n");
}

void testAllMotors()
{
  Serial.println("=== Starting Individual Motor Tests ===");
  testMotor('A');
  delay(1000);
  testMotor('B');
  delay(1000);
  testMotor('C');
  delay(1000);
  testMotor('D');
  delay(1000);
  Serial.println("=== All Motor Tests Completed ===");
}

void printMotorStatus()
{
  Serial.println("\n=== Motor Configuration Status ===");
  Serial.print("Motor A Speed: ");
  Serial.println(motorASpeed);
  Serial.print("Motor B Speed: ");
  Serial.println(motorBSpeed);
  Serial.print("Motor C Speed: ");
  Serial.println(motorCSpeed);
  Serial.print("Motor D Speed: ");
  Serial.println(motorDSpeed);
  Serial.println("\nPin Configuration:");
  Serial.print("IN1 (Motor A+): GPIO ");
  Serial.println(IN1_PIN);
  Serial.print("IN2 (Motor A-): GPIO ");
  Serial.println(IN2_PIN);
  Serial.print("IN3 (Motor B+): GPIO ");
  Serial.println(IN3_PIN);
  Serial.print("IN4 (Motor B-): GPIO ");
  Serial.println(IN4_PIN);
  Serial.print("IN5 (Motor C+): GPIO ");
  Serial.println(IN5_PIN);
  Serial.print("IN6 (Motor C-): GPIO ");
  Serial.println(IN6_PIN);
  Serial.print("IN7 (Motor D+): GPIO ");
  Serial.println(IN7_PIN);
  Serial.print("IN8 (Motor D-): GPIO ");
  Serial.println(IN8_PIN);
  Serial.println("=====================================\n");
}

// --- Setup Function ---
void setup()
{
  Serial.begin(115200);
  Serial.println("\nESP32 DRV8833 Web Control");
  // Initialize LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  // Check if required files exist
  Serial.println("Checking LittleFS files:");
  if (LittleFS.exists("/index.html"))
  {
    Serial.println("✓ /index.html found");
  }
  else
  {
    Serial.println("✗ /index.html NOT found");
  }

  if (LittleFS.exists("/css/style.css"))
  {
    Serial.println("✓ /css/style.css found");
  }
  else
  {
    Serial.println("✗ /css/style.css NOT found");
  }

  if (LittleFS.exists("/js/app.js"))
  {
    Serial.println("✓ /js/app.js found");
  }
  else
  {
    Serial.println("✗ /js/app.js NOT found");
  }

  // List all files in LittleFS for debugging
  Serial.println("Files in LittleFS:");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    Serial.print("  ");
    Serial.println(file.name());
    file = root.openNextFile();
  }
  file.close();
  root.close();
  // Initialize Motor Control Pins
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(IN5_PIN, OUTPUT);
  pinMode(IN6_PIN, OUTPUT);
  pinMode(IN7_PIN, OUTPUT);
  pinMode(IN8_PIN, OUTPUT);

  // Initialize all motor pins to LOW (stopped)
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, LOW);
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, LOW);
  // Stop motors initially
  motorStop();
  setMotorSpeed(150); // Set initial speed variable from slider default

  // Print motor configuration
  printMotorStatus();

  // Optional: Uncomment to test each motor individually at startup
  // WARNING: This will run motors for 8 seconds each!
  // testAllMotors();

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Replace with your actual SSID and password
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Initialize mDNS
  if (!MDNS.begin(mdnsHostname))
  {
    Serial.println("Error setting up MDNS responder!");
  }
  else
  {
    Serial.println("mDNS responder started");
    Serial.print("Access the car at: http://");
    Serial.print(mdnsHostname);
    Serial.println(".local");

    // Add service to mDNS-SD
    MDNS.addService("http", "tcp", 80);
    MDNS.addServiceTxt("http", "tcp", "model", "ESP32_Car");
    MDNS.addServiceTxt("http", "tcp", "version", "1.0");
    MDNS.addServiceTxt("http", "tcp", "description", "ESP32 DRV8833 Car Control");
  }

  // Print ESP32 Local IP Address
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());
  // --- Define Web Server Routes ---  // Root route: Serve the HTML page from LittleFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(LittleFS.exists("/index.html")) {
                request->send(LittleFS, "/index.html", "text/html; charset=utf-8");
              } else {
                request->send(404, "text/plain", "index.html not found in LittleFS");
              } });

  // Diagnostics page
  server.on("/diagnostics", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(LittleFS.exists("/diagnostics.html")) {
                request->send(LittleFS, "/diagnostics.html", "text/html; charset=utf-8");
              } else {
                request->send(404, "text/plain", "diagnostics.html not found in LittleFS");
              } });

  // Serve CSS files
  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(LittleFS.exists("/css/style.css")) {
                request->send(LittleFS, "/css/style.css", "text/css");
              } else {
                request->send(404, "text/plain", "CSS file not found");
              } });

  // Serve JavaScript files
  server.on("/js/app.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(LittleFS.exists("/js/app.js")) {
                request->send(LittleFS, "/js/app.js", "application/javascript");
              } else {
                request->send(404, "text/plain", "JS file not found");
              } });
  // Handle favicon requests (both .ico and .svg)
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(LittleFS.exists("/favicon.ico")) {
                request->send(LittleFS, "/favicon.ico", "image/x-icon");
              } else {
                request->send(204); // No Content
              } });
              
  server.on("/favicon.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(LittleFS.exists("/favicon.svg")) {
                request->send(LittleFS, "/favicon.svg", "image/svg+xml");
              } else {
                request->send(204); // No Content
              } });

  // Serve SVG icons
  server.on("/icons/forward.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/forward.svg", "image/svg+xml"); });
  server.on("/icons/backward.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/backward.svg", "image/svg+xml"); });
  server.on("/icons/left.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/left.svg", "image/svg+xml"); });
  server.on("/icons/right.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/right.svg", "image/svg+xml"); });
  server.on("/icons/stop.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/stop.svg", "image/svg+xml"); });
  server.on("/icons/speed.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/speed.svg", "image/svg+xml"); });
  server.on("/icons/car.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/car.svg", "image/svg+xml"); });
  server.on("/icons/wifi.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/icons/wifi.svg", "image/svg+xml"); });
  // Status check endpoint for connection monitoring
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String status = "{";
    status += "\"wifi_connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    status += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    status += "\"mdns_hostname\":\"" + String(mdnsHostname) + ".local\",";
    status += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    status += "\"motor_speeds\":{";
    status += "\"A\":" + String(motorASpeed) + ",";
    status += "\"B\":" + String(motorBSpeed) + ",";
    status += "\"C\":" + String(motorCSpeed) + ",";
    status += "\"D\":" + String(motorDSpeed);
    status += "},";
    status += "\"uptime\":" + String(millis());
    status += "}";
    request->send(200, "application/json", status);
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

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request)
            {
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

  // Motor diagnostic routes
  server.on("/test/motor", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message = "Motor Test Commands:\n";
    message += "/test/motor/A - Test Motor A\n";
    message += "/test/motor/B - Test Motor B\n"; 
    message += "/test/motor/C - Test Motor C\n";
    message += "/test/motor/D - Test Motor D\n";
    message += "/test/motor/all - Test All Motors\n";
    request->send(200, "text/plain", message); });
  server.on("/test/motor/A", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Received request: /test/motor/A");
    testMotor('A');
    request->send(200, "text/plain", "Motor A test completed - check serial monitor"); });

  server.on("/test/motor/B", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Received request: /test/motor/B");
    testMotor('B');
    request->send(200, "text/plain", "Motor B test completed - check serial monitor"); });

  server.on("/test/motor/C", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Received request: /test/motor/C");
    testMotor('C');
    request->send(200, "text/plain", "Motor C test completed - check serial monitor"); });

  server.on("/test/motor/D", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Received request: /test/motor/D");
    testMotor('D');
    request->send(200, "text/plain", "Motor D test completed - check serial monitor"); });

  server.on("/test/motor/all", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Received request: /test/motor/all");
    testAllMotors();
    request->send(200, "text/plain", "All motor tests completed - check serial monitor"); });

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

  // Note: ESP32 mDNS runs automatically in background,
  // no manual update needed unlike ESP8266
}