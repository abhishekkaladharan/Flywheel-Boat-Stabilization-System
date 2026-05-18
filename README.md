# Flywheel Boat Stabilization System

## Project Overview

This project is a **Reaction Wheel / Flywheel Based Boat Stabilization System** designed to reduce roll motion in small boats using a high-inertia flywheel and IMU-based feedback control.

The system detects boat tilt using an **MPU6050 IMU sensor** and controls a motor-driven flywheel to generate counter torque, stabilizing the boat against waves and disturbances.

---

# Objective

* Reduce left-right rolling motion of the boat
* Improve stability in water waves
* Implement embedded control using Arduino/ESP32
* Develop a low-cost experimental marine stabilizer

---

#  Working Principle

## System Flow

```text
Boat Tilt
    ↓
BNO055 detects angle + angular velocity
    ↓
Arduino Nano processes sensor data
    ↓
PD control algorithm calculates correction
    ↓
Stepper/BLDC motor rotates flywheel
    ↓
Flywheel generates counter torque
    ↓
Boat returns to stable position
```

---

# Main Components

| Component                         | Description                   |
| --------------------------------- | ----------------------------- |
| Arduino Nano / ESP32              | Main controller               |
| BNO055                            | IMU sensor for tilt detection |
| NEMA23 Stepper Motor / BLDC Motor | Flywheel drive motor          |
| TB6600 / DM542 / VESC             | Motor driver                  |
| Flywheel                          | Generates stabilizing torque  |
| Battery                           | Power supply                  |
| Boat Hull                         | Stabilization platform        |

---

# Boat Dimensions

| Parameter        | Value  |
| ---------------- | ------ |
| Length           | 84 cm  |
| Width            | 25 cm  |
| Height           | 12 cm  |
| Draft            | 6–7 cm |
| Estimated Weight | 6–8 kg |

---

# Flywheel Design

| Parameter      | Value                         |
| -------------- | ----------------------------- |
| Outer Diameter | 180 mm                        |
| Inner Diameter | 120 mm                        |
| Thickness      | 12 mm                         |
| Material       | Mild Steel                    |
| Weight         | 1.5–1.8 kg                    |
| Design Type    | Rim-weighted 3-spoke flywheel |

---

# Mechanical Design

## Flywheel Features

* 3-spoke lightweight center
* Heavy outer rim for higher inertia
* Shaft mounted hub
* Balanced structure for low vibration

---

# Electronics Architecture

```text
BNO055
   │
Arduino Nano / ESP32
   │
Motor Driver
   │
Stepper / BLDC Motor
   │
Flywheel
```

---

# Wiring Connections

## MPU6050 → Arduino Nano

| BNO055  | Nano |
| ------- | ---- |
| VCC     | 5V   |
| GND     | GND  |
| SDA     | A4   |
| SCL     | A5   |

---

## TB6600 → Arduino Nano

| TB6600 | Nano |
| ------ | ---- |
| PUL+   | D2   |
| DIR+   | D3   |
| PUL-   | GND  |
| DIR-   | GND  |

---

# Control Algorithm

The system uses:

## PD Control

```math
Output = Kp × Error + Kd × GyroRate
```

Where:

* `Kp` → proportional gain
* `Kd` → derivative gain
* `Error` → tilt angle
* `GyroRate` → angular velocity

---

# Adjustable Parameters

```cpp
float Kp = 6.0;
float Kd = 1.2;

float MIN_RPM = 40;
float MAX_RPM = 140;

float DEAD_ANGLE = 3.5;
float DEAD_GYRO = 3.5;

float RPM_SCALE = 5.5;
```

---

# Stability Tuning

## If response is slow

Increase:

```cpp
Kp += 0.5;
```

---

## If boat oscillates

Increase:

```cpp
Kd += 0.2;
```

---

## If motor jerks

Increase:

```cpp
DEAD_ANGLE = 4.0;
```

---

# Noise Reduction Techniques

The MPU6050 may show unstable readings due to EMI and vibration from the motor.

## Solutions Implemented

* Separate power supply for motor and controller
* Common grounding
* Capacitor filtering
* Sensor calibration
* Software low-pass filtering
* Dead zone implementation
* Flywheel balancing

---

# Buoyancy Calculation

Using Archimedes Principle:

```math
F_b = ρgV
```

Displaced volume:

```math
V = L × B × T
```

Maximum displacement:

```math
≈ 12.6 kg
```

Recommended safe load:

```math
6–8 kg
```

---

# Features

Real-time stabilization
MPU6050 angle feedback
PD control algorithm
Adjustable RPM control
Flywheel-based stabilization
Water-wave response
Noise filtering
Modular design

---

# Future Improvements

* Complementary/Kalman filtering
* Closed-loop BLDC FOC control
* Dual-axis stabilization
* Wireless tuning
* Data logging
* GPS integration
* Waterproof enclosure

---

# Software Requirements

## Arduino Libraries

Install:

* BNO055 library
* Wire library

---

# Project Applications

* Marine stabilization
* Research platform
* Embedded systems learning
* Reaction wheel experiments
* Autonomous boats
* Robotics projects

---

# Project Outcome

The developed system successfully:

* detects boat roll motion
* controls flywheel dynamically
* reduces rolling motion in water
* demonstrates practical reaction-wheel stabilization

---

# Author

**Abhishek Kaladharan**
Embedded & Electronics Engineer
B.Tech Electronics and Communication Engineering

---

# License

This project is for educational and research purposes.
