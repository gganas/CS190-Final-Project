/*Gilynne Ganas, Steven Chalmers

   IOT WATER BOTTLE

  Any code commented out are for fingerprint usability. It will disable the bluetooth if uncommented
  Code had been added to accomodate for the fingprint scanner-less mode. 
  
*/

#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

//6DoF Breakout
LSM6DS3 myIMU;
float currentAngleX;
float currentAngleY;
float baseAngleX, baseAngleY;
float allowedDifference = 0.01;

//FingerPrint Scanner
FPS_GT511C3 fps(4, 5); //SS-RX, SS-TX
int fingerID;

//Liquid Level Sensor
#define SERIES_RESISTOR     560    // Value of the series resistor in ohms.    
#define SENSOR_PIN          0      // Analog pin which is connected to the sensor. 

#define ZERO_VOLUME_RESISTANCE    2500    // Resistance value (in ohms) when no liquid is present.
#define CALIBRATION_RESISTANCE    1950    // Resistance value (in ohms) when liquid is at max line.
#define CALIBRATION_VOLUME        16.00    // Volume (in any units) when liquid is at max line.

float currentVolume;
float reportVolume;

//HC-05
SoftwareSerial BTSerial(10, 11); //SS-RX, SS-TX
int BTData;

int bottle_state;
#define START                 0
#define GENERAL               1
#define DRINK                 2
#define REFILLED_OR_EMPTIED   3
#define MEASURE               4
#define ENROLL_NEW_USER       5


//float fData[20]; //Contains amount of water drank per user, position in array relates to fingerID
float waterDrank = 0.00;


void setup() {
  Serial.begin(9600);
  //Start-state
  myIMU.begin();
  fps.Open();         //send serial command to initialize fps
  fps.SetLED(true);   //turn on LED so fps can see fingerprint
  BTSerial.begin(9600);
  delay(2000);
  setBaseAngles(); //calculate what a flat surface is
  reportVolume = measureVolume();
  delay(500);
  bottle_state = GENERAL;

}

void loop() {

  switch (bottle_state) {

    case START:
      Serial.println("Start");
      BTSerial.listen();
      if (BTSerial.available() > 0) {
        BTData = BTSerial.read();
        Serial.println(BTData);
        if (BTData == '1') {
          Serial.println("Connected!");
          BTSerial.write(reportVolume);
          BTData = 0;
          bottle_state = GENERAL;
          break;
        }

      }
      else {
        delay(500);
        break;
      }


    case GENERAL:
      Serial.println("General");
      BTSerial.listen();
      if (BTSerial.available() > 0) {
        Serial.println("data");
        BTData = BTSerial.read();
        Serial.print("BTData: ");
        Serial.println(BTData);
        
        if (BTData == '1') {
          BTSerial.write(reportVolume);
          BTData = 0;
          delay(100);
          break;
        }
        
        else if (BTData == '2') {
          bottle_state = ENROLL_NEW_USER;
          BTData = 0;
          delay(100);
          break;
        }
        else if (BTData == '3') {
          BTSerial.write(waterDrank);     //comment this line and the next if you want the fingerprint scanner
          break;
        
//          while (true) {
//            if (fps.IsPressFinger()) {
//              fps.CaptureFinger(false);
//              fingerID = fps.Identify1_N();
//              if (fingerID < 200) {
//                fps.SetLED(false);
//                delay(500);
//                fps.SetLED(true);
//                Serial.print(fData[fingerID]);
//                BTSerial.write(fData[fingerID]);
//                BTData = 0;
//                delay(100);
//                break;
//              }
//            }
//          }
        }
      }


//      if (fps.IsPressFinger()) {
//        fps.CaptureFinger(false);
//        fingerID = fps.Identify1_N();
//        if (fingerID < 200) {
//          bottle_state = DRINK;
//          fps.SetLED(false);
//          delay(500);
//          fps.SetLED(true);
//          break;
//        }
//        else {
//          BTSerial.write("Finger not found!");
//          bottle_state = REFILLED_OR_EMPTIED;
//          break;
//        }
//      }



      calcAngle();
      if ((currentAngleX <= (baseAngleX + allowedDifference)) &&
          (currentAngleX >= (baseAngleX - allowedDifference)) &&
          (currentAngleY <= (baseAngleY + allowedDifference)) &&
          (currentAngleY >= (baseAngleY - allowedDifference))) {
            
//**comment section out for fps 
        currentVolume = measureVolume();
        if(reportVolume > currentVolume + 2){
          bottle_state = DRINK;
          break;
          }
        else if(reportVolume < currentVolume - 2){
          bottle_state = REFILLED_OR_EMPTIED;
          }
//**end section
        
//        if (((reportVolume - currentVolume) >= 1) || (((-1) * (reportVolume - currentVolume)) >= 1)) {
//          bottle_state = REFILLED_OR_EMPTIED;
//          break;
//        }
      }

      delay(100);
      break;

    case DRINK:
      Serial.println("Drink");
      delay(2000);
//      calcAngle();
//
//      if ((currentAngleX <= (baseAngleX + allowedDifference)) &&
//          (currentAngleX >= (baseAngleX - allowedDifference)) &&
//          (currentAngleY <= (baseAngleY + allowedDifference)) &&
//          (currentAngleY >= (baseAngleY - allowedDifference))) {
//
//        currentVolume = measureVolume();

        waterDrank += reportVolume - currentVolume; //comment out for fps
        Serial.print("water drank: ");
        Serial.println(waterDrank);
//      fData[fingerID] += drank;
        reportVolume = currentVolume;
        bottle_state = GENERAL;
        break;
//      }
//      else {
//        delay(1000);      
//        break;
//      }


    case REFILLED_OR_EMPTIED:
      Serial.println("R/E");
      reportVolume = currentVolume;
      bottle_state = GENERAL;
      break;

      
    case ENROLL_NEW_USER:
      Serial.println("Enroll");
      Enroll();
      BTSerial.write("STOP");
      bottle_state = GENERAL;
      break;


  }

}
