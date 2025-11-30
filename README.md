# Grabby 🤖🖐️

**Grabby** is a custom-built mobile manipulator robot designed for the **LAM Research Challenge 2025**. This repository contains the **official controller application**, which provides a seamless, high-performance interface for operating Grabby using **Web Bluetooth (BLE)** technology directly from your browser.

![Grabby Banner](https://github.com/venky24K/grabby/assets/placeholder.png)

## 🚀 Overview

This application bridges the gap between web technologies and hardware control, turning any smartphone or tablet into a professional-grade controller for Grabby. Built with **React** and **Vite**, it ensures low-latency communication for precise operation.

Grabby consists of two distinct hardware modules controlled by this app:
1.  **Mecanum Wheel Base**: Omnidirectional movement with precision velocity control.
2.  **4-DOF Robotic Arm**: Inverse kinematics and joint-level control for complex manipulation tasks.

## ✨ Key Features

-   **📱 Web Bluetooth Integration**: Connect directly to ESP32 microcontrollers without installing native apps.
-   **🎮 Dual Virtual Joysticks**:
    -   **Left Stick**: Holonomic control for the Mecanum base (Forward/Back/Strafe).
    -   **Right Stick**: Intuitive control for the robotic arm (Shoulder & Elbow).
-   **⚡ Precision Sliders**: Fine-tune base rotation speed and wrist angles.
-   **🦾 Smart Arm Control**:
    -   **Homing Sequence**: One-tap reset to a safe "Home" position.
    -   **Gripper Logic**: Safety-aware open/close mechanism with pressure sensing support.
-   **🎨 Modern UI/UX**:
    -   Dark mode aesthetic with Cyberpunk/Sci-Fi vibes.
    -   **Landscape Lock**: Enforced orientation for optimal control.
    -   **Fullscreen Mode**: Immersive experience free from browser distractions.
    -   **Multi-touch Support**: Control base and arm simultaneously.

## 🛠️ Tech Stack

### Frontend
-   **Framework**: [React 19](https://react.dev/)
-   **Build Tool**: [Vite](https://vitejs.dev/)
-   **Styling**: [Tailwind CSS 4](https://tailwindcss.com/)
-   **Icons**: [Lucide React](https://lucide.dev/)

### Firmware / Hardware
-   **Microcontroller**: ESP32 (x2)
-   **Communication**: Bluetooth Low Energy (BLE)
-   **Libraries**: `ESP32Servo`, `BLEDevice`

## 📦 Installation

### 1. Firmware Setup
Before running the app, you need to flash the ESP32s.
👉 **[Read the ESP32 Setup Guide](./ESP32_SETUP.md)** for detailed wiring and upload instructions.

### 2. Frontend Setup
Clone the repository and install dependencies:

```bash
git clone https://github.com/venky24K/grabby.git
cd grabby
npm install
```

### 3. Running Locally
Start the development server:

```bash
npm run dev
```

Open your browser (Chrome/Edge recommended) and navigate to the local URL (usually `http://localhost:5173`).

## 🎮 Usage Guide

1.  **Power On**: Turn on your robot. Ensure both ESP32s are running.
2.  **Open App**: Launch Grabby on your mobile device or laptop.
3.  **Connect Base**: Tap the **Gamepad Icon** (Left). Select `MecanumRobot` from the list.
4.  **Connect Arm**: Tap the **Hand Icon** (Right). Select `MecanumArm` from the list.
5.  **Control**:
    -   Use the **Left Joystick** to move the robot.
    -   Use the **Right Joystick** to move the arm.
    -   Use the **Sliders** for rotation and wrist control.
    -   Use the **Open/Close** buttons for the gripper.

## 🏆 LAM Research Challenge 2025

This project was developed as a submission for the LAM Research Challenge 2025, demonstrating the potential of web-based robotics control systems.

## 📄 License

MIT License. See [LICENSE](LICENSE) for details.

---
*Built with ❤️ by [Venky](https://github.com/venky24K)*
