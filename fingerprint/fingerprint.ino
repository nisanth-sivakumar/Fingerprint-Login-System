#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  Serial.begin(9600); // Begins serial communication
  finger.begin(57600); // Begins fingerprint communication
}

void loop()                   
{
  while (Serial.available() <= 0) {} // Waits for serial communication
  char receivedChar = char(Serial.read()); // Reads as char from serial port
  int receivedID = receivedChar - '0'; // Converts to int
  checkCredentials(receivedID); // Method to compare IDs
  delay(10); // Doesn't need to run at full speed
}

// Checks the received ID
void checkCredentials(int receivedID) {
  if(receivedID == getFingerprintIDez()) { // Checks if ID from serial matches with fingerprint
    Serial.println("v"); // Sends verification to node.js server from serial monitor
  }
  else if(receivedID != receivedID) { 
    Serial.println("n"); // Sends login failed to node.js server
  }
}


// Example method from Adafruit Library
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
