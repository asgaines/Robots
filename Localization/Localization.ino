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

// For calculating distances between two beacons: r1 and r2
int r1Readings[numReadings];
int numr1Readings = 0;
int r2Readings[numReadings];
int numr2Readings = 0;
int r1Ave = 0; // Distance from r1 beacon
int r2Ave = 0; // Distance from r2 beacon

float L = 0.1; // Distance between beacons
// Sparki's position
float x = 0.0;
float y = 0.0;

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
  
  // Only accept the values within 10cm of target
  // Second reading lies......
  if (cm > targetDistance - 10 && cm < targetDistance + 10) {
    acceptedValues[numAcceptedValues][0] = cm;
    acceptedValues[numAcceptedValues][1] = angle;
    numAcceptedValues++;
  }

  if(angle>endAngle){ 
    angle=startAngle;
    counter = 0;

    for (int index = 0; index < numReadings; index++) {
      Serial.println(distances[index]);
    }

    // Scan through distance measurements, sort into r1, r2 arrays
    for (int index = 2; index < numReadings; index++) { // Ignore first 2 inaccurate values
      while (distances[index] == -1) {} // Ignore leading -1 values
      while (distances[index] != -1) {
        r1Readings[numr1Readings] = distances[index];
        numr1Readings++;
      }
      while (distances[index] == -1) {} // Ignore -1 values between r1, r2
      while (distances[index] != -1) {
        r2Readings[numr2Readings] = distances[index];
        numr2Readings++;
      }
      break; // No need to check any more values beyond r2
    }

    // Accumulate all r1 readings
    for (int index = 0; index < numr1Readings; index++) {
      r1Ave += r1Readings[index];
    }
    r1Ave = r1Ave / numr1Readings;

    // Accumulate all r2 readings
    for (int index = 0; index < numr2Readings; index++) {
      r2Ave += r2Readings[index];
    }
    r2Ave = r2Ave / numr2Readings;

    // Calculate Sparki's x, y coordinates
    x = (L * L + r1Ave * r1Ave - r2Ave * r2Ave) / (2 * L);
    y = sqrt(r1Ave * r1Ave - x * x);
    sparki.clearLCD();
    sparki.print("x: ");
    sparki.println(x);
    sparki.print("y: ");
    sparki.println(y);
    sparki.updateLCD();
    
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
