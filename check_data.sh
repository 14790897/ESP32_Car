#!/bin/bash

echo "Checking LittleFS data directory structure..."
echo

echo "Data directory contents:"
find data -type f -exec echo "  {}" \;

echo
echo "File sizes:"
find data -type f -exec ls -lh {} \; | awk '{print "  " $9 ": " $5}'

echo
echo "Total size:"
du -sh data

echo
echo "Checking for required files:"
required_files=("data/index.html" "data/css/style.css" "data/js/app.js")

for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "  ✓ $file exists"
    else
        echo "  ✗ $file missing"
    fi
done

echo
echo "LittleFS data validation complete!"
