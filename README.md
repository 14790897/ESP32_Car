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

1. Open Serial Monitor to see the ESP32's IP address
2. Connect to the same WiFi network as the ESP32
3. Open a web browser and navigate to the ESP32's IP address
4. Use the web interface to control the car:
   - Buttons for Forward/Backward/Left/Right/Stop
   - Speed slider (0-255)
   - Keyboard controls (WASD/Arrow keys/Spacebar)

## API Endpoints

- `GET /` - Main web interface
- `GET /forward` - Move forward
- `GET /backward` - Move backward  
- `GET /left` - Turn left
- `GET /right` - Turn right
- `GET /stop` - Stop all motors
- `GET /speed?value=X` - Set speed (X = 0-255)

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
