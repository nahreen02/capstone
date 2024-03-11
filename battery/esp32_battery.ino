const int batteryVoltagePin = 1; // GPIO1

void setup() {
  Serial.begin(115200);

  // Configure ADC
  analogReadResolution(12); // Set ADC resolution to 12 bits
}

void loop() {
  Serial.print("Battery Voltage: ");
  Serial.print(getBattery(), 2); // Print voltage with 2 decimal places
  Serial.println(" V");

  delay(1000); // Delay for 1 second before the next reading
}

float getBattery(){
  int rawValue = analogRead(batteryVoltagePin); // Read raw ADC value
  float batt = (3.3 / (1 << 12)) * 3 * rawValue;
  return batt; 
}

float getPercent(){
  float percent = (getBattery()/3.7)*100
  return percent; 
}