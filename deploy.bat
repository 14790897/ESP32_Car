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
echo Opening serial monitor...
pio device monitor

pause
