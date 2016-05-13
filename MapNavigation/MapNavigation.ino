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
unsigned long startOfLoop; // Time since start of program, calculated at each loop beginning
const float pi = 3.1415926;

const float speedMotor = 0.3 / (10773.0 / 1000.0); // m / s
const float lengthAxle = 0.084; // meters

float posX = 0.0;
float posY = 0.0;
float theta = pi; // Initially facing left ("West" or 180 degrees)

int threshold = 700;

int lineLeft;
int lineCenter;
int lineRight;

// Map of obstacle course
const int numRows = 4;
const int numCols = 4;
int displayRows = 64;
int displayCols = 128;

byte envMap[numRows][numCols] = {
  {0, 0, 0, 1},
  {1, 1, 0, 1},
  {0, 0, 0, 0},
  {1, 0, 1, 0},
};

byte startPosition[2] = {0, 0};
byte currentPosition[2] = {startPosition[0], startPosition[1]};
byte goalPosition[2] = {3, 1};

void setup() {
  displayMap(); 
}

void loop() {
//  startOfLoop = millis();
//  
//  lineLeft   = sparki.lineLeft();   // measure the left IR sensor
//  lineCenter = sparki.lineCenter(); // measure the center IR sensor
//  lineRight  = sparki.lineRight();  // measure the right IR sensor
//
//  if ( lineCenter < threshold ) // if line is below left line sensor
//  {  
//    sparki.moveForward(); // move forward
//    posX += cos(theta) * (speedMotor * 0.1);
//    posY += sin(theta) * (speedMotor * 0.1);
//    
//    if (lineLeft < threshold && lineRight < threshold) {
//      // Sparki is at origin. Tell him, cause he forgets
//      posX = 0.0;
//      posY = 0.0;
//    }
//  }
//  else{
//    if ( lineLeft < threshold ) // if line is below left line sensor
//    {  
//      sparki.moveLeft(); // turn left
//      theta += 2.0 * (speedMotor * 0.1) / lengthAxle;
//    }
//  
//    if ( lineRight < threshold ) // if line is below right line sensor
//    {  
//      sparki.moveRight(); // turn right
//      theta -= 2.0 * (speedMotor * 0.1) / lengthAxle;
//    }
//  }
//
//  sparki.clearLCD(); // wipe the screen
//  
//  sparki.print("X-position: "); // show left line sensor on screen
//  sparki.println(posX);
//  
//  sparki.print("Y-position: "); // show center line sensor on screen
//  sparki.println(posY);
//  
//  sparki.print("Theta: "); // show right line sensor on screen
//  sparki.println((theta / 3.1415) * 180.0);
//  
//  sparki.updateLCD(); // display all of the information written to the screen
//
//  while (millis() < startOfLoop + 100){
//    // Wait and do nothing, this is used to ensure Sparki was moving for 100 msec
//  }
}

void displayMap() {
  sparki.clearLCD();
  int widthRow = displayRows / numRows;
  int widthCol = displayCols / numCols;
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (envMap[row][col]){
//        sparki.print("(");
//        sparki.print(row);
//        sparki.print(",");
//        sparki.print(col);
//        sparki.print(") ");
        
        sparki.drawRectFilled(widthCol * col, widthRow * row, widthCol * (col + 1), widthRow * (row + 1));
      }
    }
  }
  sparki.updateLCD();
}

