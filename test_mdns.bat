@echo off
echo ===========================================
echo ESP32 Car mDNS Test
echo ===========================================
echo.

echo Testing mDNS connectivity...
echo.

echo 1. Pinging esp32car.local...
ping -n 4 esp32car.local
echo.

echo 2. Testing HTTP connection...
curl -s -o nul -w "HTTP Status: %%{http_code}\nResponse Time: %%{time_total}s\n" http://esp32car.local/status
if %errorlevel% equ 0 (
    echo   ✓ HTTP connection successful
) else (
    echo   ✗ HTTP connection failed
    echo.
    echo Troubleshooting tips:
    echo - Make sure ESP32 is powered on and connected to WiFi
    echo - Check if Bonjour service is running on Windows
    echo - Try accessing via IP address instead
)

echo.
echo 3. Opening web interface...
start http://esp32car.local

echo.
echo ===========================================
echo Test completed
echo ===========================================
pause
