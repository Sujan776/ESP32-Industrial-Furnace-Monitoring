# Industrial Furnace Monitoring & Automatic Control System

An ESP32-based industrial furnace monitoring and automatic control system developed as a **Wokwi simulation using PlatformIO**.

The system continuously monitors simulated **temperature and airflow conditions**, determines the furnace operating state, automatically controls a **servo-based damper**, and provides real-time visual and audio alerts.

---

## 📌 Project Overview

Industrial furnaces require continuous monitoring of operating parameters such as temperature and airflow to maintain safe and efficient operation.

This project demonstrates a simulated embedded control system using an **ESP32**.

Two potentiometers are used to simulate:

- 🌡️ Furnace Temperature
- 💨 Airflow

Based on these simulated parameters, the ESP32 classifies the furnace condition as:

- 🟢 **NORMAL**
- 🟡 **WARNING**
- 🔴 **CRITICAL**

The system then automatically controls a servo motor representing a **furnace air damper** and activates visual/audio alarms when abnormal conditions occur.

> **Note:** This project is a Wokwi-based prototype/simulation intended for educational and portfolio purposes. The potentiometers simulate furnace sensors, and the servo represents damper control.

---

## 🎯 Objectives

The main objectives of this project are:

- Monitor furnace temperature in real time.
- Monitor airflow conditions.
- Detect abnormal furnace operating conditions.
- Automatically control a simulated air damper.
- Provide visual status indication using LEDs.
- Generate an alarm during critical conditions.
- Display real-time parameters on an OLED.
- Implement furnace START/STOP control.
- Demonstrate embedded industrial automation using ESP32.

---

## ⚙️ Key Features

- 🌡️ Temperature monitoring
- 💨 Airflow monitoring
- 🧠 Automatic condition classification
- ⚙️ Servo-based damper control
- 🖥️ Real-time OLED monitoring
- 🟢 Normal status indication
- 🟡 Warning indication
- 🔴 Critical alarm indication
- 🔊 Over-temperature buzzer alarm
- 🔘 Furnace START/STOP pushbutton
- 📟 Serial monitoring
- 🎛️ Simulated sensors using potentiometers
- 🔌 ESP32-based embedded control
- 🧪 Wokwi simulation
- 🛠️ PlatformIO development environment

---

## 🏭 Industrial Use Case

The concept can be applied to industrial environments where temperature and airflow must be monitored and controlled.

Potential applications include:

- Industrial furnaces
- Heat treatment systems
- Metal processing plants
- Manufacturing industries
- Kilns
- Boilers
- Thermal processing equipment
- Industrial heating systems

A real-world implementation could replace the simulated potentiometers with actual industrial sensors and connect the controller to an industrial automation system.

---

## 🧩 System Architecture

```text
              ┌──────────────────────┐
              │      ESP32 MCU        │
              │                      │
              │  Monitoring & Control │
              └──────────┬───────────┘
                         │
          ┌──────────────┼──────────────┐
          │              │              │
          ▼              ▼              ▼
   Temperature        Airflow       Pushbutton
   Potentiometer     Potentiometer    START/STOP
          │              │              │
          └──────────────┼──────────────┘
                         │
                         ▼
                ┌────────────────┐
                │ Control Logic  │
                └───────┬────────┘
                        │
        ┌───────────────┼────────────────┐
        │               │                │
        ▼               ▼                ▼
     OLED Display    Status LEDs      Buzzer
        │               │                │
        └───────────────┼────────────────┘
                        │
                        ▼
                Servo Damper Control
