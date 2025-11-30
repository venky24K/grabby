# Joystick & Control System - Technical Explanation

## Overview

The **Grabby** controller uses two virtual joysticks to independently control the robot's base and arm. Unlike traditional WebSocket-based systems, this application uses **Web Bluetooth (BLE)** to communicate directly with two separate ESP32 microcontrollers.

## System Architecture

### Dual BLE Connection
The app manages two concurrent Bluetooth Low Energy connections:
1.  **Base Controller (`MecanumRobot`)**: Handles the 4-wheel mecanum chassis.
2.  **Arm Controller (`MecanumArm`)**: Handles the 4-DOF robotic arm and gripper.

### 1. Base Joystick (Left)
**Function**: Omnidirectional Movement (Holonomic)
**Input**: 2D Vector (X, Y)

**Kinematics (Mecanum Wheels):**
The joystick input is converted into four motor speeds using standard mecanum kinematics:
-   **Front Left**:  `Vy + Vx`
-   **Front Right**: `Vy - Vx`
-   **Rear Left**:   `Vy - Vx`
-   **Rear Right**:  `Vy + Vx`

*Note: Rotation is handled separately via discrete "Rotate Left/Right" buttons to avoid accidental rotation during translation.*

**BLE Protocol (Base):**
-   **Service UUID**: `0000FFE0-0000-1000-8000-00805F9B34FB`
-   **Characteristic UUID**: `0000FFE1-0000-1000-8000-00805F9B34FB`
-   **Command Format**: `M:vx:vy`
    -   `vx`: X-axis velocity (-1.0 to 1.0)
    -   `vy`: Y-axis velocity (-1.0 to 1.0)
    -   Example: `M:0.5:-0.8` (Move forward-right)

### 2. Arm Joystick (Right)
**Function**: Joint Control (Shoulder & Elbow)
**Input**: 2D Vector (X, Y) mapped to Velocity

**Control Logic:**
Unlike the base, the arm joystick controls the **velocity** of the joints, not their absolute position. This allows for smooth, precise adjustments.

-   **X-Axis (Horizontal)**: Controls **Shoulder** joint.
    -   Right: Increase angle (Raise shoulder)
    -   Left: Decrease angle (Lower shoulder)
-   **Y-Axis (Vertical)**: Controls **Elbow** joint.
    -   Up: Increase angle (Extend elbow)
    -   Down: Decrease angle (Retract elbow)

**BLE Protocol (Arm):**
-   **Service UUID**: `0000FFE0-0000-1000-8000-00805F9B34FB`
-   **Characteristic UUID**: `0000FFE1-0000-1000-8000-00805F9B34FB`
-   **Command Format**: `A:base:shoulder:elbow:wrist:gripper`
    -   Sends absolute angles (0-180) for all servos.
    -   Example: `A:90:45:120:90:0`

## Component Implementation

### `Joystick` Component (`src/App.jsx`)
A reusable React component that handles touch and mouse logic.

**Visuals:**
-   **Base Joystick**: Cyan theme.
-   **Arm Joystick**: Purple theme.
-   **Feedback**: The inner knob follows the user's finger but is constrained within the outer ring radius.

**Touch Handling:**
-   Uses `touch-action: none` to prevent browser scrolling.
-   Supports multi-touch: Users can move the base and arm simultaneously.
-   **Auto-Center**: When released, the joystick snaps back to (0,0), sending a stop command.

## Safety Features

1.  **Connection Monitoring**:
    -   The app tracks connection state (`isConnected`).
    -   Joysticks are disabled (grayed out) if not connected.

2.  **Heartbeat / Keep-Alive**:
    -   The Base controller logic sends commands at **20Hz** (every 50ms).
    -   If the joystick is released, it sends a "Stop" command (`M:0:0`) repeatedly for a short burst to ensure the robot halts, then stops transmitting to save bandwidth.

3.  **Arm Limits**:
    -   Software limits are enforced in `App.jsx` to prevent servos from driving beyond physical stops (e.g., Shoulder 0-70°, Elbow 0-180°).

4.  **Force Sensor (FSR) Safety**:
    -   The firmware monitors the Force Sensitive Resistor (GPIO 34) on the gripper.
    -   If pressure exceeds the threshold (`FSR_THRESHOLD`), the gripper motor is prevented from closing further to avoid crushing objects.

## UI Layout Reference

The controller interface is divided into three main sections:

### 1. Left Section (Base Control)
-   **MOVE Joystick (Cyan)**: Controls the omnidirectional movement of the robot base.
    -   **Drag**: Move robot (Forward, Backward, Strafe Left/Right).
    -   **Release**: Stop robot immediately.

### 2. Center Section (Settings & Fine Control)
-   **Top Bar**:
    -   **Gamepad Icon**: Connect/Disconnect Base BLE.
    -   **Home Icon**: Reset Arm to "Home" position.
    -   **Speed Slider (Zap)**: Adjust global movement speed limit.
    -   **Hand Icon**: Connect/Disconnect Arm BLE.
    -   **Fullscreen Icon**: Toggle fullscreen mode.
-   **Middle Sliders**:
    -   **Base Rotation**: Velocity control for rotating the base in place.
    -   **Wrist Angle**: Velocity control for rotating the wrist joint.
-   **Bottom Buttons**:
    -   **Rotate Left/Right**: Discrete buttons for precise base rotation.
    -   **OPEN / CLOSE**: Control the gripper state.

### 3. Right Section (Arm Control)
-   **ARM Joystick (Purple)**: Controls the main arm joints.
    -   **Horizontal (X)**: Shoulder Up/Down.
    -   **Vertical (Y)**: Elbow Extend/Retract.
    -   **Feedback**: Displays current servo angles (S:xxx E:xxx).




