// Use blynk app for support in mobile app. 

//Bluetooth

#define BLYNK_PRINT Serial

#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX

#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "The Code you get from your Blynk App";

SoftwareSerial SerialBLE(7, 8); // RX, TX

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);
// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget

int op = 0;
int pinValue = 0;

//Water level
const int trigPin = 2;
const int echoPin = 4;
const int motor = 12;
long duration;
long distance;
int count = 0;

void setup() {
  //Water Level
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT);
  pinMode(motor, OUTPUT);
  Serial.begin(9600);

  //LDR
  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(6, OUTPUT);

  //Bluetooth

  // Debug console
  Serial.begin(38400);

  SerialBLE.begin(38400);
  Blynk.begin(SerialBLE, auth);

  Serial.println("Waiting for connections...");

}
void loop() {
  //Waterlevel start
  // Clears the trigPin
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration / 29 / 2;

  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm");
  Serial.print(100 - ((distance - 3) * 100) / 12);
  Serial.println("%");
  op = 100 - ((distance - 3) * 100) / 12;

  if (distance > 9)
  {
    //motor on
    digitalWrite(motor, 0);
    Serial.println("Motor On");
  }
  if (distance <= 3)
  {
    //motor off
    digitalWrite(motor, 1);
    Serial.println("Motor Off");
  }
//  delay(2000);
  //Waterlevel end



  //LDR OP
  int ldrVal = analogRead(A0);

  if (ldrVal < 450)
  {
    digitalWrite(6, 450 - ldrVal);
    digitalWrite(9, 450 - ldrVal);
    digitalWrite(10, 450 - ldrVal);
    digitalWrite(11, 450 - ldrVal);
  }
  else
  {
    digitalWrite(6, 0);
    digitalWrite(9, 0);
    digitalWrite(10, 0);
    digitalWrite(11, 0);
  }
  //ldr end

  //Bluetooth start
  Blynk.run();
  Blynk.virtualWrite(V2, op);

  //bluetooth end

}
