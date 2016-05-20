#define NO_LCD 
#include  <Sparki.h>; // include the sparki library 

const int startAngle=-30;
int angle = startAngle;
const int endAngle = 30;
const int numReadings = endAngle - startAngle;
int counter = 0;
int distances[numReadings];
int numAcceptedValues;
int acceptedValues[numReadings][2]; // Store {distance, angle}

int targetDistance = 10;

void setup() {
} 

void loop() { 
  sparki.servo(angle);
  counter++;
  angle=angle+1; 
  
  // measures the distance with Sparki's eyes 
  int cm = sparki.ping();
  distances[counter] = cm;

  // Only accept the values within 5cm of target
  // Second reading lies......
  if (cm > targetDistance - 5 && cm < targetDistance + 5) {
    acceptedValues[numAcceptedValues][0] = cm;
    acceptedValues[numAcceptedValues][1] = angle;
    numAcceptedValues++;
  }

  if(angle>endAngle){ 
    angle=startAngle;
    counter = 0;
    for (int index = 0; index < numReadings; index++) {
      Serial.print(distances[index]);
      Serial.print(", ");
    }
    Serial.println();

    for (int reading = 0; reading < numAcceptedValues; reading++) {
      Serial.print("Distance: ");
      Serial.print(acceptedValues[reading][0]);
      Serial.print(", angle: ");
      Serial.println(acceptedValues[reading][1]);
    }
    // Prepare for next sweep of readings
    numAcceptedValues = 0;    
  }  
} 
