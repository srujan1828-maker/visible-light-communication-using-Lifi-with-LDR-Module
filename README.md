# Software-Defined Li-Fi (VLC) Receiver with Dynamic Jitter Compensation

This repository contains the C++ firmware and system architecture for a robust, low-cost Visible Light Communication (VLC) system. Traditional simplex Li-Fi setups using standard photoresistors (LDRs) and non-real-time operating systems (like Android) suffer from severe clock drift and ambient light interference. 

This project solves those hardware limitations through pure software engineering, utilizing an ESP32 to achieve reliable text transmission via a smartphone flashlight.

## 🌟 Key Innovations
Unlike standard Arduino VLC projects, this system does not rely on manual hardware tuning or perfectly synchronized real-time transmitters. 

* **Dynamic Timing Compensator:** An algorithm that dynamically expands the ESP32's sampling window to 305ms for the first half of a byte and contracts it to 300ms for the second half. This acts as a shock absorber for Android OS hardware latency, eliminating cumulative clock jitter.
* **Environmental Auto-Calibration:** A boot-sequence algorithm that samples dynamic room lighting ($V_{dark}$) and peak signal lighting ($V_{light}$) via the ESP32's 12-bit ADC. It calculates the precise mathematical threshold to filter out ambient noise, replacing the need for an LM393 hardware comparator.
* **Asynchronous Framing:** Utilizes an 8-bit `11111111` preamble to achieve falling-edge phase alignment without a shared clock wire.

## 🛠️ Hardware Requirements
* **Receiver Microcontroller:** ESP32 Development Board
* **Optical Sensor:** Standard CdS Photoresistor (LDR)
* **Resistor:** 10kΩ Resistor (for the voltage divider network)
* **Transmitter:** Any Android Smartphone (with a camera flashlight)
* **Cables:** Jumper wires and a Micro-USB/USB-C data cable for the ESP32

## 🔌 Circuit / Wiring Diagram
To utilize the software auto-calibration, you must use an **Analog** pin, not a Digital pin.

1. Connect one leg of the **LDR** to the **3.3V** pin on the ESP32.
2. Connect the other leg of the **LDR** to **Pin 34 (Analog In)** on the ESP32.
3. Connect that same LDR/Pin 34 junction to the **10kΩ Resistor**.
4. Connect the other end of the **10kΩ Resistor** to the **GND** pin on the ESP32.

*(Note: If you are using an LM393 sensor module, ignore the D0 pin and connect the A0 pin directly to ESP32 Pin 34).*

## 💻 Software Requirements
* **Arduino IDE:** (v2.0 or higher recommended) with the ESP32 Board Manager installed.
* **MIT App Inventor:** To compile and run the Android transmitter application.

## 🚀 Installation & Setup Instructions

### Phase 1: Flashing the ESP32 Receiver
1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/YOUR_USERNAME/YOUR_REPOSITORY_NAME.git](https://github.com/YOUR_USERNAME/YOUR_REPOSITORY_NAME.git)
Open the LiFi_Receiver.ino file in the Arduino IDE.

Ensure your Board is set to DOIT ESP32 DEVKIT V1 (or your specific ESP32 model) and the correct COM port is selected.

Click Upload.

Once uploaded, open the Serial Monitor and set the baud rate to 115200.

Phase 2: Setting up the Android Transmitter
Go to MIT App Inventor.

Import the provided .aia project file included in this repository.

Build the project and export the .apk file to your Android device.

Install the application on your phone (you may need to allow installations from unknown sources).

Phase 3: System Calibration & Execution
Positioning: Place the smartphone face down (or point the flashlight directly) at the LDR. Keep the distance between 10cm and 60cm for 0% Bit Error Rate (BER).

Auto-Calibration: Press the Reset (EN) button on the ESP32. Watch the Serial Monitor. The ESP32 will take 3 seconds to sample the ambient room light, and then ask you to turn on the flashlight to sample the peak signal.

Transmission: Once the threshold is dynamically set, open the Android App, type your text payload, and hit send.

Decoding: The Serial Monitor will display the raw binary stream and the decoded ASCII characters in real-time.


Performance Data
Empirical testing shows that the Dynamic Timing Compensator and Auto-Calibration allow the system to maintain a 0% Bit Error Rate (BER) at distances up to 60cm in standard office lighting, and up to 200cm in dark room conditions.
