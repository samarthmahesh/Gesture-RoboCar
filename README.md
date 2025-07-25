


# 🖐️➡️🚗 Gesture Controlled RC Car with MediaPipe & ESP32

![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![OpenCV](https://img.shields.io/badge/OpenCV-5C3EE8?style=for-the-badge&logo=opencv&logoColor=white)
![MediaPipe](https://img.shields.io/badge/MediaPipe-007F7F?style=for-the-badge&logo=google&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)

-Samarth Mahesh

---

A robot car using nothing but hand gestures? This project does just that—your webcam sees your hand (input), interprets which fingers are out using *mediapipe*, and sends a command (wirelessly!) using *USP Wifi* to an *ESP32* microcontroller that moves the car accordingly.

---
## Folder Structure

<img width="983" height="218" alt="image" src="https://github.com/user-attachments/assets/c00054e0-b6ae-4c57-b643-f10f7db7ac6f" />


---
## ✨ Features

- **Live Gesture Control:** A Python app on your computer uses a webcam and MediaPipe to figure out how many fingers are up, then matches that to a movement command.
- **No Wires Needed:** The ESP32 becomes a WiFi hotspot. Your computer connects directly, so the system works even outdoors—no router required.
- **Simple Gesture Map:**  
  | Fingers        | Car Action    |
  |:--------------:|:-------------|
  | Fist (0)       | Stop         |
  | 1              | Forward      |
  | 2              | Backward     |
  | 3              | Turn Left    |
  | 4              | Turn Right   |

---

## ⚙️ How the System Comes Together

There are two main parts:

### 1. Your Computer (Python Controller)
- Runs `input_fingers.py`.
- Captures webcam video.
- MediaPipe analyzes the hand and counts extended fingers.
- The script sends this count by UDP WiFi to the ESP32.

### 2. The Car (ESP32 + Motors)
- Runs `car_code.ino` (Arduino sketch).
- ESP32 acts as a WiFi hotspot with its own SSID and password.
- Listens for UDP packets (on port 4210).
- Matches the finger count to a motion, then signals the motor driver to move the car.

---

## 🛠️ Technology Stack

- **Vision & Control:** Python with OpenCV and MediaPipe
- **On-Car Logic:** C++ via Arduino IDE for ESP32
- **Hardware:** ESP32, motor driver (TB6612FNG/L298N), DC motors
- **Comms:** WiFi UDP

---

## 🛒 What You Need

- ESP32 development board
- Car chassis (2 or 4 motors)
- Motor driver compatible with ESP32 (e.g., L298N)
- Power for ESP32 + motors (LiPo, power bank, or similar)
- Computer with webcam

---

## 🔌 Hardware Setup (ESP32 + Car)

- **Connect Motor Driver Inputs:**
  - Motor A: `AIN1` (GPIO 26), `AIN2` (GPIO 25), `PWMA` (GPIO 33)
  - Motor B: `BIN1` (GPIO 14), `BIN2` (GPIO 12), `PWMB` (GPIO 13)
  - Standby: `STDBY` (GPIO 27)
- Wire outputs from the driver to your motors and ensure common GND.

---

## 💻 Software Setup

### Arduino/ESP32

1. Install Arduino IDE & add ESP32 support.
2. Load `car_code.ino` and set your desired WiFi SSID/PASSWORD (optional).
3. Upload the sketch and open the serial monitor (9600 baud). The ESP32 should broadcast its AP (default IP: 192.168.4.1).

### Python Controller

1. Clone your repo on the computer you’ll use for control.
2. Install dependencies:
    ```
    pip install opencv-python mediapipe
    ```
3. Connect your computer to the ESP32's WiFi.
4. Run:
    ```
    python input_fingers.py
    ```
5. You’ll see your webcam video. Raise your hand—fist stops, one finger forward, and so on!

---

## 📂 Project Layout

<img width="958" height="105" alt="image" src="https://github.com/user-attachments/assets/3403984c-63e1-4267-8de1-8bde63b83edb" />


text

---

## 📝 Tips & Notes

- Make sure both ESP32 and the controlling computer have sufficient power and share the same WiFi network.
- Lighting can affect gesture recognition—ensure your hand is visible and well-lit.
- If the car does not respond, check serial logs on the ESP32 and confirm correct IP in the Python script.

---

## 📈 Possible Expansions

- Add more gestures for advanced controls
- Implement feedback from the car (e.g., acknowledge commands)
- Stream live camera feed from the car using ESP32-CAM module
- Replace hand gestures with face or color tracking

---


