// Interface coding
//
// Author:  Gary Cousins
// Date: 21/08/16
// Platform: Arduniuo UNO

// This code can be used, modified and distributed without limitation
// but the author takes no responsibility of loss or damages incurred as
// a result.  The author offers no support or warranty of any kind.  It
// is the responsibility of the end user to ensure this code is fit
// for purpose.

// ---------------------------------------------------------------------
// The message consist of:
// Header (3 bytes)
// Timestamp (4 bytes)
// Analog values (4x2 bytes)
// Digital value (1 byte)
// Footer (2 bytes)
// Total length 18 bytes.


// ---------------------------------------------------------------------
// Defined values
#define FIRST_DIG_PIN 2
#define NUM_DIG_PINS  8
#define FIRST_AN_PIN  0
#define NUM_AN_PINS   4

#define MSG_HEADER    "MSG"
#define MSG_FOOTER    "E\n"

// ---------------------------------------------------------------------

void setup()
{
  // Don't use the setup function
}

// ---------------------------------------------------------------------

void loop()
{
  // Local variables
  int analog[4] = {0, 0, 0, 0};  // Holds the 4 analogue values
  unsigned long timeStamp1 = 0;  // 2 Variabls to work out the timestamp
  unsigned long timeStamp2 = 0;
  byte boolResult = 0;           // Holds 8 digital values
  int tempBoolValue = 0;         // Holds the current bool result
  int i = 0;                     // Counter variable

  // Setup serial port
  Serial.begin(57600);

  // Setup digital IO lines, no setup for analag lines needed
  for (i = FIRST_DIG_PIN; i < (FIRST_DIG_PIN + NUM_DIG_PINS); i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
  
  // Take a reading, for a message and pass to the PC
  do
  {
    // Take a timestamp
    timeStamp1 = millis();

    // Read analogue pins
    for (i = FIRST_AN_PIN; i < (FIRST_AN_PIN + NUM_AN_PINS); i++)
    {
      analog[i] = analogRead(i);
    }

    // Read digital pins
    boolResult = 0;
    for (i = FIRST_DIG_PIN; i < (FIRST_DIG_PIN + NUM_DIG_PINS); i++)
    {
      tempBoolValue = digitalRead(i);
      boolResult = boolResult + (tempBoolValue << (i - FIRST_DIG_PIN));
    }

    // Take a second timestamp and use the average
    timeStamp2 = millis();
    timeStamp2 = (timeStamp1 + timeStamp2) / 2;

    // Send message
    Serial.write(MSG_HEADER);
    Serial.write((byte*)&timeStamp2, sizeof(timeStamp2));
    Serial.write((byte*)&analog[0], sizeof(analog));
    Serial.write(boolResult);
    Serial.write(MSG_FOOTER);

    // Short delay
    delay(100);
  } while (true);
}
