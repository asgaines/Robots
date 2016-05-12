/*******************************************
 Sparki Line-following example
 
 Threshold is the value that helps you 
 determine what's black and white. Sparki's 
 infrared reflectance sensors indicate white 
 as close to 900, and black as around 200.
 This example uses a threshold of 700 for 
 the example, but if you have a narrow line, 
 or perhaps a lighter black, you may need to 
 adjust.
********************************************/

#include <Sparki.h> // include the sparki library
int startOfLoop; // Time since start of program, calculated at each loop beginning
const float pi = 3.1415926;

const float speedMotor = 0.3 / (10773.0 / 1000.0); // m / s
const float lengthAxle = 0.084; // meters

float posX = 0.0;
float posY = 0.0;
float theta = pi; // Initially facing left (180 degrees)

boolean turningRight;
boolean turningLeft;

int threshold = 700;

int lineLeft;
int lineCenter;
int lineRight;

void setup() {}

void loop() {
  startOfLoop = millis();
  
  lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  lineCenter = sparki.lineCenter(); // measure the center IR sensor
  lineRight  = sparki.lineRight();  // measure the right IR sensor

  if ( lineCenter < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
//    turningRight = false;
//    turningLeft = false;
    posX += cos(theta) * (speedMotor * 0.1);
    posY += sin(theta) * (speedMotor * 0.1);
    
    if (lineLeft < threshold && lineRight < threshold) {
      // Sparki is at origin. Tell him, cause he forgets
      posX = 0.0;
      posY = 0.0;
    }
  }
  else{
    if ( lineLeft < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
      theta += 2.0 * (speedMotor * 0.1) / lengthAxle;
//      turningLeft = true;
//      turningRight = false;
    }
  
    if ( lineRight < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      theta -= 2.0 * (speedMotor * 0.1) / lengthAxle;
//      turningRight = true;
//      turningLeft = false;
    }
  }

  sparki.clearLCD(); // wipe the screen
  
  sparki.print("X-position: "); // show left line sensor on screen
  sparki.println(posX);
  
  sparki.print("Y-position: "); // show center line sensor on screen
  sparki.println(posY);
  
  sparki.print("Theta: "); // show right line sensor on screen
  sparki.println((theta / 3.1415) * 180.0);
  
  sparki.updateLCD(); // display all of the information written to the screen


  // Stop Sparki for the calculations
//  sparki.moveStop();

  // Calculate the kinematics of Sparki
  
  // Calculate change in theta
//  if (turningRight && !turningLeft) {
    // Sparki is turning right
//    theta -= 2.0 * (speedMotor * 0.1) / lengthAxle;
//  } else if (turningLeft && !turningRight) {
    // Sparki is turning left
//    theta += 2.0 * (speedMotor * 0.1) / lengthAxle;
//  } // No change if moving straight forward

  // Keep theta between 0 and 2*pi
//  if (theta < 0) {
//    theta += 2 * pi;
//  } else if (theta > 2 * pi) {
//    theta -= 2 * pi;
//  }

  // Calculate change in position
//  if (!turningLeft && !turningRight) {
//    posX += cos(theta) * (speedMotor * 0.1);
//    posY += sin(theta) * (speedMotor * 0.1);
//  }

  delay(75);
//  while (millis() <= startOfLoop + 100){
//    // Wait and do nothing, this is used to ensure Sparki was moving for 100 msec
//  }

}
