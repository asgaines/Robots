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

const float speedMotor = 0.3 / (10773 / 1000); // m / s
const float lengthAxle = 0.084; // meters

float xPos = 0;
float yPos = 0;
float theta = pi; // Initially facing left (180 degrees)

// If not moving forward, moving backwards instead
boolean rightForward = true;
boolean leftForward = true;

int threshold = 700;

int lineLeft;
int lineCenter;
int lineRight;

void setup() 
{
}

void loop() {
  startOfLoop = millis();
  
  lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  lineCenter = sparki.lineCenter(); // measure the center IR sensor
  lineRight  = sparki.lineRight();  // measure the right IR sensor

//  if ( lineCenter < threshold ) // if line is below left line sensor
//  {  
//    sparki.moveForward(); // move forward
//    rightForward = true;
//    leftForward = true;
//  }
//  else{
//    if ( lineLeft < threshold ) // if line is below left line sensor
//    {  
//      sparki.moveLeft(); // turn left
//      leftForward = false;
//      rightForward = true;
//    }
//  
//    if ( lineRight < threshold ) // if line is below right line sensor
//    {  
//      sparki.moveRight(); // turn right
//      leftForward = true;
//      rightForward = false;
//    }
//  }
  
  sparki.moveLeft();
  leftForward = true;
  rightForward = false;

  sparki.clearLCD(); // wipe the screen
  
  sparki.print("X-position: "); // show left line sensor on screen
  sparki.println(xPos);
  
  sparki.print("Y-position: "); // show center line sensor on screen
  sparki.println(yPos);
  
  sparki.print("Theta: "); // show right line sensor on screen
  sparki.println(theta);
  
  sparki.updateLCD(); // display all of the information written to the screen

  while (millis() - startOfLoop <= 100){
    // Wait and do nothing, this is used to ensure Sparki was moving for 100 msec
  }

  // Calculate the kinematics of Sparki
  
  // Calculate change in theta
  if (leftForward && !rightForward) {
    theta -= speedMotor / lengthAxle;// % (2 * pi);
  } else if (rightForward && !leftForward) {
    theta += speedMotor / lengthAxle;// % (2 * pi);
  } // No change if moving straight forward
  
   
}
