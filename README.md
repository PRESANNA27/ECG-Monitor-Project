# ECG Monitor using ESP32

## 📌 Overview
This project is a real-time ECG monitoring system using ESP32 and AD8232 sensor. It displays ECG signals on both OLED and a web dashboard.

## ⚙️ Features
- Real-time ECG waveform
- BPM calculation
- OLED display output
- Web dashboard visualization
- WebSocket communication
- Simulation + Live mode

## 🔌 Hardware Used
- ESP32 (DOIT DevKit V1)
- AD8232 ECG Sensor
- OLED Display (SSD1306)
- Electrodes

## 🌐 Network
- SSID: ECG_MONITOR
- IP: 192.168.4.1
- WebSocket: ws://192.168.4.1:81

## 🚀 How to Run
1. Upload `ECG.ino` to ESP32
2. Connect to WiFi: ECG_MONITOR
3. Open `index.html`
4. Click CONNECT

## 📊 Output
- Real-time ECG waveform
- BPM display
- Signal strength indicator

## ⚠️ Note
Proper electrode placement is required for accurate ECG signal.

## 🎯 Future Improvements
- Mobile app integration
- Cloud storage
- AI-based arrhythmia detection
