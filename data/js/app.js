// Connection status tracking
let isConnected = true;
let commandQueue = [];

function updateConnectionStatus(status) {
  const statusElement = document.getElementById('connectionStatus');
  const statusContainer = statusElement.parentElement;
  
  statusContainer.className = `status ${status}`;
  
  switch(status) {
    case 'connected':
      statusElement.textContent = 'Connected';
      break;
    case 'connecting':
      statusElement.textContent = 'Connecting...';
      break;
    case 'disconnected':
      statusElement.textContent = 'Disconnected';
      break;
  }
}

function showButtonFeedback(button, success = true) {
  const originalTransform = button.style.transform;
  
  button.style.transform = 'scale(0.95)';
  button.style.filter = success ? 'brightness(1.2)' : 'brightness(0.8)';
  
  setTimeout(() => {
    button.style.transform = originalTransform;
    button.style.filter = '';
  }, 150);
}

function sendCommand(commandPath) {
  console.log("Sending command: " + commandPath);
  
  // Find the button that triggered this command
  const buttons = document.querySelectorAll('.control-button');
  let activeButton = null;
  
  buttons.forEach(button => {
    const onclick = button.getAttribute('onclick');
    if (onclick && onclick.includes(commandPath)) {
      activeButton = button;
    }
  });
  
  updateConnectionStatus('connecting');
  
  fetch(commandPath)
    .then(response => {
      updateConnectionStatus('connected');
      if (!response.ok) { 
        console.error('Command failed:', response.status);
        if (activeButton) showButtonFeedback(activeButton, false);
        throw new Error(`HTTP ${response.status}`);
      }
      if (activeButton) showButtonFeedback(activeButton, true);
      return response.text();
    })
    .then(text => {
      console.log('Server response:', text);
      // Show success animation
      if (activeButton) {
        activeButton.style.animation = 'pulse 0.5s ease';
        setTimeout(() => {
          activeButton.style.animation = '';
        }, 500);
      }
    })
    .catch(error => {
      console.error('Fetch error:', error);
      updateConnectionStatus('disconnected');
      if (activeButton) showButtonFeedback(activeButton, false);
      
      // Auto-retry connection after 2 seconds
      setTimeout(() => {
        updateConnectionStatus('connecting');
        // Retry the command
        fetch(commandPath).then(() => {
          updateConnectionStatus('connected');
        }).catch(() => {
          updateConnectionStatus('disconnected');
        });
      }, 2000);
    });
}

function sendSpeed(speedValue) {
  const speedDisplay = document.getElementById('speedValue');
  const slider = document.getElementById('speedSlider');
  
  speedDisplay.innerText = speedValue;
  
  // Update slider background based on speed
  const percentage = (speedValue / 255) * 100;
  slider.style.background = `linear-gradient(to right, #f0f0f0 0%, #4CAF50 ${percentage}%, #f0f0f0 ${percentage}%)`;
  
  const commandPath = '/speed?value=' + speedValue;
  console.log("Sending speed: " + commandPath);
  
  updateConnectionStatus('connecting');
  
  fetch(commandPath)
    .then(response => {
      updateConnectionStatus('connected');
      if (!response.ok) { 
        console.error('Speed command failed:', response.status);
        throw new Error(`HTTP ${response.status}`);
      }
      return response.text();
    })
    .then(text => {
      console.log('Server response:', text);
      // Add a subtle glow effect to the slider
      slider.style.boxShadow = '0 0 10px rgba(76, 175, 80, 0.5)';
      setTimeout(() => {
        slider.style.boxShadow = '';
      }, 300);
    })
    .catch(error => {
      console.error('Fetch error:', error);
      updateConnectionStatus('disconnected');
      
      // Auto-retry after 2 seconds
      setTimeout(() => {
        updateConnectionStatus('connecting');
        fetch(commandPath).then(() => {
          updateConnectionStatus('connected');
        }).catch(() => {
          updateConnectionStatus('disconnected');
        });
      }, 2000);
    });
}

// Enhanced keyboard controls with visual feedback
document.addEventListener('keydown', function(event) {
  // Prevent default only for our control keys
  const controlKeys = ['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight', ' ', 'w', 'W', 's', 'S', 'a', 'A', 'd', 'D'];
  
  if (!controlKeys.includes(event.key)) return;
  
  event.preventDefault();
  
  let command = '';
  let buttonClass = '';
  
  switch(event.key) {
    case 'ArrowUp':
    case 'w':
    case 'W':
      command = '/forward';
      buttonClass = 'forward';
      break;
    case 'ArrowDown':
    case 's':
    case 'S':
      command = '/backward';
      buttonClass = 'backward';
      break;
    case 'ArrowLeft':
    case 'a':
    case 'A':
      command = '/left';
      buttonClass = 'left';
      break;
    case 'ArrowRight':
    case 'd':
    case 'D':
      command = '/right';
      buttonClass = 'right';
      break;
    case ' ':
      command = '/stop';
      buttonClass = 'stop';
      break;
  }
  
  if (command) {
    // Find and highlight the corresponding button
    const button = document.querySelector(`.control-button.${buttonClass}`);
    if (button) {
      button.style.transform = 'scale(0.95)';
      button.style.filter = 'brightness(1.2)';
      setTimeout(() => {
        button.style.transform = '';
        button.style.filter = '';
      }, 150);
    }
    
    sendCommand(command);
  }
});

// Prevent context menu on long press for mobile
document.addEventListener('contextmenu', function(event) {
  if (event.target.closest('.control-button')) {
    event.preventDefault();
  }
});

// Touch feedback for mobile devices
document.addEventListener('touchstart', function(event) {
  if (event.target.closest('.control-button')) {
    event.target.style.transform = 'scale(0.95)';
  }
});

document.addEventListener('touchend', function(event) {
  if (event.target.closest('.control-button')) {
    setTimeout(() => {
      event.target.style.transform = '';
    }, 150);
  }
});

// Initialize on page load
document.addEventListener('DOMContentLoaded', function() {
  const slider = document.getElementById('speedSlider');
  const speedValue = document.getElementById('speedValue');
  
  // Set initial display value and slider background
  speedValue.innerText = slider.value;
  const initialPercentage = (slider.value / 255) * 100;
  slider.style.background = `linear-gradient(to right, #f0f0f0 0%, #4CAF50 ${initialPercentage}%, #f0f0f0 ${initialPercentage}%)`;
  
  // Update slider background on input
  slider.addEventListener('input', function() {
    const percentage = (this.value / 255) * 100;
    this.style.background = `linear-gradient(to right, #f0f0f0 0%, #4CAF50 ${percentage}%, #f0f0f0 ${percentage}%)`;
    speedValue.innerText = this.value;
  });
  
  // Initial connection check
  fetch('/status')
    .then(() => updateConnectionStatus('connected'))
    .catch(() => updateConnectionStatus('disconnected'));
  
  console.log('ESP32 Car Control Interface Loaded');
  console.log('Keyboard Controls:');
  console.log('- WASD or Arrow Keys for movement');
  console.log('- Spacebar to stop');
});
