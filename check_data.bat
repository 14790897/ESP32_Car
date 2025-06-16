@echo off
echo ===========================================
echo ESP32 Car Project - LittleFS Data Check
echo ===========================================
echo.

echo Checking LittleFS data directory structure...
echo.

if exist "data" (
    echo ✓ data/ directory exists
    echo.
    echo Data directory contents:
    dir /s /b data
    echo.
    
    echo Checking for required files:
    if exist "data\index.html" (
        echo   ✓ data\index.html exists
        for %%A in ("data\index.html") do echo     Size: %%~zA bytes
    ) else (
        echo   ✗ data\index.html missing
    )

    if exist "data\css\style.css" (
        echo   ✓ data\css\style.css exists
        for %%A in ("data\css\style.css") do echo     Size: %%~zA bytes
    ) else (
        echo   ✗ data\css\style.css missing
    )

    if exist "data\js\app.js" (
        echo   ✓ data\js\app.js exists
        for %%A in ("data\js\app.js") do echo     Size: %%~zA bytes
    ) else (
        echo   ✗ data\js\app.js missing
    )    if exist "data\favicon.ico" (
        echo   ✓ data\favicon.ico exists
        for %%A in ("data\favicon.ico") do echo     Size: %%~zA bytes
    ) else (
        echo   ✗ data\favicon.ico missing
    )

    if exist "data\favicon.svg" (
        echo   ✓ data\favicon.svg exists
        for %%A in ("data\favicon.svg") do echo     Size: %%~zA bytes
    ) else (
        echo   ✗ data\favicon.svg missing
    )
    
    if exist "data\icons" (
        echo   ✓ data\icons/ directory exists
        echo   Icon files:
        if exist "data\icons\forward.svg" (echo     ✓ forward.svg) else (echo     ✗ forward.svg missing)
        if exist "data\icons\backward.svg" (echo     ✓ backward.svg) else (echo     ✗ backward.svg missing)
        if exist "data\icons\left.svg" (echo     ✓ left.svg) else (echo     ✗ left.svg missing)
        if exist "data\icons\right.svg" (echo     ✓ right.svg) else (echo     ✗ right.svg missing)
        if exist "data\icons\stop.svg" (echo     ✓ stop.svg) else (echo     ✗ stop.svg missing)
        if exist "data\icons\speed.svg" (echo     ✓ speed.svg) else (echo     ✗ speed.svg missing)
        if exist "data\icons\car.svg" (echo     ✓ car.svg) else (echo     ✗ car.svg missing)
        if exist "data\icons\wifi.svg" (echo     ✓ wifi.svg) else (echo     ✗ wifi.svg missing)
    ) else (
        echo   ✗ data\icons/ directory missing
    )
    
) else (
    echo ✗ data/ directory does not exist
    echo Please create it with: mkdir data
)

echo.
echo ===========================================
echo Next steps:
echo ===========================================
echo 1. To upload filesystem: pio run --target uploadfs
echo 2. To build and upload:   deploy.bat
echo 3. To monitor serial:     pio device monitor
echo 4. To test mDNS:         test_mdns.bat
echo.
echo After deployment, access your car at:
echo   http://esp32car.local
echo.

pause
