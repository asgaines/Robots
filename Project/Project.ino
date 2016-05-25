#define infinity 999
#include <Sparki.h> // include the sparki library

const float pi = 3.1415926;

const float speedMotor = 0.3 / (10773.0 / 1000.0); // m / s
const float lengthAxle = 0.084; // meters

// For odometry
//float posX = 0.0;
//float posY = 0.0;

// Pixels on Sparki display
int displayRows = 64;
int displayCols = 128;

// The princess Position
int princessCol= 0;
int princessRow = 0;
int princessNode = infinity;
bool foundPrincess = 0;
bool returned = 0;

// Dimensions of line-following map
float mapWidth = 70.612; // cm
float mapHeight = 50.038; // cm

// Distance between Sparki's x, y start point and the corner of the map
float yStartDifference = 0.055;
float xStartDifference = 0.143;

// Map of obstacle course
const int numRows = 4;
const int numCols = 4;
bool envMap[numRows][numCols] = {
  {1, 1, 1, 0},
  {0, 0, 1, 0},
  {1, 1, 1, 1},
  {0, 1, 0, 1},
};
float theta = 0; // Initially facing right ("East" or 0 radians)

// Distance to each node
int distanceToNode[numRows * numCols];

byte startPos[2] = {0, 0};
byte currentPos[2] = {startPos[0], startPos[1]};
byte goalPos[2] = {3, 1};

void setup() {
 sparki.gripperOpen();
 delay(3000);
 sparki.gripperStop();    
 findPrincess();
  for (int node = 0; node < numCols * numRows; node++) {
    distanceToNode[node] = infinity;
  }
  dij(numRows * numCols, princessNode, distanceToNode);
 // displayMap(); 
}

int moveBackSize = 0;
int moveBack[16]; //1 for mvoe Up, 2 for move Right, 3 for move Down, and 4 for move Left

