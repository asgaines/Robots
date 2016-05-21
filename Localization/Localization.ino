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

int targetDistance = 30;
float mean;
float variance;

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

    sparki.clearLCD();


    for (int reading = 0; reading < numAcceptedValues; reading++) {
      Serial.print("Distance: ");
      sparki.print(acceptedValues[reading][0]);
      sparki.print(" ");
      Serial.print(", angle: ");
      Serial.println(acceptedValues[reading][1]);
      mean = mean + acceptedValues[reading][0];
    }
    sparki.println();
    mean = mean / numAcceptedValues;
    sparki.print("Mean = ");
    sparki.println(mean);

    for(int reading = 0; reading < numAcceptedValues; reading++) {
      variance = variance + (acceptedValues[reading][0] - mean) * (acceptedValues[reading][0] - mean); 
    }
    variance = variance / numAcceptedValues;

    sparki.print("Variance = ");
    sparki.println(variance);
    sparki.updateLCD();

    //10cm: variance = approximately 0.29
    //20cm: variance = approximately 0.62 - 0.80
    //30cm: variance = approximately 1.16
    //40cm: variance = approximately 1.58
    
    // Prepare for next sweep of readings
    numAcceptedValues = 0;    
  }  
} 
