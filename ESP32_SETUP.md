# ESP32 Setup Guide

This guide explains how to set up and upload the ESP32 code for both the base and arm controllers using **Bluetooth Low Energy (BLE)**.

## Prerequisites

1. **Arduino IDE** (or PlatformIO)
2. **ESP32 Board Support** installed in Arduino IDE
3. **Required Libraries**:
   - **ESP32Servo** (for arm controller)
   - *Note: BLE libraries are built-in to the ESP32 Arduino Core.*

## Installing Libraries

### In Arduino IDE:
1. Go to **Tools → Manage Libraries**
2. Search and install:
   - **ESP32Servo** by Kevin Harrington / John K. Bennett

## Hardware Configuration

### Base ESP32 (Mecanum Wheels)

**Motor Driver Setup:**
- Uses PWM + Direction pins for each motor
- Default pins (adjust in code if needed):
  - Front Left:  GPIO 16 (IN1), 17 (IN2), 4 (EN)
  - Front Right: GPIO 27 (IN1), 26 (IN2), 25 (EN)
  - Rear Left:   GPIO 22 (IN1), 21 (IN2), 32 (EN)
  - Rear Right:  GPIO 19 (IN1), 18 (IN2), 5 (EN)

**Motor Driver Options:**
- L298N, L293D, or similar dual H-bridge drivers
- Adjust pin assignments in `esp32_base.ino` based on your setup

### Arm ESP32 (4 DOF Arm)

**Servo Configuration:**
- Default pins (adjust in code if needed):
  - Base:     GPIO 2
  - Shoulder: GPIO 4
  - Elbow:    GPIO 5
  - Wrist:    GPIO 18
  - Gripper:  GPIO 19
  - FSR (Pressure Sensor): GPIO 34

**Servo Requirements:**
- Standard 180° servos (SG90, MG996R, etc.)
- 5V power supply (external recommended for multiple servos)
- PWM frequency: 50Hz (default)

## Uploading Code

### Base Controller:
1. Open `esp32_base.ino` in Arduino IDE
2. Select board: **Tools → Board → ESP32 Dev Module**
3. Select port: **Tools → Port → [Your ESP32 COM port]**
4. Click **Upload**
5. Open Serial Monitor (115200 baud) and check for: `BLE Ready! Waiting for connections...`

### Arm Controller:
1. Open `esp32_arm.ino` in Arduino IDE
2. Select board: **Tools → Board → ESP32 Dev Module**
3. Select port: **Tools → Port → [Your ESP32 COM port]**
4. Click **Upload**
5. Open Serial Monitor (115200 baud) and check for: `BLE Arm Controller Ready!`

## BLE Configuration

The system uses two separate BLE services. No manual WiFi configuration is needed.

### Device Names:
- **Base Controller**: `MecanumRobot`
- **Arm Controller**: `MecanumArm`

### UUIDs (Hardcoded):
- **Service UUID**: `0000FFE0-0000-1000-8000-00805F9B34FB`
- **Characteristic UUID**: `0000FFE1-0000-1000-8000-00805F9B34FB`

*Note: These UUIDs match standard HM-10 BLE module defaults for compatibility.*

## Testing

1. **Power on** both ESP32s.
2. **Open the React App** in a Chrome-based browser (Chrome, Edge, Opera) or Bluefy on iOS.
   - *Note: Firefox and Safari (on macOS) do not support Web Bluetooth.*
3. Click the **Gamepad Icon** (Left) to connect to the **Base**.
   - Select `MecanumRobot` from the pairing list.
4. Click the **Hand Icon** (Right) to connect to the **Arm**.
   - Select `MecanumArm` from the pairing list.
5. Icons will turn colored (Green/Purple) when connected.

## Troubleshooting

### Connection Issues:
- **Device not found**: Ensure ESP32 is powered and Serial Monitor shows "BLE Ready".
- **"Pairing failed"**: Try moving closer to the device or restarting the ESP32.
- **Browser Support**: Ensure you are using a browser with Web Bluetooth support (Chrome/Edge).
- **Permissions**: Grant Bluetooth permissions if prompted by the browser/OS.

### Base Controller:
- **Motors not moving**: Check motor driver connections and power supply (12V/9V usually required for motors).
- **Wrong direction**: Swap motor wires or change logic in `esp32_base.ino`.

### Arm Controller:
- **Servos not moving**: Check 5V power supply. ESP32 3.3V pin cannot drive multiple servos.
- **Brownout/Reset**: If ESP32 restarts when servos move, your power supply is insufficient. Use a separate 5V 2A+ supply for servos.

## Customization

### Adjusting Motor Speeds:
In `esp32_base.ino`, modify PWM settings:
```cpp
const int PWM_FREQ = 1000;        // Frequency
const int PWM_RESOLUTION = 8;     // 8-bit = 0-255
```

### Adjusting Servo Range:
In `esp32_arm.ino`, modify servo attachment:
```cpp
servoBase.attach(SERVO_BASE, 500, 2500); // min/max pulse width in microseconds
```

