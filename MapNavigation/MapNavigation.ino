#define infinity 999
#include <Sparki.h> // include the sparki library

unsigned long startOfLoop; // Time since start of program, calculated at each loop beginning
const float pi = 3.1415926;

const float speedMotor = 0.3 / (10773.0 / 1000.0); // m / s
const float lengthAxle = 0.084; // meters

// For odometry
//float posX = 0.0;
//float posY = 0.0;

int threshold = 700;
int lineLeft;
int lineCenter;
int lineRight;

// Pixels on Sparki display
int displayRows = 64;
int displayCols = 128;

// Dimensions of line-following map
float mapWidth = 0.595; // meters
float mapHeight = 0.415; // meters

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
  for (int node = 0; node < numCols * numRows; node++) {
    distanceToNode[node] = infinity;
  }
  dij(numRows * numCols, posToNode(goalPos[0], goalPos[1]), distanceToNode);
  displayMap(); 
}

void loop() {
  if (currentPos[0] != goalPos[0] || currentPos[1] != goalPos[1]){
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
      sparki.moveForward(mapHeight / numCols * 100.0);
      currentPos[0] -= 1;
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
      sparki.moveForward(mapWidth / numRows * 100.0);
      currentPos[1] += 1;
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
      sparki.moveForward(mapHeight / numCols * 100.0);
      currentPos[0] += 1;
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
      sparki.moveForward(mapWidth / numRows * 100.0);
      currentPos[1] -= 1;
    }
  } else {
    // We're there!
    sparki.clearLCD();
    sparki.drawCircle(displayCols / 2 - 20, 25, 10); //eye 1
    sparki.drawCircle(displayCols / 2 + 20, 25, 10); //eye 2 
    sparki.drawCircleFilled(displayCols / 2 - 20, 25, 2); //pupil 1
    sparki.drawCircleFilled(displayCols / 2 + 20, 25, 2); //pupil 2
    sparki.drawLine(displayCols / 2 - 30, 45, displayCols / 2 - 15, 55);
    sparki.drawLine(displayCols / 2 - 15, 55, displayCols / 2 + 15, 55);
    sparki.drawLine(displayCols / 2 + 15, 55, displayCols / 2 + 30, 45);
    sparki.updateLCD();
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

// Originally used to generate the adjacency list
//int ** generateCostMatrix( bool mapE[numRows][numCols] ){
//  int** costMatrix = 0;
//  costMatrix = new int*[numRows * numCols];
//
//  for(int i = 0; i < numRows * numCols; i++)
//  {
//    costMatrix[i] = new int[numRows * numCols];
//    for(int j = 0; j < numRows * numCols; j++)
//    {
//      costMatrix[i][j] = infinity;
//    }
//  }
//  for(int i = 0; i < numRows; i++)
//  {
//    for(int j = 0; j < numCols; j++)
//    {
//      if(mapE[i][j] == 1)
//      {
//        if(mapE[i][j+1] == 1 && j != numCols - 1)
//        {
//          // The position has an available neighbor spot to right
//          costMatrix[j + numRows * i][j + numRows * i + 1] = 1;
//          costMatrix[j + numRows * i + 1][j + numRows * i] = 1;
//        }
//        if(mapE[i+1][j] == 1 && i != numRows - 1)
//        {
//          // The position has an available neighbor spot below
//          costMatrix[j + numRows * i][j + numRows * i + numRows] = 1;
//          costMatrix[j + numRows * i + numRows][j + numRows * i] = 1;
//        }
//      }
//    }
//  }
//  return costMatrix;
//}

void dij(int n,int v,int distance[])//int** cost)
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
