#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const byte FLAME_PIN = 2;
const byte SMOKE_AO_PIN = A0;
const byte BUZZER_PIN = 8;

// Flame sensor logic
const bool FLAME_ACTIVE_LOW = true;

// Timing
const unsigned long FLAME_CONFIRM_MS = 500;
const unsigned long SENSOR_WARMUP_MS = 30000;
const unsigned long CALIBRATION_TIME_MS = 10000;
const unsigned long DISPLAY_INTERVAL_MS = 500;
const unsigned long SERIAL_INTERVAL_MS = 500;

// Smoke increase required above clean-air baseline
// Adjust these if needed.
const int SMOKE_MEDIUM_INCREASE = 80;
const int SMOKE_HIGH_INCREASE = 200;

// Flame state
bool fireDetected = false;
bool fireTiming = false;
unsigned long fireStartTime = 0;

// Smoke values
int cleanAirBaseline = 0;
int smokeValue = 0;
int mediumThreshold = 0;
int highThreshold = 0;

enum SmokeLevel {
  SMOKE_LOW,
  SMOKE_MEDIUM,
  SMOKE_HIGH
};

SmokeLevel currentSmokeLevel = SMOKE_LOW;

// Timers
unsigned long systemStartTime;
unsigned long lastDisplayTime = 0;
unsigned long lastSerialTime = 0;

// LCD cache
String lastLine1 = "";
String lastLine2 = "";


// Read the flame sensor
bool readFireSensor() {
  int value = digitalRead(FLAME_PIN);

  if (FLAME_ACTIVE_LOW) {
    return value == LOW;
  } else {
    return value == HIGH;
  }
}


// Confirm fire detection
void confirmFireDetection(bool rawFire, unsigned long currentTime) {
  if (!rawFire) {
    fireDetected = false;
    fireTiming = false;
    fireStartTime = 0;
    return;
  }

  if (!fireTiming) {
    fireTiming = true;
    fireStartTime = currentTime;
    fireDetected = false;
  }

  if (currentTime - fireStartTime >= FLAME_CONFIRM_MS) {
    fireDetected = true;
  }
}


// Take an average analog reading
int readSmokeValue() {
  const byte samples = 10;
  long total = 0;

  for (byte i = 0; i < samples; i++) {
    total += analogRead(SMOKE_AO_PIN);
    delay(2);
  }

  return total / samples;
}


// Calibrate the sensor in clean air
void calibrateSmokeSensor() {
  long total = 0;
  int minimumValue = 1023;
  int maximumValue = 0;

  unsigned long startTime = millis();

  updateLCD("Calibrating...", "Clean air only");

  Serial.println();
  Serial.println("Smoke sensor calibration started.");
  Serial.println("Keep the sensor in clean air.");
  Serial.println("Do not use smoke during calibration.");

  while (millis() - startTime < CALIBRATION_TIME_MS) {
    int value = analogRead(SMOKE_AO_PIN);

    total += value;

    if (value < minimumValue) {
      minimumValue = value;
    }

    if (value > maximumValue) {
      maximumValue = value;
    }

    delay(100);
  }

  cleanAirBaseline = total / ((CALIBRATION_TIME_MS / 100) + 1);

  mediumThreshold = cleanAirBaseline + SMOKE_MEDIUM_INCREASE;
  highThreshold = cleanAirBaseline + SMOKE_HIGH_INCREASE;

  Serial.print("Clean-air baseline: ");
  Serial.println(cleanAirBaseline);

  Serial.print("Medium threshold: ");
  Serial.println(mediumThreshold);

  Serial.print("High threshold: ");
  Serial.println(highThreshold);
}


// Determine smoke level relative to the baseline
SmokeLevel getSmokeLevel(int value) {
  if (value >= highThreshold) {
    return SMOKE_HIGH;
  }
  else if (value >= mediumThreshold) {
    return SMOKE_MEDIUM;
  }
  else {
    return SMOKE_LOW;
  }
}


// Convert smoke level to text
String smokeLevelText() {
  if (currentSmokeLevel == SMOKE_HIGH) {
    return "HIGH";
  }
  else if (currentSmokeLevel == SMOKE_MEDIUM) {
    return "MEDIUM";
  }
  else {
    return "LOW";
  }
}


// Update the LCD
void updateLCD(String line1, String line2) {
  if (line1 != lastLine1) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(line1.substring(0, 16));
    lastLine1 = line1;
  }

  if (line2 != lastLine2) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(line2.substring(0, 16));
    lastLine2 = line2;
  }
}


// Update display
void updateDisplay() {
  String line1 = "Smoke: " + smokeLevelText();
  String line2;

  if (fireDetected) {
    line2 = "Fire: DETECTED";
  } else {
    line2 = "Fire: Clear";
  }

  updateLCD(line1, line2);
}


// Control buzzer
void updateBuzzer() {
  if (fireDetected || currentSmokeLevel == SMOKE_HIGH) {
    tone(BUZZER_PIN, 2000);
  }
  else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}


// Print diagnostic values
void printDiagnostics(bool rawFire) {
  Serial.print("Smoke AO: ");
  Serial.print(smokeValue);

  Serial.print(" | Baseline: ");
  Serial.print(cleanAirBaseline);

  Serial.print(" | Medium threshold: ");
  Serial.print(mediumThreshold);

  Serial.print(" | High threshold: ");
  Serial.print(highThreshold);

  Serial.print(" | Smoke level: ");
  Serial.print(smokeLevelText());

  Serial.print(" | Flame: ");
  Serial.println(fireDetected ? "DETECTED" : "Clear");
}


void setup() {
  Serial.begin(9600);

  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();

  updateLCD("Fire & Smoke", "Detector");
  delay(2000);

  systemStartTime = millis();

  // MQ sensor warm-up
  updateLCD("Warming Sensor", "Please wait");
  Serial.println("MQ sensor warming up...");
  delay(SENSOR_WARMUP_MS);

  // Calibrate clean-air baseline
  calibrateSmokeSensor();

  updateLCD("System Running", "Smoke: LOW");
  delay(2000);
}


void loop() {
  unsigned long currentTime = millis();

  bool rawFire = readFireSensor();

  smokeValue = readSmokeValue();
  currentSmokeLevel = getSmokeLevel(smokeValue);

  confirmFireDetection(rawFire, currentTime);

  if (currentTime - lastDisplayTime >= DISPLAY_INTERVAL_MS) {
    lastDisplayTime = currentTime;
    updateDisplay();
  }

  updateBuzzer();

  if (currentTime - lastSerialTime >= SERIAL_INTERVAL_MS) {
    lastSerialTime = currentTime;
    printDiagnostics(rawFire);
  }

  delay(20);
}