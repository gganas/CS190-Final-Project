/* Liquid Level Functions
 * 
 */

float measureVolume(){
  float resistance = readResistance(SENSOR_PIN, SERIES_RESISTOR);
  // Map resistance to volume.
  float volume = resistanceToVolume(resistance, ZERO_VOLUME_RESISTANCE, CALIBRATION_RESISTANCE, CALIBRATION_VOLUME);

  delay(1000);
  return volume;
}

void measureResistence(){
  /*
   * used to calibrate full and empty water bottles
   */
  float resistance = readResistance(SENSOR_PIN, SERIES_RESISTOR);
  Serial.print("Resistance: "); 
  Serial.print(resistance, 2);
  Serial.println(" ohms");
  // Map resistance to volume.
  float volume = resistanceToVolume(resistance, ZERO_VOLUME_RESISTANCE, CALIBRATION_RESISTANCE, CALIBRATION_VOLUME);
  Serial.print("Calculated volume: ");
  Serial.println(volume, 5);
  // Delay for a second.
  delay(1000);
}


float readResistance(int pin, int seriesResistance) {
  // Get ADC value.
  float resistance = analogRead(pin);
  // Convert ADC reading to resistance.
  resistance = (1023.0 / resistance) - 1.0;
  resistance = seriesResistance / resistance;
  return resistance;
}


float resistanceToVolume(float resistance, float zeroResistance, float calResistance, float calVolume) {
  if (resistance > zeroResistance || (zeroResistance - calResistance) == 0.0) {
    // Stop if the value is above the zero threshold, or no max resistance is set (would be divide by zero).
    return 0.0;
  }
  // Compute scale factor by mapping resistance to 0...1.0+ range relative to maxResistance value.
  float scale = (zeroResistance - resistance) / (zeroResistance - calResistance);
  // Scale maxVolume based on computed scale factor.
  return calVolume * scale;
}
