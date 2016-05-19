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

#define infinity 999
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

// Dimensions of line-following map
float mapWidth = 0.595; // meters
float mapHeight = 0.415; // meters

// Distance between Sparki's x, y start point and the corner of the map
float yStartDifference = 0.055;
float xStartDifference = 0.143;

bool envMap[numRows][numCols] = {
  {1, 1, 1, 0},
  {0, 0, 1, 0},
  {1, 1, 1, 1},
  {0, 1, 0, 1},
};

byte startPosition[2] = {0, 0};
byte currentPosition[2] = {startPosition[0], startPosition[1]};
byte goalPosition[2] = {3, 1};

void setup() {
  int ** costMatrix = generateCostMatrix(envMap);
  int distance[numRows * numCols];
  dij(numRows * numCols, 0, costMatrix, distance);
  
  sparki.clearLCD(); // wipe the screen
  //displayMap(); 
}

void loop() {
  
    /*dij(10, 0, cost, distance, previousNode);
    for(int i = 0; i < 10; i++)
  {
    sparki.clearLCD();
    sparki.print("Distance ");
    sparki.print(i);
    sparki.print(" = ");
    sparki.println(distance[i]);
    sparki.updateLCD();
    delay(1000);
  }
*/
  
  startOfLoop = millis();
  
  lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  lineCenter = sparki.lineCenter(); // measure the center IR sensor
  lineRight  = sparki.lineRight();  // measure the right IR sensor

  if ( lineCenter < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
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
    }
  
    if ( lineRight < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
      theta -= 2.0 * (speedMotor * 0.1) / lengthAxle;
    }
  }
  
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
  
  sparki.updateLCD(); // display all of the information written to the screen

  while (millis() < startOfLoop + 100){
    // Wait and do nothing, this is used to ensure Sparki was moving for 100 msec
  }
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

int ** generateCostMatrix( bool mapE[numRows][numCols] ){
  int** costMatrix = 0;
  costMatrix = new int*[numRows * numCols];

  for(int i = 0; i < numRows * numCols; i++)
  {
    costMatrix[i] = new int[numRows * numCols];
    for(int j = 0; j < numRows * numCols; j++)
    {
      costMatrix[i][j] = infinity;
    }
  }
  for(int i = 0; i < numRows; i++)
  {
    for(int j = 0; j < numCols; j++)
    {
      if(mapE[i][j] == 1)
      {
        if(mapE[i][j+1] == 1 && j != numCols - 1)
        {
          // The position has an available neighbor spot to right
          costMatrix[j + numRows * i][j + numRows * i + 1] = 1;
          costMatrix[j + numRows * i + 1][j + numRows * i] = 1;
        }
        if(mapE[i+1][j] == 1 && i != numRows - 1)
        {
          // The position has an available neighbor spot below
          costMatrix[j + numRows * i][j + numRows * i + numRows] = 1;
          costMatrix[j + numRows * i + numRows][j + numRows * i] = 1;
        }
      }
    }
  }
  return costMatrix;
}

void dij(int n,int v,int** cost,int dist[])
{
  int i,u,count,w,flag[4],min;
  for(i=0;i<n;i++)
    flag[i]=0,dist[i]=cost[v][i];
  count=1;
  while(count<n)
  {
    min=99;
    for(w=0;w<n;w++)
      if(dist[w]<min && !flag[w])
      min=dist[w],u=w;
    flag[u]=1;
    count++;
    for(w=0;w<n;w++)
      if((dist[u]+cost[u][w]<dist[w]) && !flag[w])
        dist[w]=dist[u]+cost[u][w];
  }

  // Temporarily, this is only place we have access to distance
  // Could use global variable or return from function
}
