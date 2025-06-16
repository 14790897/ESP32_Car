#!/bin/bash

echo "==========================================="
echo "ESP32 Car mDNS Test"
echo "==========================================="
echo

echo "Testing mDNS connectivity..."
echo

echo "1. Testing mDNS resolution..."
if command -v avahi-resolve &> /dev/null; then
    avahi-resolve -n esp32car.local
elif command -v dig &> /dev/null; then
    dig @224.0.0.251 -p 5353 esp32car.local
else
    echo "No mDNS tools found, trying ping..."
    ping -c 4 esp32car.local
fi

echo
echo "2. Testing HTTP connection..."
if command -v curl &> /dev/null; then
    curl -s -o /dev/null -w "HTTP Status: %{http_code}\nResponse Time: %{time_total}s\n" http://esp32car.local/status
    if [ $? -eq 0 ]; then
        echo "  ✓ HTTP connection successful"
    else
        echo "  ✗ HTTP connection failed"
    fi
else
    echo "curl not found, trying wget..."
    wget -q --spider http://esp32car.local/status
    if [ $? -eq 0 ]; then
        echo "  ✓ HTTP connection successful"
    else
        echo "  ✗ HTTP connection failed"
    fi
fi

echo
echo "3. Getting device status..."
if command -v curl &> /dev/null; then
    echo "Device information:"
    curl -s http://esp32car.local/status | python3 -m json.tool 2>/dev/null || curl -s http://esp32car.local/status
fi

echo
echo "4. Opening web interface..."
if command -v xdg-open &> /dev/null; then
    xdg-open http://esp32car.local
elif command -v open &> /dev/null; then
    open http://esp32car.local
else
    echo "Please open http://esp32car.local manually in your browser"
fi

echo
echo "==========================================="
echo "Test completed"
echo "==========================================="
