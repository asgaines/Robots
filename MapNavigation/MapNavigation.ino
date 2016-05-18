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

byte envMap[numRows][numCols] = {
  {1, 1, 1, 0},
  {0, 0, 1, 0},
  {1, 1, 1, 1},
  {0, 1, 0, 1},
};

int cost[10][10] = {
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
};

int distance[10];
int previousNode[10];

byte startPosition[2] = {0, 0};
byte currentPosition[2] = {startPosition[0], startPosition[1]};
byte goalPosition[2] = {3, 1};

void setup() {
  sparki.clearLCD(); // wipe the screen
  //displayMap(); 
}

void loop() {
  

    costMatrix(envMap);
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


void costMatrix( byte mapE[4][4] ){
  int costMatrix[16][16];
  for(int i = 0; i < 16; i++)
  {
    for(int j = 0; j < 16; j++)
    {
      costMatrix[i][j] = 999;
    }
  }
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      if(mapE[i][j] == 1)
      {
        if(mapE[i][j+1] == 1 && j != 3)
        {
          costMatrix[j + 4 * i][j + 4 * i + 1] = 1; 
        }
        if(mapE[i+1][j] == 1 && i != 3)
        {
          costMatrix[j + 4 * i][j + 4 * i + 4] = 1;           
        }
      }
    }
  }
  for(int i = 0; i < 16; i++)
  {
    for(int j = 0; j < 16; j++)
    {
      sparki.clearLCD();
      sparki.print(i);
      sparki.print(",");
      sparki.print(j);
      sparki.print(" = "); 
      sparki.println(costMatrix[i][j]); 
      sparki.updateLCD();
      delay(1000); 
    }
  }
}

 
void dij(int n, int startNode,int cost[10][10],int dist[], int prevNode[])
{
  int i,minNode,count,w,flag[10],minVal;
  for(i=1;i<=n;i++){
    flag[i]=0;
    dist[i]=cost[startNode][i];
  }
  count=2;
  while(count<=n)
  {
    minVal=99;
    for(w=1;w<=n;w++){
      if(dist[w]<minVal && !flag[w]){
        minVal=dist[w];
        minNode=w;
        flag[minNode]=1;
        count++;
      }
    }
    for(w=1;w<=n;w++){
      if((dist[minNode]+cost[minNode][w]<dist[w]) && !flag[w]){
        dist[w]=dist[minNode]+cost[minNode][w];
        prevNode[w] = minNode;
//        sparki.print("minNode: ");
//        sparki.println(minNode);
      }
    }
  }
}
