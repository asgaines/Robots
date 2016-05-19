#include <iostream>
#include <stdio.h>
using namespace std;
#define infinity 999

// Map of obstacle course
const int numRows = 4;
const int numCols = 4;

bool envMap[numRows][numCols] = {
  {1, 1, 1, 0},
  {0, 0, 1, 0},
  {1, 1, 1, 1},
  {0, 1, 0, 1},
};

int distanceToNode[numRows * numCols];

bool startPosition[2] = {0, 0};
bool currentPosition[2] = {startPosition[0], startPosition[1]};
bool goalPosition[2] = {3, 1};

void setup() {
  //displayMap(); 
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

void dij(int n,int v,int** cost)
 {
  int i,u,count,w,flag[4],min;
  for(i=0;i<n;i++)
   flag[i]=0,distanceToNode[i]=cost[v][i];
  count=1;
  while(count<n)
  {
   min=99;
   for(w=0;w<n;w++)
    if(distanceToNode[w]<min && !flag[w])
     min=distanceToNode[w],u=w;
   flag[u]=1;
   count++;
   for(w=0;w<n;w++)
    if((distanceToNode[u]+cost[u][w]<distanceToNode[w]) && !flag[w])
     distanceToNode[w]=distanceToNode[u]+cost[u][w];
  }
  for (int d = 0; d < numRows * numCols; d++) {
    if (d != v) {
      printf("%d->%d,cost=%d\n",v,d,distanceToNode[d]);
    }
  }
}

int main() {
    int ** costMatrix = generateCostMatrix(envMap);

    dij(numRows * numCols, 13, costMatrix);
}

