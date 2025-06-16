@echo off
echo Testing ESP32 Car Stop Function
echo.

echo Testing basic connectivity...
curl -s "http://esp32car.local/status" > nul
if errorlevel 1 (
    echo ERROR: Cannot connect to ESP32 car
    echo Make sure the car is powered on and connected to WiFi
    pause
    exit /b 1
)
echo OK - Connected to ESP32 car
echo.

echo Testing stop command...
curl -s "http://esp32car.local/stop"
echo.
echo Stop command sent

echo.
echo Testing movement then stop sequence...
echo 1. Moving forward...
curl -s "http://esp32car.local/forward"
timeout /t 2 /nobreak > nul

echo 2. Stopping...
curl -s "http://esp32car.local/stop"
echo.

echo.
echo Test completed. Check if motors actually stopped.
echo If motors are still running, there may be a hardware issue.
pause
