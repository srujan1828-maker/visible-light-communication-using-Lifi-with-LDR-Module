
const int analogRxPin = 34;

const int ClockDelay = 500; 

const int delayFirstBits = 510; 
const int delayLaterBits = 500; 

int lightThreshold = 0; 

int readBit() {
  int sensorValue = analogRead(analogRxPin);
  
  if (sensorValue < lightThreshold) {
    return 1; 
  } else {
    return 0;
  }
}

void calibrateSensor() {
  long totalDark = 0;
  long totalLight = 0;
  int samples = 30;

  Serial.println("\n--- ENVIRONMENTAL CALIBRATION START ---");
  
  Serial.println("\n[1/2] Keep the flashlight OFF.");
  Serial.println("Reading ambient room light in 3 seconds...");
  delay(3000); 
  
  Serial.print("Measuring DARK level");
  for (int i = 0; i < samples; i++) {
    totalDark += analogRead(analogRxPin);
    Serial.print(".");
    delay(50);
  }
  int avgDark = totalDark / samples;
  Serial.println("\n-> Average DARK value: " + String(avgDark));

  Serial.println("\n[2/2] Turn the flashlight ON and point it steadily at the sensor.");
  Serial.println("Reading flashlight level in 5 seconds...");
  delay(5000); 
  
  Serial.print("Measuring LIGHT level");
  for (int i = 0; i < samples; i++) {
    totalLight += analogRead(analogRxPin);
    Serial.print(".");
    delay(50);
  }
  int avgLight = totalLight / samples;
  Serial.println("\n-> Average LIGHT value: " + String(avgLight));

  lightThreshold = (avgDark + avgLight) / 2;
  
  Serial.println("\n--- CALIBRATION COMPLETE ---");
  Serial.print("New Custom Threshold set to: ");
  Serial.println(lightThreshold);
  Serial.println("Please turn the flashlight OFF. Ready to receive data!");
  Serial.println("=======================================================\n");
}

void waitForStart() {
  Serial.println("SYSTEM Waiting for wake-up signal (8 solid flashes)...");
  int highCount = 0;

  while (highCount < 8) {
    if (readBit() == 1) {
      highCount++;
    } else {
      highCount = 0;
    }
    delay(ClockDelay);
  }

  Serial.println("SYSTEM Locked! Waiting for the synchronization edge...");

  while (readBit() == 1) {
    delay(2); 
  }

  delay(ClockDelay / 2);
}

char readByte() {
  char c = 0;

  for (int i = 0; i < 8; i++) {
    unsigned long startOfBit = millis(); 

    c = (c << 1) | readBit(); 
    
    int currentDelay;
    if (i < 4) {
      currentDelay = delayFirstBits; 
    } else {
      currentDelay = delayLaterBits; 
    }

    while (millis() - startOfBit < currentDelay) {
    }
  }

  return c;
}

void setup() {
  pinMode(analogRxPin, INPUT);
  
  Serial.begin(115200); 
  delay(1000); 
  
  Serial.println("=====================================");
  Serial.println(" ESP32 LI-FI RECEIVER INITIALIZING...");
  Serial.println("=====================================");
  
  calibrateSensor();
}

void loop() {
  waitForStart();

  Serial.print("Incoming Message: ");
  while (true) {
    char c = readByte();

    if (c == 0) {
      break; 
    }

    Serial.print(c);
  }

  Serial.println(); 
  Serial.println("[SYSTEM] Transmission Complete.\n");
}