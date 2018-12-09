/* Fingerprint Scanner functions.
 * 
 */
//BTSerial.write
//Serial.print

int Enroll(){
  /*Adds new fingerprint to fingerprint database and assigns it with a id.
   * 
   */

  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true) {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid==true) enrollid++;
  }
  fps.EnrollStart(enrollid);

  // enroll
  BTSerial.write("Press finger to Enroll #");                     //change
  BTSerial.write(enrollid);                                     //change
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false) {
    BTSerial.write("Remove finger");                            //change
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    BTSerial.write("Press same finger again");                 // change
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false) {
      BTSerial.write("Remove finger");                        //change
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      BTSerial.write("Press same finger yet again");         //change
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false) {
        BTSerial.write("Remove finger");                     //change
        iret = fps.Enroll3();
        if (iret == 0) {
          BTSerial.write("Enrolling Successful");            //change
          return enrollid;
        }
        else {
          BTSerial.write("Enrolling Failed with error code:");     //change
          BTSerial.write(iret);                                   //change
        }
      }
      else BTSerial.write("Failed to capture third finger");         //change
    }
    else BTSerial.write("Failed to capture second finger");            //change
  }
  else BTSerial.write("Failed to capture first finger");               //change
}
