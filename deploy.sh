#!/bin/bash

echo "==========================================="
echo "ESP32 Car Project Deployment Script"
echo "==========================================="
echo

echo "Step 1: Building project..."
pio run
if [ $? -ne 0 ]; then
    echo "Build failed! Please fix compilation errors."
    exit 1
fi

echo
echo "Step 2: Uploading LittleFS filesystem..."
pio run --target uploadfs
if [ $? -ne 0 ]; then
    echo "Filesystem upload failed!"
    exit 1
fi

echo
echo "Step 3: Uploading firmware..."
pio run --target upload
if [ $? -ne 0 ]; then
    echo "Firmware upload failed!"
    exit 1
fi

echo
echo "==========================================="
echo "Deployment completed successfully!"
echo "==========================================="
echo

echo "Opening serial monitor..."
pio device monitor
