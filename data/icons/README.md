# SVG Icons for ESP32 Car Control

This directory contains vector graphics icons for the ESP32 Car web interface.

## Icon Files

- **forward.svg** - Green gradient forward arrow for forward movement
- **backward.svg** - Orange gradient backward arrow for reverse movement  
- **left.svg** - Blue gradient left arrow for left turns
- **right.svg** - Purple gradient right arrow for right turns
- **stop.svg** - Red gradient stop square for emergency stop
- **speed.svg** - Gray gradient speedometer for speed control
- **car.svg** - Car logo/icon for the header
- **wifi.svg** - WiFi signal indicator for connection status

## Features

All icons are:
- **Vector-based**: Scalable without quality loss
- **Responsive**: Work well on different screen sizes
- **Color-coded**: Each function has its own color theme
- **Gradient-enhanced**: Modern gradient effects for visual appeal
- **Lightweight**: Small file sizes for fast loading

## Usage

The icons are automatically loaded by the web interface when you access the ESP32's IP address. They provide visual feedback and enhance the user experience with:

- Color-coded buttons for different functions
- Hover effects that match the icon colors
- Keyboard control indicators
- Connection status visualization

## Customization

To customize the icons:
1. Edit the SVG files with any text editor or vector graphics software
2. Modify colors by changing the gradient stop colors
3. Upload the modified files using `pio run --target uploadfs`

Example color modification:
```xml
<!-- Change this -->
<stop offset="0%" style="stop-color:#4CAF50;stop-opacity:1" />
<!-- To this for a different color -->
<stop offset="0%" style="stop-color:#FF5722;stop-opacity:1" />
```
