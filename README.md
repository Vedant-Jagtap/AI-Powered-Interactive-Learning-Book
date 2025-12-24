# AI-Powered-Interactive-Learning-Book

# 🎓 AI-Powered Assistive Learning Assistant

![Project Status](https://img.shields.io/badge/Status-Prototype-green)
![Competition](https://img.shields.io/badge/Competition-DIPEX_2026-blue)
![Tech Stack](https://img.shields.io/badge/IoT-ESP32%20%7C%20Gemini%20AI%20%7C%20Flask-orange)

## 📖 Overview
The **AI-Powered Assistive Learning Assistant** is a dual-node IoT device designed to help **visually impaired students** and **early learners** interact with the physical world independently. 

Unlike traditional static tools, this system uses **Generative AI (Google Gemini Pro Vision)** to "see" and describe objects in real-time, alongside an **RFID-based tactile learning mode** for structured lessons.

## 🚀 Key Features
* **Generative AI Vision:** Captures images via ESP32-CAM and generates descriptive audio using Google Gemini API.
* **Dual-Node Architecture:** Separates Vision (Camera) and Interaction (Audio/RFID) tasks using **ESP-NOW** for low-latency, wireless communication.
* **Smart Flashlight:** Auto-flash mechanism for clear image capture in low light.
* **RFID Learning Mode:** Scans physical tags to play pre-recorded lessons for alphabet/number learning.
* **Portable Design:** Battery-powered using LiPo cells with TP4056 charging protection.

---

## 🛠️ Hardware Architecture

The system consists of two independent wireless nodes:

### 1. Vision Node (The "Eye")
* **Controller:** ESP32-CAM (AI-Thinker)
* **Power:** LiPo Battery via TP4056 & MT3608/134N3P Booster
* **Function:** Captures images, controls flashlight, communicates with Python Backend.

### 2. Interaction Node (The "Voice")
* **Controller:** ESP32 WROOM (Dev Module)
* **Peripherals:** * **DFPlayer Mini** (MP3 Audio Output)
    * **MFRC522** (RFID Reader)
    * **Speaker** (4 Ohm, 3W)
* **Function:** Receives AI text via ESP-NOW, plays TTS audio, handles RFID input.

---

## 💻 Tech Stack
* **Firmware:** C++ (Arduino IDE)
* **Backend Server:** Python (Flask)
* **AI Model:** Google Gemini Pro Vision (via API)
* **Communication Protocols:** * **HTTP/REST:** Image transfer (ESP32 -> Laptop Server)
    * **ESP-NOW:** Peer-to-Peer data (ESP32 -> ESP32)
    * **UART:** DFPlayer Control
    * **SPI:** RFID Communication

---

## 🔌 Circuit & Wiring

### Vision Node (ESP32-CAM)
| Component Pin | ESP32-CAM Pin | Description |
| :--- | :--- | :--- |
| Flashlight LED | GPIO 4 | High-intensity LED control |
| Status LED | GPIO 33 | Connection status indicator |
| Button | GPIO 13 | Trigger for capture |

### Interaction Node (ESP32 WROOM)
| Component | Pin | ESP32 Pin |
| :--- | :--- | :--- |
| **DFPlayer** | TX | GPIO 16 (RX2) |
| **DFPlayer** | RX | GPIO 17 (TX2) |
| **RFID** | SDA (SS) | GPIO 5 |
| **RFID** | RST | GPIO 27 |
| **RFID** | SCK | GPIO 18 |
| **RFID** | MISO | GPIO 19 |
| **RFID** | MOSI | GPIO 23 |

---

## ⚙️ Installation & Setup

### 1. Backend Setup (Laptop)
Ensure you have Python installed.

```bash
# Clone the repository
git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
cd your-repo-name

# Install dependencies
pip install flask google-generativeai python-dotenv

# Set your Gemini API Key
# (Create a .env file or export it in your terminal)
export GEMINI_API_KEY="YOUR_GOOGLE_API_KEY"

# Run the Server
python app.py
