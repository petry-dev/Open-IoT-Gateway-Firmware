# 🏭 Open IoT Gateway (ESP32 Firmware)

![Status](https://img.shields.io/badge/Status-Active-success)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C)
![License](https://img.shields.io/badge/License-MIT-green)

## 📖 Overview

**Open IoT Gateway** is a robust, generic firmware designed for **Industry 4.0** applications. It transforms an ESP32 microcontroller into a bridge between physical industrial signals (Digital/Analog I/O) and modern IT protocols (MQTT).

The project features a responsive **Single Page Application (SPA)** hosted directly on the ESP32's filesystem, allowing for user-friendly configuration without hardcoding credentials.

---

## 📸 Interface Gallery

The user interface is built to be responsive and supports high-contrast **Dark Mode** for industrial environments.

### ☀️ Light Mode (Day)

| 1. Start Screen | 2. General Config | 3. Sensors Map |
| :---: | :---: | :---: |
| <img src="assets/light-1.png" width="100%"> | <img src="assets/light-2.png" width="100%"> | <img src="assets/light-3.png" width="100%"> |

| 4. Validation & Test | 5. Success Feedback | |
| :---: | :---: | :---: |
| <img src="assets/light-4.png" width="100%"> | <img src="assets/light-5.png" width="100%"> | 👆 **Responsive UI** |

### 🌙 Dark Mode (Night)

| 1. Start Screen | 2. General Config | 3. Sensors Map |
| :---: | :---: | :---: |
| <img src="assets/dark-1.png" width="100%"> | <img src="assets/dark-2.png" width="100%"> | <img src="assets/dark-3.png" width="100%"> |

| 4. Validation & Test | 5. Success Feedback | |
| :---: | :---: | :---: |
| <img src="assets/dark-4.png" width="100%"> | <img src="assets/dark-5.png" width="100%"> | 👆 **High Contrast** |

---

## ✨ Key Features

* **Connectivity:**
    * **Wi-Fi Manager:** Automatic failover between Station Mode (STA) and Access Point (AP).
    * **MQTT Client:** Real-time publishing of machine states and sensor data.
    * **MTConnect Proxy:** Fetches data from CNC machines via HTTP and converts it to MQTT.
* **Web Interface (SPA):**
    * **Modern UI:** HTML5, CSS3, and Vanilla JS (no heavy frameworks).
    * **Async WebServer:** Non-blocking request handling.
    * **Internationalization (i18n):** English, Portuguese, and Spanish.
    * **Theme System:** Persistent Dark/Light mode.
* **Architecture:**
    * **Clean Code:** Hardware abstraction via `IOController`.
    * **Non-Volatile Storage:** Uses ESP32 Preferences (NVS).
    * **Safety:** Physical factory reset (Long-press BOOT button).

## 🛠️ Tech Stack

* **Microcontroller:** ESP32 (DevKit V1)
* **Framework:** Arduino / PlatformIO
* **Language:** C++
* **Filesystem:** LittleFS
* **Protocols:** MQTT, HTTP, MTConnect

## 🚀 Installation & Setup

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/petry-dev/Open-IoT-Gateway-Firmware.git](https://github.com/petry-dev/Open-IoT-Gateway-Firmware.git)
    ```

2.  **Upload Filesystem (Important):**
    * In PlatformIO: *Project Tasks* -> *Platform* -> *Upload Filesystem Image*.
    * *This uploads the HTML/CSS/JS files to the ESP32.*

3.  **Upload Firmware:**
    * In PlatformIO: *Project Tasks* -> *General* -> *Upload*.

4.  **Configuration:**
    * Connect to Wi-Fi AP: `IoT-Gateway-XXXX` (Pass: `admin1234`)
    * Go to: `http://192.168.4.1`

## 📂 Project Structure

```text
├── assets/                # Documentation images
├── data/                  # Web App (HTML/CSS/JS)
├── include/               # Headers
├── src/                   # C++ Source Code
│   ├── main.cpp           # Main Loop
│   ├── ConfigWifi.cpp     # WebServer Logic
│   └── ...
└── platformio.ini         # Configuration