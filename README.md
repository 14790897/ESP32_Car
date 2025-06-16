# ESP32 Car with LittleFS Web Interface

This project has been modified to use LittleFS for serving web files instead of storing them in PROGMEM. This provides better separation of concerns and easier maintenance of the frontend.

## Project Structure

```
ESP32_Car/
├── platformio.ini          # PlatformIO configuration
├── src/
│   └── main.cpp            # Main ESP32 code
├── include/
│   └── secrets.h           # WiFi credentials and configuration
└── data/                   # LittleFS files (web interface)
    ├── index.html          # Main web page
    ├── css/
    │   └── style.css       # Stylesheet
    └── js/
        └── app.js          # JavaScript functionality
```

## Features

### Frontend (LittleFS)
- Modern, responsive web interface
- Separated HTML, CSS, and JavaScript files
- Enhanced styling with hover effects and transitions
- Keyboard controls (WASD + Arrow keys + Spacebar for stop)
- Speed control slider with real-time feedback

### Backend (ESP32)
- WiFi web server
- Motor control for 4-wheel drive
- DRV8833 motor driver support
- PWM speed control
- RESTful API endpoints

## Setup Instructions

### 1. Hardware Connection
Connect your DRV8833 motor drivers according to the pin definitions in `main.cpp`:

**DRV8833 #1 (Motors A & B):**
- IN1 → GPIO 2
- IN2 → GPIO 3  
- IN3 → GPIO 10
- IN4 → GPIO 6

**DRV8833 #2 (Motors C & D):**
- IN5 → GPIO 1
- IN6 → GPIO 12
- IN7 → GPIO 18
- IN8 → GPIO 19

### 2. Software Setup

1. **Configure WiFi**: Edit `include/secrets.h` with your WiFi credentials:
   ```cpp
   #define WIFI_SSID "YourWiFiName"
   #define WIFI_PASSWORD "YourWiFiPassword"
   ```

2. **Upload Filesystem**: Before uploading the main code, you need to upload the LittleFS filesystem containing the web files:
   ```bash
   pio run --target uploadfs
   ```

3. **Upload Code**: After the filesystem is uploaded, upload the main code:
   ```bash
   pio run --target upload
   ```

### 3. Usage

1. Open Serial Monitor to see the ESP32's IP address and mDNS hostname
2. Connect to the same WiFi network as the ESP32
3. Open a web browser and navigate to either:
   - **mDNS Address**: `http://esp32car.local` (recommended)
   - **IP Address**: `http://192.168.x.x` (shown in serial monitor)
4. Use the web interface to control the car:
   - Buttons for Forward/Backward/Left/Right/Stop
   - Speed slider (0-255)
   - Keyboard controls (WASD/Arrow keys/Spacebar)

### 4. Motor Troubleshooting

If only one motor is working, check:

1. **Visit Diagnostics Page**: Go to `http://esp32car.local/diagnostics`
2. **Test Individual Motors**: Use the diagnostic interface to test each motor
3. **Check Pin Connections**: Verify wiring matches the updated pin configuration:

**Updated Pin Configuration (safer GPIO pins):**
- Motor A: GPIO 4 (IN1), GPIO 5 (IN2)
- Motor B: GPIO 16 (IN3), GPIO 17 (IN4)  
- Motor C: GPIO 18 (IN5), GPIO 19 (IN6)
- Motor D: GPIO 21 (IN7), GPIO 22 (IN8)

4. **Common Issues**:
   - Loose connections
   - DRV8833 power supply issues
   - Wrong GPIO pin assignments
   - Motor driver chip failure
   - Keyboard controls (WASD/Arrow keys/Spacebar)

## API Endpoints

- `GET /` - Main web interface
- `GET /status` - Device status and information (JSON)
- `GET /forward` - Move forward
- `GET /backward` - Move backward  
- `GET /left` - Turn left
- `GET /right` - Turn right
- `GET /stop` - Stop all motors
- `GET /speed?value=X` - Set speed (X = 0-255)

## mDNS Support

The ESP32 Car supports mDNS (Multicast DNS) for easy device discovery:

- **Hostname**: `esp32car.local`
- **Service**: HTTP on port 80
- **Service Discovery**: The device advertises itself as "ESP32_Car" with additional metadata

### Benefits of mDNS:
- No need to remember IP addresses
- Easy access via `http://esp32car.local`
- Automatic device discovery on the network
- Works with most modern browsers and operating systems

### Troubleshooting mDNS:
- **Windows**: Ensure Bonjour service is installed (comes with iTunes or can be installed separately)
- **macOS/iOS**: Built-in support, should work out of the box
- **Linux**: Install `avahi-daemon` package
- **Android**: Most browsers support mDNS, some may need specific apps

## Development Notes

### LittleFS Benefits
- **Separation of Concerns**: Frontend and backend code are separated
- **Easier Maintenance**: Web files can be edited without recompiling firmware
- **Better Performance**: No large strings in program memory
- **Scalability**: Easy to add more web assets

### File Upload Process
When you run `pio run --target uploadfs`, PlatformIO will:
1. Create a LittleFS filesystem image from the `data/` folder
2. Upload this image to the ESP32's flash memory
3. The ESP32 can then serve these files directly from flash

### Troubleshooting
- If the web interface doesn't load, ensure LittleFS was uploaded successfully
- Check Serial Monitor for error messages
- Verify WiFi connection and IP address
- Make sure all files exist in the `data/` directory before uploading

## Motor Configuration
The code assumes a 4-wheel drive configuration:
- Motor A: Left Front
- Motor B: Right Front  
- Motor C: Left Rear
- Motor D: Right Rear

Turning is achieved by rotating wheels on opposite sides in different directions.
