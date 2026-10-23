# Arduino-Fire-Detection-System
Arduino-based fire and flame detection system using a flame sensor, smoke sensor, buzzer, and LCD display.

# Fire and Smoke Detection System

An Arduino-based fire and smoke detection system using a flame sensor, MQ-series smoke sensor, I2C LCD, and buzzer.

The system displays:

- Smoke level: LOW, MEDIUM, or HIGH
- Fire status: Clear or DETECTED
- Alarm notification using a buzzer

## Features

- Detects fire using a flame sensor
- Measures smoke level using an MQ smoke sensor
- Displays smoke level on a 16x2 LCD
- Displays fire detection status
- Activates the buzzer when fire is detected
- Activates the buzzer when smoke reaches a high level
- Uses clean-air calibration for smoke detection
- Provides sensor readings through the Serial Monitor

## Required Components

- Arduino Uno
- Flame sensor module
- MQ-2, MQ-5, MQ-135, or similar smoke sensor
- 16x2 I2C LCD
- Buzzer
- Breadboard
- Jumper wires
- USB cable
- 5V power supply

## Circuit Connections

### Flame Sensor

| Flame Sensor Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| DO | D2 |

The flame sensor digital output is connected to Arduino digital pin D2.

### MQ Smoke Sensor

| MQ Sensor Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| AO | A0 |

The analog output of the MQ sensor is connected to Arduino analog pin A0.

The analog output is used to measure smoke levels. The digital output is not used for the smoke level measurement.

### Buzzer

| Buzzer Pin | Arduino Pin |
|---|---|
| Positive | D8 |
| Negative | GND |

### I2C LCD

| LCD Pin | Arduino Uno Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## Display Output

When no smoke or fire is detected, the LCD displays:

```text
Smoke: LOW
Fire: Clear
```

When a medium level of smoke is detected, the LCD displays:

```text
Smoke: MEDIUM
Fire: Clear
```

When a high level of smoke is detected, the LCD displays:

```text
Smoke: HIGH
Fire: Clear
```

When fire is detected, the LCD displays:

```text
Smoke: LOW
Fire: DETECTED
```

The smoke level may also display MEDIUM or HIGH while fire is detected.

## How the System Works

### Fire Detection

The flame sensor detects the presence of a flame using its digital output.

When a flame is detected, the system confirms the signal for a short period. This helps reduce false detections caused by sensor noise.

After confirmation:

- The LCD displays Fire: DETECTED
- The buzzer turns on

When the flame is removed, the display returns to Fire: Clear.

### Smoke Detection

The MQ smoke sensor uses its analog output to measure the smoke level.

The Arduino reads the analog sensor value and compares it with the clean-air baseline.

The smoke level is classified as:

- LOW when the reading is close to the clean-air baseline
- MEDIUM when the reading increases above the medium threshold
- HIGH when the reading increases above the high threshold

The buzzer turns on when the smoke level reaches HIGH.

## MQ Sensor Warm-Up

MQ sensors contain an internal heating element and require warm-up time before they provide stable readings.

During startup:

- The sensor should remain in clean air
- Smoke should not be placed near the sensor
- The sensor should not be touched
- The sensor should not be exposed directly to a flame
- The system should be allowed to warm up before testing

A new MQ sensor may require a longer initial warm-up period for more accurate results.

## Smoke Sensor Calibration

The smoke sensor is calibrated in clean air when the system starts.

During calibration, the system records the normal analog value of the sensor. This value becomes the clean-air baseline.

The medium and high smoke thresholds are calculated from this baseline.

For accurate calibration:

- Place the sensor in clean air
- Keep smoke away from the sensor
- Do not breathe directly onto the sensor
- Avoid perfumes, sprays, and gases near the sensor
- Wait until calibration is complete

## Smoke Level Adjustment

Every MQ sensor can produce different analog readings. The smoke level thresholds may need to be adjusted for each sensor.

If the sensor always displays LOW, the threshold values may be too high.

If the sensor always displays MEDIUM or HIGH in clean air, the sensor may need more warm-up time or the threshold values may be too low.

The sensor should be tested using the Serial Monitor to observe the clean-air reading and the reading when smoke is present.

## Testing

### Flame Sensor Test

1. Power on the Arduino.
2. Wait for the system to start.
3. Place a small flame near the flame sensor.
4. Check that the LCD displays Fire: DETECTED.
5. Check that the buzzer turns on.
6. Remove the flame.
7. Check that the LCD returns to Fire: Clear.

### Smoke Sensor Test

1. Power on the Arduino.
2. Keep the MQ sensor in clean air during startup.
3. Wait for the warm-up and calibration process.
4. Confirm that the LCD displays Smoke: LOW.
5. Carefully bring a small amount of smoke near the sensor.
6. Check whether the display changes to Smoke: MEDIUM or Smoke: HIGH.
7. Remove the smoke and allow the sensor to return to the LOW level.

Do not place the sensor directly in a flame.

## Troubleshooting

### Smoke Always Displays LOW

Check the following:

- The MQ sensor analog output is connected to A0
- The sensor is powered from 5V
- The sensor ground is connected to Arduino ground
- The sensor has had enough warm-up time
- The sensor was calibrated in clean air
- Smoke is actually reaching the sensor
- The smoke threshold is not too high

### Smoke Always Displays MEDIUM or HIGH

Check the following:

- The sensor has warmed up properly
- Calibration was performed in clean air
- The sensor is not near perfume, alcohol, gas, or chemical vapors
- The sensor is not in a dusty or polluted area
- The smoke thresholds are not too low

### Smoke Reading Does Not Change

Check the following:

- The AO pin is connected to A0
- VCC is connected to 5V
- GND is connected to Arduino GND
- The MQ sensor heater becomes warm
- The jumper wires are properly connected
- The sensor module is not damaged
- The analog pin is working correctly

### Fire Is Always Detected

Check the flame sensor wiring and confirm that the digital output is connected to D2.

The flame sensor logic may need to be reversed depending on the sensor module. Some modules detect fire when their output is LOW, while others detect fire when their output is HIGH.

### LCD Does Not Display Text

Check the following:

- LCD VCC is connected to 5V
- LCD GND is connected to GND
- SDA is connected to A4
- SCL is connected to A5
- The LCD contrast adjustment is set correctly
- The LCD I2C address matches the program

Common LCD I2C addresses include 0x27 and 0x3F.

## Safety Warning

This project is intended for educational and demonstration purposes only.

It is not a certified fire alarm, smoke detector, or gas safety system. Do not use it as the only protection against fire, smoke, or dangerous gases.

For real safety applications, use certified smoke alarms, fire alarms, and gas detectors.

Do not expose the MQ sensor directly to fire. Test the system in a well-ventilated area and avoid inhaling smoke.

## License

This project is provided for educational use. You may modify and improve it for personal projects.