void loop() {
  if(!foundPrincess) {
        if (currentPos[0] != princessRow || currentPos[1] != princessCol){
    int currentNode = posToNode(currentPos[0], currentPos[1]);
    int moveToNode;
    
    // above, right, below, left
    // {distance, node}
    int possibleNodes[4][2] = {
      {infinity, 99},
      {infinity, 99},
      {infinity, 99},
      {infinity, 99},
    };

    // Check node above
    if (currentPos[0] != 0) {
      possibleNodes[0][0] = distanceToNode[currentNode - numCols];
      possibleNodes[0][1] = currentNode - numCols;
    }
    // Check node to right
    if (currentPos[1] != numCols - 1) {
      possibleNodes[1][0] = distanceToNode[currentNode + 1];
      possibleNodes[1][1] = currentNode + 1;
    }
    // Check node below
    if (currentPos[0] != numRows - 1) {
      possibleNodes[2][0] = distanceToNode[currentNode + numRows];
      possibleNodes[2][1] = currentNode + numRows;
    }
    // Check node to left
    if (currentPos[1] != 0) {
      possibleNodes[3][0] = distanceToNode[currentNode - 1];
      possibleNodes[3][1] = currentNode - 1;
    }

    int minStep1 = min(possibleNodes[0][0], possibleNodes[1][0]);
    int minStep2 = min(possibleNodes[2][0], possibleNodes[3][0]);
    int minStep = min(minStep1, minStep2);

    for (int directionNode = 0; directionNode < 4; directionNode++) {
      if (possibleNodes[directionNode][0] == minStep) {
        moveToNode = possibleNodes[directionNode][1];
        break;
      }
    }
    
    if (currentNode < numCols && moveToNode == currentNode - numRows) {
      // We should move up
      if (theta == 0) {
        sparki.moveLeft(90);
      } else if (theta == 180) {
        sparki.moveRight(90);
      } else if (theta == 270) {
        sparki.moveRight(180);
      }
      theta = 90;
      sparki.moveForward(mapHeight / numCols);
      currentPos[0] -= 1;
      moveBack[moveBackSize] = 1;
      moveBackSize++;
    } else if (currentNode % 4 != numCols - 1 && moveToNode == currentNode + 1) {
      // We should move right
      if (theta == 90) {
        sparki.moveRight(90);
      } else if (theta == 180) {
        sparki.moveRight(180);
      } else if (theta == 270) {
        sparki.moveLeft(90);
      }
      theta = 0;
      sparki.moveForward(mapWidth / numRows);
      currentPos[1] += 1;
      moveBack[moveBackSize] = 2;
      moveBackSize++;
    } else if (currentNode > numRows * (numCols - 1) - 1 < numCols && moveToNode == currentNode + numRows) {
      // We should move down
      if (theta == 0) {
        sparki.moveRight(90);
      } else if (theta == 180) {
        sparki.moveLeft(90);
      } else if (theta == 90) {
        sparki.moveRight(180);
      }
      theta = 270;
      sparki.moveForward(mapHeight / numCols);
      currentPos[0] += 1;
      moveBack[moveBackSize] = 3;
      moveBackSize++;
    } else if (currentNode % numRows != 0 && moveToNode == currentNode - 1) {
      // We should move left
      if (theta == 0) {
        sparki.moveLeft(180);
      } else if (theta == 90) {
        sparki.moveLeft(90);
      } else if (theta == 270) {
        sparki.moveRight(90);
      }
      theta = 180;
      sparki.moveForward(mapWidth / numRows);
      currentPos[1] -= 1;
      moveBack[moveBackSize] = 4;
      moveBackSize++;
    }
  } else {
    // We're there!
    if(!foundPrincess) {
   sparki.gripperClose(); // close the robot's gripper
   delay(3000);           // for 1 second (1000 milliseconds)
   sparki.gripperStop();
//    sparki.clearLCD();
//    sparki.drawCircle(displayCols / 2 - 20, 25, 10); //eye 1
//    sparki.drawCircle(displayCols / 2 + 20, 25, 10); //eye 2 
//    sparki.drawCircleFilled(displayCols / 2 - 20, 25, 2); //pupil 1
//    sparki.drawCircleFilled(displayCols / 2 + 20, 25, 2); //pupil 2
//    sparki.drawLine(displayCols / 2 - 30, 45, displayCols / 2 - 15, 55);
//    sparki.drawLine(displayCols / 2 - 15, 55, displayCols / 2 + 15, 55);
//    sparki.drawLine(displayCols / 2 + 15, 55, displayCols / 2 + 30, 45);
//    sparki.updateLCD();      
   }
    foundPrincess = 1;
    //Reset the robot's rotation to zero
    if( theta == 180) {
    sparki.moveLeft(180);      
    } else if( theta == 90) {
      sparki.moveLeft(90);
    } else if( theta == 270) {
      sparki.moveRight(90);
    }
  }
  if(foundPrincess) {
    if(!returned) {
          for(int moveNumber = moveBackSize - 1; moveNumber >= 0; moveNumber--) {
       if (moveBack[moveNumber] == 1){
          // We should move up
          if (theta == 0) {
            sparki.moveLeft(90);
          } else if (theta == 180) {
            sparki.moveRight(90);
          } else if (theta == 270) {
            sparki.moveRight(180);
          }
          theta = 90;
          sparki.moveForward(mapHeight / numCols);
          currentPos[0] -= 1;
        } else if (moveBack[moveNumber] == 2) {
          // We should move right
          if (theta == 90) {
            sparki.moveRight(90);
          } else if (theta == 180) {
            sparki.moveRight(180);
          } else if (theta == 270) {
            sparki.moveLeft(90);
          }
          theta = 0;
          sparki.moveForward(mapWidth / numRows);
          currentPos[1] += 1;
        } else if (moveBack[moveNumber] == 3) {
          // We should move down
          if (theta == 0) {
            sparki.moveRight(90);
          } else if (theta == 180) {
            sparki.moveLeft(90);
          } else if (theta == 90) {
            sparki.moveRight(180);
          }
          theta = 270;
          sparki.moveForward(mapHeight / numCols);
          currentPos[0] += 1;
        } else if (moveBack[moveNumber] == 4) {
          // We should move left
          if (theta == 0) {
            sparki.moveLeft(180);
          } else if (theta == 90) {
            sparki.moveLeft(90);
          } else if (theta == 270) {
            sparki.moveRight(90);
          }
          theta = 180;
          sparki.moveForward(mapWidth / numRows);
          currentPos[1] -= 1;
        }
      }
      sparki.clearLCD();
      sparki.print("Done");
      sparki.updateLCD();
      sparki.moveStop();
      returned = 1;
    }
}

  }
  

// Odometry code not used for navigating map here
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
  
//  sparki.print("X-position: "); // show left line sensor on screen
//  sparki.println(posX);
//  
//  sparki.print("Y-position: "); // show center line sensor on screen
//  sparki.println(posY);
//  
//  sparki.print("Theta: "); // show right line sensor on screen
//  sparki.println((theta / 3.1415) * 180.0);
  

//  sparki.clearLCD();
//  // Print out the pixel on the graph for where Sparki currently is
//  sparki.drawPixel(((xStartDifference - posX) * 127.0 / mapWidth), ((yStartDifference + posY) * 63.0 / mapHeight));
  
//  sparki.updateLCD(); // display all of the information written to the screen
//
//  while (millis() < startOfLoop + 100){
//    // Wait and do nothing, this is used to ensure Sparki was moving for 100 msec
//  }
  
//  delay(1000);
//  sparki.moveForward();
//  delay(200);
//  sparki.moveBackward();
//  delay(200);
//  sparki.moveStop();
    
}

