/* Functions for calculating the angle by using triple axis acceleration
 * 
 */

void setBaseAngles() {
  /*  To be used in setup() with the bottle on a flat surface. Determines the two angles 
   *  (XZ and YZ) by making use of acceleration from the x, y, and z axis. Sets the variables 
   *  baseAngleY and baseAngleX. 
   */

  //variables used for the angle formula. 
  float accelX, accelY, accelZ;
  float result;
  float x2, y2, z2;

  accelX = myIMU.readFloatAccelX();
  accelY = myIMU.readFloatAccelY();
  accelZ = myIMU.readFloatAccelZ();
  
  //values used for calculation of angle
  x2 = accelX * accelX;
  y2 = accelY * accelY;
  z2 = accelZ * accelZ;

  //angle for x-axis
  result = sqrt(y2 + z2);
  result = accelX/result;
  baseAngleX = atan(result);

  //angle for y-axis
  result = sqrt(x2 + z2);
  result = accelY/result;
  baseAngleY = atan(result);

}

void calcAngle(){
  /*calculate current angle using current acceleration. 
   * Updates values: currentAngleX and current AngleY. 
   */
  float accelX, accelY, accelZ;
  float result;
  float x2, y2, z2;
  
  //baseline
  accelX = myIMU.readFloatAccelX();
  accelY = myIMU.readFloatAccelY();
  accelZ = myIMU.readFloatAccelZ();
  
  //values used for calculation of angle
  x2 = accelX * accelX;
  y2 = accelY * accelY;
  z2 = accelZ * accelZ;

  //angle for x-axis
  result = sqrt(y2 + z2);
  result = accelX/result;
  currentAngleX = atan(result);

  //angle for y-axis
  result = sqrt(x2 + z2);
  result = accelY/result;
  currentAngleY = atan(result);
  
  }
