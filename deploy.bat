@echo off
echo ===========================================
echo ESP32 Car Project Deployment Script
echo ===========================================
echo.

echo Step 0: Checking data files...
call check_data.bat
echo.

echo Step 1: Building project...
pio run
if %errorlevel% neq 0 (
    echo Build failed! Please fix compilation errors.
    pause
    exit /b 1
)

echo.
echo Step 2: Uploading LittleFS filesystem...
echo This includes HTML, CSS, JS, and SVG icon files...
pio run --target uploadfs
if %errorlevel% neq 0 (
    echo Filesystem upload failed!
    echo Make sure the ESP32 is connected and no serial monitor is open.
    pause
    exit /b 1
)

echo.
echo Step 3: Uploading firmware...
pio run --target upload
if %errorlevel% neq 0 (
    echo Firmware upload failed!
    pause
    exit /b 1
)

echo.
echo ===========================================
echo Deployment completed successfully!
echo ===========================================
echo.
echo Your ESP32 Car is now accessible via:
echo   1. mDNS: http://esp32car.local
echo   2. IP Address: (check serial monitor)
echo.
echo Note: For mDNS to work on Windows, you may need
echo Bonjour service (installed with iTunes).
echo.
echo Opening serial monitor...
pio device monitor

pause
