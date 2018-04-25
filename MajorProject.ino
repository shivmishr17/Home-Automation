//Bluetooth
#define BLYNK_PRINT Serial


#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX

#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "66df23f89c9944deb50a80f5971bfa0b";

SoftwareSerial SerialBLE(7, 8); // RX, TX

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);
// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget

int op = 0;
int pinValue = 0;


BLYNK_WRITE(V1)
{

  terminal.print(op);
  terminal.println("%");
  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Marco") == param.asStr()) {
    terminal.println("You said: 'Marco'") ;
    terminal.println("I said: 'Polo'") ;
  } else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  terminal.flush();
}



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

//  // This will print Blynk Software version to the Terminal Widget when
//  // your hardware gets connected to Blynk Server
//  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
//  terminal.println(F("-------------"));
//  terminal.println(F("Type 'Marco' and get a reply, or type"));
//  terminal.println(F("anything else and get it printed back."));
//  terminal.flush();

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


  //  Serial.print("LDR:>>>");
  //  Serial.println(ldrVal);

  //  terminal.print(ldrVal);
  //  terminal.println();
  //  terminal.flush();
}