void displayMap() {
  sparki.clearLCD();
  int widthRow = displayRows / numRows;
  int widthCol = displayCols / numCols;
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (!envMap[row][col]){
        sparki.drawRectFilled(widthCol * col, widthRow * row, widthCol, widthRow);
      }
    }
  }
  sparki.updateLCD();
}

int cost(int start, int goal){
  if (start == goal) {
    return 0;
  }
  int dist = 0;
  int x1 = start / numRows;
  int x2 = goal / numRows;
  int y1 = start % numCols;
  int y2 = goal % numCols;
  
  if (envMap[x1][y1] == 0 || envMap[x2][y2] == 0) {
    return infinity;
  }
  
  dist = abs(x2-x1)+abs(y2-y1);
  
  if (dist > 1) {
    return infinity;
  } else {
    return 1;
  }
}

void dij(int n,int v,int distance[])
{
  int i,u,count,w,flag[numCols * numRows],min;
  for(i=0;i<n;i++)
    flag[i]=0,distance[i]=cost(v,i);
  count=1;
  while(count<n)
  {
    min=infinity;
    for(w=0;w<n;w++)
      if(distance[w]<min && !flag[w])
      min=distance[w],u=w;
    flag[u]=1;
    count++;
    for(w=0;w<n;w++)
      if((distance[u]+cost(u,w)<distance[w]) && !flag[w])
        distance[w]=distance[u]+cost(u,w);
  }
}

// Convert x, y position to node
byte posToNode(byte posX, byte posY) {
  return posX * numRows + posY;
}

void findPrincess() {
  // Sparki initially facing 0 (East)
  // To account for limited range of motion, turn Sparkster 45 degrees right first
  const int servoStart = -50;
  const int servoEnd = 50;
  sparki.moveRight(45);
  sparki.servo(servoStart);

  int readings[servoEnd + abs(servoStart)];
  // Sweep for the princess
  for (int angle = servoStart; angle < servoEnd; angle++) {
    sparki.servo(angle);
    readings[angle + abs(servoStart)] = sparki.ping();
  }

  // Find the princess in the data. She won't be more than 70 cm away on our map
  int greatestDistance = 70; 
  int leastDistance = 5;
  int numAcceptedReadings = 0;
  int acceptedReadings[servoEnd + abs(servoStart)][2]; // {distance, angle}

  // Filter the readings, first few readings are always off
  for (int index = 3; index < servoEnd + abs(servoStart); index++) {
    if (readings[index] < greatestDistance && readings[index] > leastDistance) {
      acceptedReadings[numAcceptedReadings][0] = readings[index];
      acceptedReadings[numAcceptedReadings][1] = index - abs(servoStart);
      numAcceptedReadings++;
    }
  }

  float aveDist = 0.0;
  float aveAngle = 0.0;
  for (int acceptedReading = 0; acceptedReading < numAcceptedReadings; acceptedReading++) {
    aveDist += acceptedReadings[acceptedReading][0];
    aveAngle += acceptedReadings[acceptedReading][1];
  }

  aveDist /= numAcceptedReadings;
  aveAngle /= numAcceptedReadings;

  Serial.print("distance: ");
  Serial.print(aveDist);
  Serial.print(" angle: ");
  Serial.println(aveAngle);

//  int variance = 0;
//   for(int reading = 0; reading < numAcceptedReadings; reading++) {
//      variance = variance + (acceptedReadings[reading][0] - aveDist) * (acceptedReadings[reading][0] - aveDist); 
//    }
//    variance = variance / numAcceptedReadings;

  // Position of princess
  float princessX = cos((45 + aveAngle) / 360.0 * 2 * pi) * aveDist;
  float princessY = sin((45 + aveAngle) / 360.0 * 2 * pi) * aveDist;
  // Account for Sparki being in middle of node when measuring
  princessX += (mapWidth / 4.0 / 2.0);// - variance;
  princessY += (mapHeight / 4.0 / 2.0);// - variance;

  Serial.print("princessX: ");
  Serial.print(princessX);
  Serial.print(" princessY: ");
  Serial.println(princessY);

  princessCol = princessX / (mapWidth / numCols);
  princessRow = princessY / (mapHeight / numRows);
  
  Serial.print("princess row: ");
  Serial.print(princessRow);
  Serial.print(" princess column: ");
  Serial.println(princessCol);

  princessNode = posToNode(princessRow, princessCol);

  sparki.moveLeft(45);
  sparki.clearLCD();
  sparki.print(princessX);
  sparki.print(" , ");
  sparki.println(princessY);
  sparki.print("princess row: ");
  sparki.println(princessRow);
  sparki.print("princess column: ");
  sparki.println(princessCol);
  sparki.print("Node: ");
  sparki.println(princessNode);
  sparki.updateLCD();
  
}
