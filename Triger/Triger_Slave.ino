#include<SoftwareSerial.h>
#include<Servo.h>

SoftwareSerial btSerial(2, 3); // RX | TX
Servo s1;

struct PacketData 
{
  byte rotor;
  byte trig;
};
PacketData data;

//Right motor
int enableRightMotor=5; 
int rightMotorPin1=12;
int rightMotorPin2=8;

//Left motor
int enableLeftMotor=4;
int leftMotorPin1=9;
int leftMotorPin2=10;

// Laser 
int laser=11;

unsigned long lastRecvTime = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  pinMode(laser,OUTPUT);

  analogWrite(enableRightMotor, 255);
  analogWrite(enableLeftMotor, 255);

  rotateMotor(0, 0);
  s1.attach(7);
  s1.write(0);
  
  
  btSerial.begin(38400);  
}


void loop()
{
  String dataString;
  if (btSerial.available())
  {
    dataString = btSerial.readStringUntil('\n');
    
    
    sscanf(dataString.c_str(), "%d,%d", &data.rotor, &data.trig);
    int power = map(data.rotor, 0, 254, -255, 255);
    
    int angle = map(data.trig, 127, 0, 180, 0);
    s1.write(angle);

  
    int rightMotorSpeed, leftMotorSpeed;
    rightMotorSpeed =  abs(power);
    leftMotorSpeed =  abs(power);
  

    String dataString;
    dataString = dataString 
               + data.rotor + " "
               + data.trig + "\n";
              //  + angle + "\n";
  
    Serial.print(dataString);

    if(rightMotorSpeed>10){
      digitalWrite(laser,HIGH);
    }
    else{
      digitalWrite(laser,LOW);
    }
  
    rotateMotor(rightMotorSpeed, leftMotorSpeed);
    lastRecvTime = millis();      
  }
  else
  {
    unsigned long now = millis();
    if ( now - lastRecvTime > 1000 )       //Signal lost after 1 second. Reset the motor to stop
    {
      rotateMotor(0, 0);   
   }
 }   
 delay(10);   
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  
  if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }      
}