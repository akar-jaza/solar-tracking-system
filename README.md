# Dual-Axis Solar Tracking System

**Sulaimani Polytechnic University – Technical College of Informatics**  
Department of Information Technology | May 2026

**Team:** Akar Jaza Fatah, Barzi Barzan, Fidyas Muhammad, Shahram Nahro  
**Supervisor:** Kanar R. Tariq

---

## Overview

This repository contains the Arduino source code for a low-cost dual-axis solar tracking prototype. The system uses four LDR (light-dependent resistor) sensors to detect the sun's position and automatically adjusts a solar panel using two SG90 servo motors — one for horizontal (azimuth) movement and one for vertical (elevation) movement. A 16×2 I2C LCD displays the real-time servo positions and measured panel voltage.

---

## Repository Contents

```
solar-tracking-system/
├── solar_tracker/
│   └── solar_tracker.ino   ← Main Arduino sketch
├── data/
│   └── sample_data.csv     ← Sample format for field test recordings
└── README.md
```

---

## Hardware Required

| Component | Specification | Quantity |
|---|---|---|
| Arduino Uno or Nano | ATmega328P | 1 |
| LDR (photoresistor) | GL5528 or equivalent | 4 |
| Resistor | 10 kΩ (for voltage dividers) | 4 |
| SG90 Servo Motor | 180°, 4.8 V | 2 |
| I2C LCD Display | 16×2, address 0x27 | 1 |
| Small PV Solar Panel | 1–5 W, 5–6 V open circuit | 1 |
| Resistors (voltage divider) | 40 kΩ + 10 kΩ | 1 set |
| Breadboard | 830-tie point, full size | 1 |
| Jumper Wires | Male-to-male | several |
| USB cable or battery pack | 5 V power supply | 1 |

---

## Pin Connections

### LDR Sensors (each wired as a voltage divider: 5V → LDR → midpoint → 10kΩ → GND)

| Arduino Pin | LDR Position |
|---|---|
| A0 | Top Right |
| A1 | Bottom Left |
| A2 | Bottom Right |
| A3 | Top Left |

### Voltage Measurement (solar panel output via voltage divider)

| Arduino Pin | Connection |
|---|---|
| A4 | Midpoint of 40kΩ / 10kΩ voltage divider across the panel |

> **Note:** The voltage divider scales the panel voltage down by a factor of 5 (ratio 10k / (40k+10k) = 0.2), allowing the Arduino to safely read up to ~25 V. For a 5–6 V panel you may simplify this to a direct connection and change the multiplier in the code from `* 5.0` to `* 1.0`.

### Servo Motors

| Arduino Pin | Servo |
|---|---|
| D9 | Horizontal servo (azimuth / left–right) |
| D10 | Vertical servo (elevation / up–down) |

### LCD Display (Software I2C — frees up A4/A5 for sensors)

| Arduino Pin | LCD |
|---|---|
| D2 | SDA |
| D3 | SCL |
| 5V | VCC |
| GND | GND |

> If your LCD has a different I2C address, change `0x27` to `0x3F` in the sketch.

---

## Required Libraries

Install all three libraries through the Arduino IDE Library Manager (**Sketch → Include Library → Manage Libraries...**) or manually via the links below.

| Library | Purpose | Install name |
|---|---|---|
| `Servo` | Controls SG90 servo motors | Built-in (no install needed) |
| `SoftwareWire` | Software I2C on D2/D3 | Search: **SoftwareWire** by Testato |
| `LiquidCrystal_SoftI2C` | LCD using SoftwareWire | Search: **LiquidCrystal_SoftI2C** |

### Manual installation (if not found in Library Manager)

1. Download the library as a `.zip` file from GitHub.
2. In the Arduino IDE go to **Sketch → Include Library → Add .ZIP Library...**.
3. Select the downloaded `.zip` file.

---

## Uploading the Code to the Arduino

1. **Install the Arduino IDE** from [arduino.cc/en/software](https://www.arduino.cc/en/software) if you have not already done so.

2. **Install the required libraries** as described in the section above.

3. **Open the sketch** — in the Arduino IDE click **File → Open** and navigate to `solar_tracker/solar_tracker.ino`.

4. **Connect the Arduino** to your computer using a USB cable.

5. **Select your board** — go to **Tools → Board** and choose **Arduino Uno** (or **Arduino Nano** if that is what you are using).  
   - For Nano: also set **Tools → Processor → ATmega328P (Old Bootloader)** if the upload fails.

6. **Select the port** — go to **Tools → Port** and choose the COM port (Windows) or `/dev/cu.usbserial-...` (Mac) that appeared when you plugged in the Arduino.

7. **Upload** — click the **Upload** button (right-arrow icon) or press `Ctrl+U` (Windows) / `Cmd+U` (Mac). Wait for "Done uploading" in the status bar.

8. **Open Serial Monitor** (optional, for debugging) — go to **Tools → Serial Monitor** and set the baud rate to **9600**. You will see live LDR readings printed every loop cycle.

---

## Adjusting the Code

| Variable | Location | Purpose |
|---|---|---|
| `tolerance` | top of sketch | Minimum LDR difference before servo moves. Increase to reduce jitter. |
| `stepSize` | top of sketch | Degrees the servo moves per loop iteration. Decrease for smoother tracking. |
| `hMin` / `hMax` | top of sketch | Safe angle limits for horizontal servo (default 40°–140°). |
| `vDown` / `vUp` | top of sketch | Safe angle limits for vertical servo (default 60°–100°). |
| `* 5.0` in voltage formula | `loop()` | Voltage divider scaling factor. Change if you use a different resistor ratio. |

---

## Serial Monitor Output

With the Serial Monitor open at 9600 baud, each line shows the four raw LDR values:

```
TL: 612 | TR: 598 | BL: 480 | BR: 471
TL: 615 | TR: 601 | BL: 483 | BR: 474
```

Higher values mean more light on that sensor. When the panel is correctly aligned all four values should be approximately equal.

---

## Test Data Format

Field test data is recorded manually in a spreadsheet and saved to CSV. See `data/sample_data.csv` for the column format. Power is calculated as P = V × I.

---

## License

This project was created for academic purposes at Sulaimani Polytechnic University. You are free to use and adapt it for educational and non-commercial work.
