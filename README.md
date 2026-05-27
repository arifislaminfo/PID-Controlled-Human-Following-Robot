<div align="center">

# 🤖 PID Controlled Human Following Robot

[![Python](https://img.shields.io/badge/Python-3.12-blue?style=for-the-badge&logo=python)](https://python.org)
[![Arduino](https://img.shields.io/badge/Arduino-IDE-teal?style=for-the-badge&logo=arduino)](https://arduino.cc)
[![ESP32](https://img.shields.io/badge/ESP32-WiFi-orange?style=for-the-badge)](https://espressif.com)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.13-green?style=for-the-badge&logo=opencv)](https://opencv.org)
[![MediaPipe](https://img.shields.io/badge/MediaPipe-0.10.13-red?style=for-the-badge)](https://mediapipe.dev)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)

**A wireless autonomous robot that detects and follows a human in real time using computer vision and PID control.**

[Features](#-features) • [Demo](#-demo) • [Hardware](#-hardware-required) • [Installation](#-installation) • [How It Works](#-how-it-works) • [Contributing](#-contributing)

</div>

---

## 📸 Demo

> Robot detecting human and following wirelessly via WiFi
Camera sees person → MediaPipe detects body → PID calculates speed → ESP32 sends via WiFi → Robot follows smoothly!

---

## ✨ Features

- 🎯 **Real-time human detection** using Google MediaPipe at ~30 FPS
- 📡 **Fully wireless** — ESP32 WiFi communication, no cables!
- 🧠 **PID smooth control** — proportional speed based on error
- 📏 **Distance management** — stops when too close, follows when far
- 🔄 **Auto reconnect** — Python retries connection automatically
- 💡 **Beginner friendly** — well documented code for learning

---

## 🛠 Hardware Required

| Component | Purpose | Quantity |
|-----------|---------|----------|
| Arduino Uno | Motor control logic | 1 |
| ESP32 NodeMCU (CP2102) | WiFi communication | 1 |
| L298N Motor Driver | Powers DC motors | 1 |
| 4WD Robot Chassis | Movement platform | 1 |
| 18650 Battery (3.7V) | Motor power supply | 2 |
| Battery Holder (2S) | Holds batteries | 1 |
| USB Power Bank | ESP32 stable power | 1 |
| Jumper Wires | Connections | 1 pack |

---

## 💻 Software Required

- Python 3.12
- Arduino IDE 2.3.8
- VS Code (recommended)

---

## 🔌 Wiring Diagram
LAPTOP (Python + OpenCV + MediaPipe)
|
| WiFi (Socket Port 8888)
|
ESP32 (GPIO 17=TX, GPIO 16=RX)
|
| UART (9600 baud)
|
Arduino Uno
|
| PWM Signals
|
L298N Motor Driver
|           |
Left Motors  Right Motors
(OUT1,OUT2)  (OUT3,OUT4)
Arduino Pin Connections:
IN1 → Pin 6    IN2 → Pin 7
IN3 → Pin 8    IN4 → Pin 9
ENA → Pin 5    ENB → Pin 10
Power:
18650 x2 (7.4V) → L298N +12V, GND
L298N 5V → Arduino 5V
Power Bank → ESP32 USB

---

## 📦 Installation

### Step 1 — Clone the repository
```bash
git clone https://github.com/YOUR_USERNAME/human-following-robot.git
cd human-following-robot
```

### Step 2 — Install Python libraries
```bash
pip install opencv-python
pip install mediapipe==0.10.13
pip install pyserial
```

### Step 3 — Install CP2102 Driver (for ESP32)
Download from: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

### Step 4 — Add ESP32 support to Arduino IDE
Add this URL in Arduino IDE → File → Preferences → Additional Boards Manager URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
Then install **esp32 by Espressif Systems** from Boards Manager.

---

## ⚙️ Configuration

### ESP32 Code (`arduino_code/esp32_wifi_server/esp32_wifi_server.ino`)
Update your WiFi credentials:
```cpp
const char* ssid = "YOUR_HOTSPOT_NAME";
const char* password = "YOUR_PASSWORD";

// Update static IP to match your network
IPAddress local_IP(10, 94, 77, 100);
IPAddress gateway(10, 94, 77, 52);
```

### Python Code (`python_code/robot_control.py`)
Update ESP32 IP:
```python
ESP32_IP = "10.94.77.100"  # Change to your ESP32 IP
ESP32_PORT = 8888
```

### PID Tuning (`python_code/robot_control.py`)
```python
KP = 0.5   # Proportional gain
KI = 0.01  # Integral gain  
KD = 0.1   # Derivative gain
BASE_SPEED = 150  # Base motor speed (0-255)
```

---

## 🚀 How To Run

### Every time you start the robot:

**Step 1** — Turn on phone hotspot (name and password as configured)

**Step 2** — Power on ESP32 via power bank, press reset button

**Step 3** — Wait 15 seconds for WiFi connection

**Step 4** — Connect laptop to same hotspot

**Step 5** — Upload Arduino code (done once)

**Step 6** — Run Python:
```bash
python python_code/robot_control.py
```

**Step 7** — Turn on battery switch → Robot follows you! 🤖

---

## 🧠 How It Works

### Human Detection
MediaPipe detects 33 body landmarks
→ Nose (landmark 0) → direction left/right
→ Shoulders (landmark 11,12) → distance estimation

### Zone Detection
Screen width = 640px divided into 3 zones:
[0 - 213px]    = LEFT  → Turn Left
[213 - 426px]  = CENTER → Go Forward
[426 - 640px]  = RIGHT → Turn Right

### Distance Control
Shoulder width > 200px → TOO CLOSE → Stop
Shoulder width < 100px → TOO FAR → Forward
100px - 200px → GOOD DISTANCE → Follow direction

### PID Control
error = nose_position - screen_center
P = KP × error          (current error)
I = KI × Σerror         (accumulated error)
D = KD × Δerror         (rate of change)
speed = P + I + D → mapped to 80-255 PWM

---

## 📁 Project Structure
human-following-robot/
│
├── python_code/
│   ├── robot_control.py      # Main robot control with PID
│   ├── detection_logic.py    # Zone and distance detection
│   ├── test_camera.py        # Camera test
│   ├── test_mediapipe.py     # MediaPipe test
│   └── test_arduino.py       # Arduino communication test
│
├── arduino_code/
│   ├── esp32_wifi_server/
│   │   └── esp32_wifi_server.ino   # ESP32 WiFi server code
│   └── arduino_motor_control/
│       └── arduino_motor_control.ino  # Arduino motor code
│
├── docs/
│   ├── IEEE_Report.docx      # Full IEEE format report
│   └── Presentation.pptx     # Project presentation
│
├── README.md
└── LICENSE

---

## 🔧 Troubleshooting

| Problem | Solution |
|---------|----------|
| Camera not opening | Check if another app is using camera |
| MediaPipe error | Run `pip install mediapipe==0.10.13` |
| ESP32 not detected | Install CP2102 driver, restart laptop |
| WiFi not connecting | Use 2.4GHz hotspot, no spaces in name |
| Python can't connect to ESP32 | Check same network, update IP address |
| Motors not moving | Check battery switch is ON, check L298N wiring |
| Camera lagging | Already fixed — 0.3s command interval |

---

## 🤝 Contributing

Contributions are welcome! Here are some ideas:

- 📷 Mount camera on robot for true autonomous following
- 🚧 Add obstacle avoidance with ultrasonic sensors
- 🍓 Replace laptop with Raspberry Pi for standalone operation
- 👤 Add person re-identification to follow specific person
- 📊 Implement Ziegler-Nichols PID tuning
- 🌙 Improve detection in low light conditions

**How to contribute:**
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📝 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgements

- [Google MediaPipe](https://mediapipe.dev) for human pose detection
- [OpenCV](https://opencv.org) for computer vision
- [Espressif](https://espressif.com) for ESP32
- [Arduino](https://arduino.cc) for microcontroller platform

---

<div align="center">

**⭐ If you found this helpful, please star the repository! ⭐**

Made with ❤️ by Arif

</div>