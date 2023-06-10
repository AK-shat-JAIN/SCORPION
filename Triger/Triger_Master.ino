#include<SoftwareSerial.h>

SoftwareSerial btSerial(2, 3); // RX | TX


int plug=5;

struct PacketData 
{
  byte rotor;
  byte ser;
};
PacketData data;

void setup()
{
  Serial.begin(9600);

  btSerial.begin(38400);   
}

int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 530)
  {
    value = map(value, 530, 1023, 127, 254);
  }
  else if (value <= 500)
  {
    value = map(value, 500, 0, 127, 0);  
  }
  else
  {
    value = 127;
  }

  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

void loop()
{
  data.rotor = mapAndAdjustJoystickDeadBandValues(analogRead(A0), false);
  data.ser = mapAndAdjustJoystickDeadBandValues(analogRead(A2), false);

    String dataString;
    dataString = dataString 
                + data.ser + "," 
                + data.rotor + "\n";
    
    Serial.print(dataString);
    
  btSerial.print(dataString);
  delay(10);
}